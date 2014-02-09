/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace display {
    namespace ssd1963 {

      /*
       * The command set as documented in v0.99 of the datasheet
       */

      enum E {
        NOP                               = 0,
        SOFTWARE_RESET                    = 1,
        SLEEP_IN                          = 0x10,
        SLEEP_OUT                         = 0x11,
        PARTIAL_MODE_ON                   = 0x12,
        NORMAL_DISPLAY_MODE_ON            = 0x13,
        DISPLAY_INVERSION_OFF             = 0x20,
        DISPLAY_INVERSION_ON              = 0x21,
        GAMMA_SET                         = 0x26,
        DISPLAY_OFF                       = 0x28,
        DISPLAY_ON                        = 0x29,
        COLUMN_ADDRESS_SET                = 0x2a,
        PAGE_ADDRESS_SET                  = 0x2b,
        MEMORY_WRITE                      = 0x2c,
        COLOUR_SET                        = 0x2d,
        MEMORY_READ                       = 0x2e,
        PARTIAL_AREA                      = 0x30,
        VERTICAL_SCROLLING_DEFINITION     = 0x33,
        TEARING_EFFECT_LINE_OFF           = 0x34,
        TEARING_EFFECT_LINE_ON            = 0x35,
        SET_ADDRESS_MODE                  = 0x36,
        VERTICAL_SCROLLING_START_ADDRESS  = 0x37,
        IDLE_MODE_OFF                     = 0x38,
        IDLE_MODE_ON                      = 0x39,
        SET_LCD_MODE                      = 0xb0,
        SET_HORIZONTAL_PERIOD             = 0xb4,
        SET_VERTICAL_PERIOD               = 0xb6,
        SET_PLL                           = 0xe0,
        SET_PLL_MN                        = 0xe2,
        SET_LSHIFT_FREQUENCY              = 0xe6,
        SET_PIXEL_DATA_INTERFACE          = 0xf0
      };
    }
  }
}
