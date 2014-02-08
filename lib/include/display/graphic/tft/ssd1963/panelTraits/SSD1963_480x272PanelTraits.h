/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {


    /**
     * Panel traits for a 480x272 SSD1963 panel with a 10Mhz SSD1963 external oscillator
     */

    struct SSD1963_480x272PanelTraits {
      enum {
        LONG_SIDE = 480,
        SHORT_SIDE = 272,

        // you'll need to get the following data from the TFT panel datasheet

        PLL_M = 29,              // (10 x 30)/3 = 100Mhz (note: 800Mhz < (M+1)*osc > 250Mhz)
        PLL_N = 2,               // also note max PLL = 110Mhz for the 8080 interface

        PIXEL_CLOCK = 94371,     // (1048576 * PCLK-Mhz)/PLL Frequency (PCLK-Mhz about 9MHz)

        H_BACK_PORCH = 43,       // non-display before display period (pclks)
        H_PERIOD = LONG_SIDE,    // display period (pclks)
        H_FRONT_PORCH = 8,       // non-display after display period (pclks)
        H_SYNC_PULSE_WIDTH = 2,  // width of the horizontal sync pulse (pclks)
        H_SYNC_POSITION = 8,     // how many clocks into the line is the sync pulse

        V_BACK_PORCH = 12,       // non-display before display period (pclks)
        V_PERIOD = SHORT_SIDE,   // display period (pclks)
        V_FRONT_PORCH = 4,       // non-display after display period (pclks)
        V_SYNC_PULSE_WIDTH = 10, // width of the horizontal sync pulse (pclks)
        V_SYNC_POSITION = 4      // how many clocks into the line is the sync pulse
      };
    };
  }
}
