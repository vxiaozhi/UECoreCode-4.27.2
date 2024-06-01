// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreTypes.h"
#include "ImageWrapperBase.h"

#if WITH_UNREALJPEG

#if WITH_LIBJPEGTURBO
using tjhandle = void*;
#endif	// WITH_LIBJPEGTURBO

/**
 * Uncompresses JPEG data to raw 24bit RGB image that can be used by Unreal textures.
 *
 * Source code for JPEG decompression from http://code.google.com/p/jpeg-compressor/
 */
class FJpegImageWrapper
	: public FImageWrapperBase
{
public:

	/** Default constructor. */
	FJpegImageWrapper(int32 InNumComponents = 4);

#if WITH_LIBJPEGTURBO
	virtual ~FJpegImageWrapper();
#endif	// WITH_LIBJPEGTURBO

public:

	//~ FImageWrapperBase interface

	virtual bool SetCompressed(const void* InCompressedData, int64 InCompressedSize) override;
	virtual bool SetRaw(const void* InRawData, int64 InRawSize, const int32 InWidth, const int32 InHeight, const ERGBFormat InFormat, const int32 InBitDepth) override;
	virtual void Uncompress(const ERGBFormat InFormat, int32 InBitDepth) override;
	virtual void Compress(int32 Quality) override;

#if WITH_LIBJPEGTURBO
	bool SetCompressedTurbo(const void* InCompressedData, int64 InCompressedSize);
	void CompressTurbo(int32 Quality);
	void UncompressTurbo(const ERGBFormat InFormat, int32 InBitDepth);
#endif	// WITH_LIBJPEGTURBO

private:

	int32 NumComponents;

#if WITH_LIBJPEGTURBO
	tjhandle Compressor;
	tjhandle Decompressor;
#endif	// WITH_LIBJPEGTURBO
};


#endif //WITH_JPEG
