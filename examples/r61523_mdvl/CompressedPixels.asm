/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


	.global BulbPixels
	.global BulbPixelsSize

	.global AudioPixels
	.global AudioPixelsSize

  .global DocPixels
  .global DocPixelsSize

  .global FlagPixels
  .global FlagPixelsSize

  .global GlobePixels
  .global GlobePixelsSize

BulbPixels:
	.incbin "lzg/r61523/bulb.r61523.64.lzg"
	BulbPixelsSize=.-BulbPixels

AudioPixels:
	.incbin "lzg/r61523/audio.r61523.64.lzg"
	AudioPixelsSize=.-AudioPixels

DocPixels:
  .incbin "lzg/r61523/doc.r61523.64.lzg"
  DocPixelsSize=.-DocPixels

GlobePixels:
  .incbin "lzg/r61523/globe.r61523.64.lzg"
  GlobePixelsSize=.-GlobePixels

FlagPixels:
  .incbin "lzg/r61523/flag.r61523.64.lzg"
  FlagPixelsSize=.-FlagPixels
