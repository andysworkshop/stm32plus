/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {
    namespace st7783 {

      /*
       * The command set as documented in v0.2 of the datasheet
       */

      enum E {
        DRIVER_OUTPUT_CONTROL             = 0x01,
        LCD_DRIVING_CONTROL               = 0x02,
        ENTRY_MODE                        = 0x03,
        DISPLAY_CONTROL_1                 = 0x07,
        DISPLAY_CONTROL_2                 = 0x08,
        DISPLAY_CONTROL_3                 = 0x09,
        POWER_CONTROL_1                   = 0x10,
        POWER_CONTROL_2                   = 0x11,
        POWER_CONTROL_3                   = 0x12,
        POWER_CONTROL_4                   = 0x13,
        RAM_HORIZONTAL_ADDRESS            = 0x20,
        RAM_VERTICAL_ADDRESS              = 0x21,
        MEMORY_WRITE                      = 0x22,
        VCOMH_CONTROL                     = 0x29,
        GAMMA_CONTROL_1                   = 0x30,
        GAMMA_CONTROL_2                   = 0x31,
        GAMMA_CONTROL_3                   = 0x32,
        GAMMA_CONTROL_4                   = 0x35,
        GAMMA_CONTROL_5                   = 0x36,
        GAMMA_CONTROL_6                   = 0x37,
        GAMMA_CONTROL_7                   = 0x38,
        GAMMA_CONTROL_8                   = 0x39,
        GAMMA_CONTROL_9                   = 0x3C,
        GAMMA_CONTROL_10                  = 0x3D,
        HORIZONTAL_ADDRESS_START          = 0x50,
        HORIZONTAL_ADDRESS_END            = 0x51,
        VERTICAL_ADDRESS_START            = 0x52,
        VERTICAL_ADDRESS_END              = 0x53,
        GATE_SCAN_CONTROL_1               = 0x60,
        GATE_SCAN_CONTROL_2               = 0x61,
        PANEL_INTERFACE_CONTROL_1         = 0x90,

        // useful bits in ENTRY_MODE

        TRI = 0x8000,
        DFM = 0x4000,
        BGR = 0x1000,
        ID1 = 0x20,
        ID0 = 0x10,
        AM  = 0x8
      };
    }
  }
}
