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
	.incbin "lzg/lgdp453x/bulb.lgdp453x.262.lzg"
	BulbPixelsSize=.-BulbPixels

AudioPixels:
	.incbin "lzg/lgdp453x/audio.lgdp453x.262.lzg"
	AudioPixelsSize=.-AudioPixels

FlagPixels:
	.incbin "lzg/lgdp453x/flag.lgdp453x.262.lzg"
	FlagPixelsSize=.-FlagPixels

DocPixels:
	.incbin "lzg/lgdp453x/doc.lgdp453x.262.lzg"
	DocPixelsSize=.-DocPixels

GlobePixels:
	.incbin "lzg/lgdp453x/globe.lgdp453x.262.lzg"
	GlobePixelsSize=.-GlobePixels
