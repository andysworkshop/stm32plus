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
	.incbin "lzg/ili9481/bulb.ili9481.64.lzg"
	BulbPixelsSize=.-BulbPixels

AudioPixels:
	.incbin "lzg/ili9481/audio.ili9481.64.lzg"
	AudioPixelsSize=.-AudioPixels

FlagPixels:
	.incbin "lzg/ili9481/flag.ili9481.64.lzg"
	FlagPixelsSize=.-FlagPixels

DocPixels:
	.incbin "lzg/ili9481/doc.ili9481.64.lzg"
	DocPixelsSize=.-DocPixels

GlobePixels:
	.incbin "lzg/ili9481/globe.ili9481.64.lzg"
	GlobePixelsSize=.-GlobePixels
