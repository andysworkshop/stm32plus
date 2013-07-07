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
	.incbin "lzg/ili9327/bulb.ili9327.262.lzg"
	BulbPixelsSize=.-BulbPixels

AudioPixels:
	.incbin "lzg/ili9327/audio.ili9327.262.lzg"
	AudioPixelsSize=.-AudioPixels

FlagPixels:
	.incbin "lzg/ili9327/flag.ili9327.262.lzg"
	FlagPixelsSize=.-FlagPixels

DocPixels:
	.incbin "lzg/ili9327/doc.ili9327.262.lzg"
	DocPixelsSize=.-DocPixels

GlobePixels:
	.incbin "lzg/ili9327/globe.ili9327.262.lzg"
	GlobePixelsSize=.-GlobePixels
