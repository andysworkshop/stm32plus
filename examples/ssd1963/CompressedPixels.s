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
  .incbin "lzg/ssd1963/bulb.ssd1963.16.lzg"
  BulbPixelsSize=.-BulbPixels

AudioPixels:
  .incbin "lzg/ssd1963/audio.ssd1963.16.lzg"
  AudioPixelsSize=.-AudioPixels

FlagPixels:
  .incbin "lzg/ssd1963/flag.ssd1963.16.lzg"
  FlagPixelsSize=.-FlagPixels

DocPixels:
  .incbin "lzg/ssd1963/doc.ssd1963.16.lzg"
  DocPixelsSize=.-DocPixels

GlobePixels:
  .incbin "lzg/ssd1963/globe.ssd1963.16.lzg"
  GlobePixelsSize=.-GlobePixels
