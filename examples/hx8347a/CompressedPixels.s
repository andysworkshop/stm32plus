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
  .incbin "lzg/hx8347a/bulb.hx8347a.64.lzg"
  BulbPixelsSize=.-BulbPixels

AudioPixels:
  .incbin "lzg/hx8347a/audio.hx8347a.64.lzg"
  AudioPixelsSize=.-AudioPixels

FlagPixels:
  .incbin "lzg/hx8347a/flag.hx8347a.64.lzg"
  FlagPixelsSize=.-FlagPixels

DocPixels:
  .incbin "lzg/hx8347a/doc.hx8347a.64.lzg"
  DocPixelsSize=.-DocPixels

GlobePixels:
  .incbin "lzg/hx8347a/globe.hx8347a.64.lzg"
  GlobePixelsSize=.-GlobePixels
