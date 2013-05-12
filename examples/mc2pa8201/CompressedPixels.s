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
	.incbin "lzg/mc2pa8201/bulb.mc2pa8201.16M.lzg"
	BulbPixelsSize=.-BulbPixels

AudioPixels:
	.incbin "lzg/mc2pa8201/audio.mc2pa8201.16M.lzg"
	AudioPixelsSize=.-AudioPixels

FlagPixels:
	.incbin "lzg/mc2pa8201/flag.mc2pa8201.16M.lzg"
	FlagPixelsSize=.-FlagPixels

DocPixels:
	.incbin "lzg/mc2pa8201/doc.mc2pa8201.16M.lzg"
	DocPixelsSize=.-DocPixels

GlobePixels:
	.incbin "lzg/mc2pa8201/globe.mc2pa8201.16M.lzg"
	GlobePixelsSize=.-GlobePixels
