/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


	.global BulbPixels
	.global BulbPixelsSize

	.global AudioPixels
	.global AudioPixelsSize

	.global FlagPixels
	.global FlagPixelsSize

	.global DocPixels
	.global DocPixelsSize

	.global GlobePixels
	.global GlobePixelsSize

BulbPixels:
	.incbin "lzg/r61523/bulb.r61523.16.lzg"
	BulbPixelsSize=.-BulbPixels

AudioPixels:
	.incbin "lzg/r61523/audio.r61523.16.lzg"
	AudioPixelsSize=.-AudioPixels

FlagPixels:
	.incbin "lzg/r61523/flag.r61523.16.lzg"
	FlagPixelsSize=.-FlagPixels

DocPixels:
	.incbin "lzg/r61523/doc.r61523.16.lzg"
	DocPixelsSize=.-DocPixels

GlobePixels:
	.incbin "lzg/r61523/globe.r61523.16.lzg"
	GlobePixelsSize=.-GlobePixels
