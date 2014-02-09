/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace display {
    namespace hx8352a {

      /*
       * The command set as documented in "version 05 December 2008" of the datasheet
       * Only the commands we use are defined
       */

      enum E {
        DISPLAY_MODE_CONTROL                = 0x1,
        COLUMN_ADDRESS_START_H              = 0x2,
        COLUMN_ADDRESS_START_L              = 0x3,
        COLUMN_ADDRESS_END_H                = 0x4,
        COLUMN_ADDRESS_END_L                = 0x5,
        ROW_ADDRESS_START_H                 = 0x6,
        ROW_ADDRESS_START_L                 = 0x7,
        ROW_ADDRESS_END_H                   = 0x8,
        ROW_ADDRESS_END_L                   = 0x9,
        VERTICAL_SCROLL_TOP_FIXED_AREA_H    = 0xE,
        VERTICAL_SCROLL_TOP_FIXED_AREA_L    = 0xF,
        VERTICAL_SCROLL_HEIGHT_H            = 0x10,
        VERTICAL_SCROLL_HEIGHT_L            = 0x11,
        VERTICAL_SCROLL_BOTTOM_FIXED_AREA_H = 0x12,
        VERTICAL_SCROLL_BOTTOM_FIXED_AREA_L = 0x13,
        VERTICAL_SCROLL_START_ADDRESS_H     = 0x14,
        VERTICAL_SCROLL_START_ADDRESS_L     = 0x15,
        MEMORY_ACCESS_CONTROL               = 0x16,
        OSC_CONTROL_1                       = 0x17,
        POWER_CONTROL_1                     = 0x19,
        POWER_CONTROL_2                     = 0x1A,
        POWER_CONTROL_3                     = 0x1B,
        POWER_CONTROL_4                     = 0x1C,
        POWER_CONTROL_5                     = 0x1D,
        POWER_CONTROL_6                     = 0x1E,
        VCOM_CONTROL                        = 0x1F,
        DISPLAY_CONTROL_1                   = 0x23,
        DISPLAY_CONTROL_2                   = 0x24,
        MEMORY_WRITE                        = 0x22,
        CYCLE_CONTROL_1                     = 0x2B,
        CYCLE_CONTROL_10                    = 0x34,
        CYCLE_CONTROL_11                    = 0x35,
        SOURCE_CONTROL_1                    = 0x3C,
        SOURCE_CONTROL_2                    = 0x3D,
        GAMMA_1                             = 0x3E,
        PANEL_CONTROL                       = 0x55,
        IP_CONTROL                          = 0x5A,
        TEST_MODE_CONTROL                   = 0x83,
        VDDD_CONTROL                        = 0x85,
        SOURCE_GAMMA_RESISTOR_1             = 0x8B,
        SOURCE_GAMMA_RESISTOR_2             = 0x8C,
        SYNC_FUNCTION                       = 0x91
      };
    }
  }
}
