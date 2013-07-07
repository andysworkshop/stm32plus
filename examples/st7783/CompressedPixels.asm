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
	.incbin "lzg/st7783/bulb.st7783.64.lzg"
	BulbPixelsSize=.-BulbPixels

AudioPixels:
	.incbin "lzg/st7783/audio.st7783.64.lzg"
	AudioPixelsSize=.-AudioPixels

FlagPixels:
	.incbin "lzg/st7783/flag.st7783.64.lzg"
	FlagPixelsSize=.-FlagPixels

DocPixels:
	.incbin "lzg/st7783/doc.st7783.64.lzg"
	DocPixelsSize=.-DocPixels

GlobePixels:
	.incbin "lzg/st7783/globe.st7783.64.lzg"
	GlobePixelsSize=.-GlobePixels
