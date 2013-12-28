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

BulbPixels:
	.incbin "lzg/r61523/bulb.r61523.64.lzg"
	BulbPixelsSize=.-BulbPixels

AudioPixels:
	.incbin "lzg/r61523/audio.r61523.64.lzg"
	AudioPixelsSize=.-AudioPixels

FlagPixels:
  .incbin "lzg/r61523/flag.r61523.64.lzg"
  FlagPixelsSize=.-FlagPixels
