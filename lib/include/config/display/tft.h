/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * This configuration file should be included if you want to use any of the TFT drivers and the
 * associated graphics library.
 */

// tft depends on gpio, fsmc, timing, dma, stream, memblock, string, font

#include "config/gpio.h"

#if defined(STM32PLUS_F1_HD) || defined(STM32F40_41xxx)
  #include "config/fsmc.h"
#endif

#include "config/timing.h"
#include "config/dma.h"
#include "config/stream.h"
#include "config/string.h"
#include "config/display/font.h"
#include "util/DoublePrecision.h"
#include "memory/Memblock.h"

// includes for the features

#include "display/Point.h"
#include "display/Size.h"
#include "display/Rectangle.h"

// include the sample fonts

#include "display/graphic/fonts/Font_apple_8.h"
#include "display/graphic/fonts/Font_volter_goldfish_9.h"
#include "display/graphic/fonts/Font_kyrou9_bold_8.h"
#include "display/graphic/fonts/Font_kyrou9_regular_8.h"
#include "display/graphic/fonts/Font_atari_st_16.h"
#include "display/graphic/fonts/Font_dos_16.h"
#include "display/graphic/fonts/Font_nintendo_ds_16.h"
#include "display/graphic/fonts/Font_pixelade_13.h"
#include "display/graphic/fonts/Font_proggy_clean_16.h"

#include "display/graphic/gamma/DisplayDeviceGamma.h"
#include "display/graphic/gamma/NullDisplayDeviceGamma.h"

#if defined(STM32PLUS_F1_HD) || defined(STM32F40_41xxx)
  #include "display/graphic/access/Fsmc16BitAccessMode.h"
  #include "display/graphic/access/Fsmc8BitAccessMode.h"
#endif

// graphics library includes

#include "display/graphic/ColourNames.h"
#include "display/graphic/Backlight.h"
#include "display/graphic/GraphicTerminal.h"
#include "display/graphic/PanelConfiguration.h"
#include "display/graphic/PicoJpeg.h"
#include "display/graphic/JpegDecoder.h"
#include "display/graphic/GraphicsLibrary.h"

// include the optimised GPIO drivers in specialisation order

#include "display/graphic/access/Gpio16BitAccessMode.h"

// these two are for devices capable of 72MHz and above

#if defined(STM32PLUS_F1) || defined(STM32PLUS_F4)
  #include "display/graphic/access/Gpio16BitAccessMode_64K_72_50_50.h"      // optimised for 64K colours
  #include "display/graphic/access/Gpio16BitAccessMode_72_50_50.h"          // generic for >64K colours
#endif

// minimum of 24MHz clock required for these

#include "display/graphic/access/Gpio16BitAccessMode_64K_24_80_80.h"      // optimised for 64K colours
#include "display/graphic/access/Gpio16BitAccessMode_24_80_80.h"          // generic for >64K colours

#include "display/graphic/access/Gpio16BitAccessMode_64K_48_42_42.h"      // optimised for 64K colours
#include "display/graphic/access/Gpio16BitAccessMode_48_42_42.h"          // generic for >64K colours

// include the device drivers

#include "display/graphic/tft/ili9325/ILI9325.h"
#include "display/graphic/tft/ili9481/ILI9481.h"
#include "display/graphic/tft/ili9327/ILI9327.h"
#include "display/graphic/tft/ili9327/ILI9327400x240PanelTraits.h"
#include "display/graphic/tft/hx8347a/HX8347A.h"
#include "display/graphic/tft/mc2pa8201/MC2PA8201.h"
#include "display/graphic/tft/lds285/LDS285.h"
#include "display/graphic/tft/ssd1289/SSD1289.h"
#include "display/graphic/tft/st7783/ST7783.h"
#include "display/graphic/tft/lgdp453x/LGDP453x.h"
#include "display/graphic/tft/hx8352a/HX8352A.h"
#include "display/graphic/tft/r61523/R61523.h"
#include "display/graphic/tft/hx8352a/panelTraits/LG_KF700.h"
#include "display/graphic/tft/hx8352a/panelTraits/TM032LDH05.h"

#if !defined(STM32PLUS_F0)        // there's an FSMC dependency in here
  #include "display/graphic/tft/ssd1963/SSD1963.h"
  #include "display/graphic/tft/ssd1963/panelTraits/SSD1963_480x272PanelTraits.h"
#endif

#include "display/graphic/tft/TftInterfaces.h"

// include the interactive gamma class

#include "display/graphic/gamma/InteractiveGamma.h"
