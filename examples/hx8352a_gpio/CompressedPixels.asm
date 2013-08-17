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
	.incbin "lzg/hx8352a/bulb.hx8352a.64.lzg"
	BulbPixelsSize=.-BulbPixels

AudioPixels:
	.incbin "lzg/hx8352a/audio.hx8352a.64.lzg"
	AudioPixelsSize=.-AudioPixels

FlagPixels:
	.incbin "lzg/hx8352a/flag.hx8352a.64.lzg"
	FlagPixelsSize=.-FlagPixels

DocPixels:
	.incbin "lzg/hx8352a/doc.hx8352a.64.lzg"
	DocPixelsSize=.-DocPixels

GlobePixels:
	.incbin "lzg/hx8352a/globe.hx8352a.64.lzg"
	GlobePixelsSize=.-GlobePixels
