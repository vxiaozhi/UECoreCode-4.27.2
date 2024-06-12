// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreTypes.h"
#include "HAL/UnrealMemory.h"
#include "Misc/AssertionMacros.h"
#include "Templates/IsSigned.h"
#include "Serialization/BitWriter.h"
#include "Serialization/BitReader.h"

/**
 * @brief Util class to manage history of received sequence numbers 
 * 
 * TSequenceHistory是用来管理接收到的序列号历史记录的.
 * 当我们接收包的时候，一般会产生一个 Ack 或者 Nak，Ack 是 1，Nak 是 0，按顺序写入 Storage 中，
 * Storage 是一个 uint32 数组，最多存储 256 位，当超过 MaxSequenceHistoryLength 的时候，会执行 FlushNet 立即发送。
 * 结构清晰了，那么判断某个序列号是 Ack 或者 Nak 的时候，只需要根据索引查找具体的位判断是否为 1 即可。
 * 写入的时候根据序列号的数量写入对应数量的 WordT 即可。
 *  
 * */
template <SIZE_T HistorySize>
class TSequenceHistory
{
public:	
	typedef uint32 WordT;
	
	static constexpr SIZE_T BitsPerWord = sizeof(WordT) * 8;   // = 32
	static constexpr SIZE_T WordCount = HistorySize / BitsPerWord;	// = 8
	static constexpr SIZE_T MaxSizeInBits = WordCount * BitsPerWord;
	static constexpr SIZE_T Size = HistorySize;

	static_assert(HistorySize > 0, "HistorySize must be > 0");
	static_assert(HistorySize % BitsPerWord == 0, "InMaxHistorySize must be a modulo of the wordsize");

public:	
	TSequenceHistory();

#if WITH_DEV_AUTOMATION_TESTS
	explicit TSequenceHistory(WordT Value, SIZE_T Count = 1);
	WordT* Data() { return &Storage[0]; }
#endif
	/** Reset */
	void Reset();

	/** Store delivery status, oldest will be dropped */
	void AddDeliveryStatus(bool Delivered);

	/** Query the status of a specific index, index 0 is last stored status */
	bool IsDelivered(SIZE_T Index) const;
	
	bool operator==(const TSequenceHistory& Other) const { return FMemory::Memcmp(Storage, Other.Storage, WordCount * sizeof (WordT)) == 0; }

	bool operator!=(const TSequenceHistory& Other) const { return FMemory::Memcmp(Storage, Other.Storage, WordCount * sizeof (WordT)) != 0; }

	/** Write history to BitStream */
	void Write(FBitWriter& Writer, SIZE_T NumWords) const;

	/** Read history from BitStream */
	void Read(FBitReader& Reader, SIZE_T NumWords);

private:
	WordT Storage[WordCount];
};

template<SIZE_T HistorySize>
constexpr SIZE_T TSequenceHistory<HistorySize>::BitsPerWord;

template<SIZE_T HistorySize>
constexpr SIZE_T TSequenceHistory<HistorySize>::WordCount;

template<SIZE_T HistorySize>
constexpr SIZE_T TSequenceHistory<HistorySize>::MaxSizeInBits;

template<SIZE_T HistorySize>
constexpr SIZE_T TSequenceHistory<HistorySize>::Size;

template <SIZE_T HistorySize>
TSequenceHistory<HistorySize>::TSequenceHistory()
{
	Reset();
}

#if WITH_DEV_AUTOMATION_TESTS
template <SIZE_T HistorySize>
TSequenceHistory<HistorySize>::TSequenceHistory(WordT Value, SIZE_T Count)
{
	Reset();
	for (SIZE_T CurrentWordIt = 0; CurrentWordIt < Count; ++CurrentWordIt)
	{
		Storage[CurrentWordIt] = Value;
	}	
}
#endif 

template <SIZE_T HistorySize>
void TSequenceHistory<HistorySize>::Reset()
{
	FPlatformMemory::Memset(&Storage[0], 0, WordCount * sizeof(WordT));
}

/**
 * @brief AddDeliveryStatus 函数用于添加一个新的交付状态到历史记录中。
 * 这个过程实际上是在将所有的交付状态向左移动一位，并将新的交付状态添加到最低位。这样，最旧的交付状态（即最高位）会被移出 Storage，而新的交付状态则会被添加到最低位。
 * @tparam HistorySize 
 * @param Delivered 
 */
template <SIZE_T HistorySize>
void TSequenceHistory<HistorySize>::AddDeliveryStatus(bool Delivered)
{
	WordT Carry = Delivered ? 1u : 0u;
	// ValueMask，其值为 1 左移 BitsPerWord - 1 位。用于获取 Storage 中每个元素的最高位。
	const WordT ValueMask = 1u << (BitsPerWord - 1);
	
	for (SIZE_T CurrentWordIt = 0; CurrentWordIt < WordCount; ++CurrentWordIt)
	{
		const WordT OldValue = Carry;
		
		// carry over highest bit in each word to the next word
		Carry = (Storage[CurrentWordIt] & ValueMask) >> (BitsPerWord - 1);
		Storage[CurrentWordIt] = (Storage[CurrentWordIt] << 1u) | OldValue;
	}
}

/**
 * @brief 判断index 对应的bit 位是否为1
 * 
 * @tparam HistorySize 
 * @param Index  按 bit 的索引号
 * @return true 
 * @return false 
 */
template <SIZE_T HistorySize>
bool TSequenceHistory<HistorySize>::IsDelivered(SIZE_T Index) const
{
	check(Index < Size);

	const SIZE_T WordIndex = Index / BitsPerWord;
	const WordT WordMask = (WordT(1) << (Index & (BitsPerWord - 1)));
	
	return (Storage[WordIndex] & WordMask) != 0u;
}

template <SIZE_T HistorySize>
void TSequenceHistory<HistorySize>::Write(FBitWriter& Writer, SIZE_T NumWords) const
{
	NumWords = FPlatformMath::Min(NumWords, WordCount);
	for (SIZE_T CurrentWordIt = 0; CurrentWordIt < NumWords; ++CurrentWordIt)
	{
		WordT temp = Storage[CurrentWordIt];
		Writer << temp;
	}
}

template <SIZE_T HistorySize>
void TSequenceHistory<HistorySize>::Read(FBitReader& Reader, SIZE_T NumWords)
{
	NumWords = FPlatformMath::Min(NumWords, WordCount);
	for (SIZE_T CurrentWordIt = 0; CurrentWordIt < NumWords; ++CurrentWordIt)
	{
		Reader << Storage[CurrentWordIt];
	}
}
