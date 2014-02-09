/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace display {
    namespace lds285 {

      /*
       * The command set as documented the preliminary datasheet
       */

      enum E {
        NOP                               = 0,
        SOFTWARE_RESET                    = 1,
        READ_DISPLAY_ID                   = 4,
        READ_DISPLAY_STATUS               = 9,
        READ_DISPLAY_POWER_MODE           = 0xA,
        READ_DISPLAY_MADCTL               = 0xb,
        READ_DISPLAY_PIXEL_FORMAT         = 0xc,
        READ_DISPLAY_IMAGE_MODE           = 0xd,
        READ_DISPLAY_SIGNAL_MODE          = 0xe,
        READ_DISPLAY_SELF_DIAGNOSTICS     = 0xf,
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
        ROW_ADDRESS_SET                   = 0x2b,
        MEMORY_WRITE                      = 0x2c,
        MEMORY_READ                       = 0x2e,
        PARTIAL_AREA                      = 0x30,
        VERTICAL_SCROLLING_DEFINITION     = 0x33,
        TEARING_EFFECT_LINE_OFF           = 0x34,
        TEARING_EFFECT_LINE_ON            = 0x35,
        MEMORY_ACCESS_CONTROL             = 0x36,
        VERTICAL_SCROLLING_START_ADDRESS  = 0x37,
        IDLE_MODE_OFF                     = 0x38,
        IDLE_MODE_ON                      = 0x39,
        INTERFACE_PIXEL_FORMAT            = 0x3a,
        WRITE_DISPLAY_BRIGHTNESS          = 0x51,
        WRITE_CTRL_DISPLAY                = 0x53,
        WRITE_CONTENT_ADAPTIVE_BRIGHTNESS = 0x55,
        READ_ID1                          = 0xda,
        READ_ID2                          = 0xdb,
        READ_ID3                          = 0xdc
      };
    }
  }
}
