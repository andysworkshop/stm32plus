/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


#include "display/graphic/GraphicsLibrary.h"
#include "display/graphic/Serial4WireSpiAccessMode.h"
#include "display/graphic/oled/ssd1306/SSD1306.h"


/*
 * Useful typedefs to ease the pain of typing out template parameters
 */

namespace stm32plus {
  namespace display {

    /*
     * SSD1306 interface landscape only
     */

    typedef GraphicsLibrary<SSD1306<LANDSCAPE,Serial4WireSpiAccessMode>,Serial4WireSpiAccessMode> SSD1306_Landscape_Serial4;
  }
}
