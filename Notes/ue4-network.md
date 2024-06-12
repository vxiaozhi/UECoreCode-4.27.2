# UE4 网络模块          {#page-network}
=============

## 术语解释

- ACK(ACKnowledge Character) 在数据传输通信中，接收方发送给发送方的一种传输控制字符，它表示确认发来的数据已经接受无误。
- NAK(Negative Acknowlegement) 是否定应答或者非应答的缩小。它是一个用于数据通信中确认数据收到但有小错误的信号。

## 连接建立过程

## 网络同步

## 源码分析

### 1. packet 收包发包入口

收包 

```
void UNetConnection::ReceivedRawPacket( void* InData, int32 Count )
{
    //...
    FBitReader Reader(Data, BitSize);

    // Set the network version on the reader
    Reader.SetEngineNetVer( EngineNetworkProtocolVersion );
    Reader.SetGameNetVer( GameNetworkProtocolVersion );

    if (Handler.IsValid())
    {
        Handler->IncomingHigh(Reader);
    }

    if (Reader.GetBitsLeft() > 0)
    {
        ReceivedPacket(Reader);

        // Check if the out of order packet cache needs flushing
        FlushPacketOrderCache();
    }
    //...
}
```
```

void UNetConnection::ReceivedPacket( FBitReader& Reader, bool bIsReinjectedPacket)
{
    // ...
    // Read packet header
		FNetPacketNotify::FNotificationHeader Header;
		if (!PacketNotify.ReadHeader(Header, Reader))
		{
			CLOSE_CONNECTION_DUE_TO_SECURITY_VIOLATION(this, ESecurityEvent::Malformed_Packet, TEXT("Failed to read PacketHeader"));
			return;
		}
    // ...
}

	/**
	 * @brief 这是网络数据的包头结构，每个数据包会携带当前的序列号信息。
	 * OutSeq 是发送序列号，当 FlushNet 发包的时候才会自增；
	 * InAckSeq 是接收序列号，当我们收包的时候，不管是 Ack 还是 Nak，都会自增；
	 * WrittenHistoryWordCount 是记录的历史序列号的数量对 BitsPerWord 求余的结果，最小是1，最大是8。
	 * 
	 * 包头序列化的时候会压缩在一个 uint32 中，14 位的 Seq，14 位的 AckedSeq，4位的 HistoryWordCount。 
	 * 4位是因为历史记录数组最大数量是8，14位是因为兼容历史？
	 * 
	 */
	struct FNotificationHeader
	{
		SequenceHistoryT History;
		SIZE_T HistoryWordCount;  // = WrittenHistoryWordCount
		SequenceNumberT Seq;	// = OutSeq
		SequenceNumberT AckedSeq;	// = InAckSeq
	};



```

```
// These methods must always write and read the exact same number of bits, that is the reason for not using WriteInt/WrittedWrappedInt
bool FNetPacketNotify::WriteHeader(FBitWriter& Writer, bool bRefresh)
{
	// we always write at least 1 word
	SIZE_T CurrentHistoryWordCount = FMath::Clamp<SIZE_T>((GetCurrentSequenceHistoryLength() + SequenceHistoryT::BitsPerWord - 1u) / SequenceHistoryT::BitsPerWord, 1u, SequenceHistoryT::WordCount);

	// We can only do a refresh if we do not need more space for the history
	if (bRefresh && (CurrentHistoryWordCount > WrittenHistoryWordCount))
	{
		return false;
	}

	// How many words of ack data should we write? If this is a refresh we must write the same size as the original header
	WrittenHistoryWordCount = bRefresh ? WrittenHistoryWordCount : CurrentHistoryWordCount;
	// This is the last InAck we have acknowledged at this time
	WrittenInAckSeq = InAckSeq;

	SequenceNumberT::SequenceT Seq = OutSeq.Get();
	SequenceNumberT::SequenceT AckedSeq = InAckSeq.Get();

	// Pack data into a uint
	uint32 PackedHeader = FPackedHeader::Pack(Seq, AckedSeq, WrittenHistoryWordCount - 1);

	// Write packed header
	Writer << PackedHeader;

	// Write ack history
	InSeqHistory.Write(Writer, WrittenHistoryWordCount);

	UE_LOG_PACKET_NOTIFY(TEXT("FNetPacketNotify::WriteHeader - Seq %u, AckedSeq %u bReFresh %u HistorySizeInWords %u"), Seq, AckedSeq, bRefresh ? 1u : 0u, WrittenHistoryWordCount);

	return true;
}

bool FNetPacketNotify::ReadHeader(FNotificationHeader& Data, FBitReader& Reader) const
{
	// Read packed header
	uint32 PackedHeader = 0;	
	Reader << PackedHeader;

	// unpack
	Data.Seq = FPackedHeader::GetSeq(PackedHeader);
	Data.AckedSeq = FPackedHeader::GetAckedSeq(PackedHeader);
	Data.HistoryWordCount = FPackedHeader::GetHistoryWordCount(PackedHeader) + 1;

	// Read ack history
	Data.History.Read(Reader, Data.HistoryWordCount);

	UE_LOG_PACKET_NOTIFY(TEXT("FNetPacketNotify::ReadHeader - Seq %u, AckedSeq %u HistorySizeInWords %u"), Data.Seq.Get(), Data.AckedSeq.Get(), Data.HistoryWordCount);

	return Reader.IsError() == false;
}
```


### 2. 握手流程（连接层握手）

### 3. 握手流程（应用层握手）

### 4. UNetConnection 创建管理的
- 什么时机创建
- 创建后保存位置



## 参考

- [《Exploring in UE4》网络同步原理深入（下）](https://blog.uwa4d.com/archives/USparkle_Exploring1.html)
- [UnrealEngine - 网络同步之连接篇](https://www.cnblogs.com/lawliet12/p/17332897.html)
- [UE4 网络模块剖析](https://www.zhyingkun.com/ue4network/ue4network/)
- [UDP可靠性详解](https://github.com/qqwx1986/ue4_doc/blob/master/UDP%E5%8F%AF%E9%9D%A0%E6%80%A7%E8%AF%A6%E8%A7%A3.md)
- [UE4 网络同步框架源码解析](https://zhuanlan.zhihu.com/p/640723352)
- [《Exploring in UE4》关于网络同步的理解与思考[概念理解]](https://www.gameres.com/806296.html)
- [UE4 UDP是如何进行可靠传输的](https://zhuanlan.zhihu.com/p/372375535)
- [[UE4]一分钟实现聊天系统](https://www.cnblogs.com/timy/p/9992030.html)
