/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace display {
    namespace ssd1289 {

      /*
       * The command set that we need
       */

      enum E {
        OSCILLATOR = 0x00,
        DRIVER_OUTPUT_CONTROL = 0x01,
        DRIVING_WAVEFORM_CONTROL = 0x02,
        DISPLAY_CONTROL = 0x07,
        GATE_SCAN_POSITION = 0xf,
        SLEEP_MODE = 0x10,
        ENTRY_MODE = 0x11,
        GRAM_WRITE_DATA = 0x22,
        GAMMA_0 = 0x30,
        GAMMA_1 = 0x31,
        GAMMA_2 = 0x32,
        GAMMA_3 = 0x33,
        GAMMA_4 = 0x34,
        GAMMA_5 = 0x35,
        GAMMA_6 = 0x36,
        GAMMA_7 = 0x37,
        GAMMA_8 = 0x3A,
        GAMMA_9 = 0x3B,
        HORIZONTAL_POSITION = 0x44,
        VERTICAL_POSITION_START = 0x45,
        VERTICAL_POSITION_END = 0x46,
        SET_GDDRAM_X = 0x4e,
        SET_GDDRAM_Y = 0x4f
      };
    }
  }
}
