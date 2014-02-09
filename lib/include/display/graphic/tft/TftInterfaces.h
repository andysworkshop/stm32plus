/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include "display/graphic/tft/hx8347a/TftInterfaces.h"
#include "display/graphic/tft/ili9325/TftInterfaces.h"
#include "display/graphic/tft/ili9327/TftInterfaces.h"
#include "display/graphic/tft/ili9481/TftInterfaces.h"
#include "display/graphic/tft/lds285/TftInterfaces.h"
#include "display/graphic/tft/mc2pa8201/TftInterfaces.h"
#include "display/graphic/tft/ssd1289/TftInterfaces.h"
#include "display/graphic/tft/st7783/TftInterfaces.h"
#include "display/graphic/tft/lgdp453x/TftInterfaces.h"
#include "display/graphic/tft/hx8352a/TftInterfaces.h"
#include "display/graphic/tft/r61523/TftInterfaces.h"

#if !defined(STM32PLUS_F0)
  #include "display/graphic/tft/ssd1963/TftInterfaces.h"
#endif
