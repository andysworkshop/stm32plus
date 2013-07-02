/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * This configuration file should be included if you want to use any of the TFT drivers and the
 * associated graphics library.
 */

// tft depends on gpio, fsmc, timing, dma, stream, minmax, memblock, string, font

#include "config/gpio.h"
#include "config/fsmc.h"
#include "config/timing.h"
#include "config/dma.h"
#include "config/stream.h"
#include "config/string.h"
#include "config/display/font.h"
#include "util/MinMax.h"
#include "memory/Memblock.h"

// includes for the features

#include "display/Point.h"
#include "display/Size.h"
#include "display/Rectangle.h"
#include "display/DoublePrecision.h"

// include the sample fonts

#include "display/graphic/Font_apple_8.h"
#include "display/graphic/Font_volter_goldfish_9.h"
#include "display/graphic/Font_kyrou9_bold_8.h"
#include "display/graphic/Font_kyrou9_regular_8.h"

#include "display/graphic/gamma/DisplayDeviceGamma.h"
#include "display/graphic/gamma/NullDisplayDeviceGamma.h"

#include "display/graphic/ColourNames.h"
#include "display/graphic/Backlight.h"
#include "display/graphic/Fsmc16BitAccessMode.h"
#include "display/graphic/FSMC8BitAccessMode.h"
#include "display/graphic/GraphicTerminal.h"
#include "display/graphic/PanelConfiguration.h"
#include "display/graphic/PicoJpeg.h"
#include "display/graphic/JpegDecoder.h"
#include "display/graphic/GraphicsLibrary.h"

#include "display/graphic/tft/ili9325/ILI9325.h"
#include "display/graphic/tft/ili9481/ILI9481.h"
#include "display/graphic/tft/ili9327/ILI9327.h"
#include "display/graphic/tft/ili9327/ILI9327400x240PanelTraits.h"
#include "display/graphic/tft/hx8347a/HX8347A.h"
#include "display/graphic/tft/mc2pa8201/MC2PA8201.h"
#include "display/graphic/tft/lds285/LDS285.h"
#include "display/graphic/tft/ssd1963/SSD1963.h"
#include "display/graphic/tft/ssd1963/panelTraits/SSD1963_480x272PanelTraits.h"
#include "display/graphic/tft/ssd1289/SSD1289.h"
#include "display/graphic/tft/st7783/ST7783.h"
#include "display/graphic/tft/lgdp453x/LGDP453x.h"

#include "display/graphic/tft/TftInterfaces.h"
