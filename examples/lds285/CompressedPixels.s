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
  .incbin "lzg/lds285/bulb.lds285.16M.lzg"
  BulbPixelsSize=.-BulbPixels

AudioPixels:
  .incbin "lzg/lds285/audio.lds285.16M.lzg"
  AudioPixelsSize=.-AudioPixels

FlagPixels:
  .incbin "lzg/lds285/flag.lds285.16M.lzg"
  FlagPixelsSize=.-FlagPixels

DocPixels:
  .incbin "lzg/lds285/doc.lds285.16M.lzg"
  DocPixelsSize=.-DocPixels

GlobePixels:
  .incbin "lzg/lds285/globe.lds285.16M.lzg"
  GlobePixelsSize=.-GlobePixels
