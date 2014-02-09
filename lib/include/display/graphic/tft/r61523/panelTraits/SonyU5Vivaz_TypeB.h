/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {


    /**
     * Panel traits for a Type B Sony U5 Vivaz
     *
     * This is the "clone" panel obtained on ebay that does not have manufacturer
     * specific values held in NVRAM. The R61523 registers all have the default values
     * and therefore we need to make changes so that the registers reflect the
     * traits for the panel.
     */

    struct SonyU5Vivaz_TypeB {
      template<class TAccessMode>
      static void initialise(TAccessMode& accessMode) {

        // this panel needs SM=1 (first parameter). the others are the same
        // as the original Sony. if SM is not set to 1 the image will appear
        // duplicated on the left and right of the display.

        accessMode.writeCommand(r61523::PANEL_DRIVING_SETTING);
        accessMode.writeData(0x8);
        accessMode.writeData(0x9f);
        accessMode.writeData(0);
        accessMode.writeData(0);
        accessMode.writeData(2);
        accessMode.writeData(0);
        accessMode.writeData(1);

        // this panel needs VC2/VC3 set to 0x5 (second parameter). This is the
        // optimum setting that reduces flicker. the other values are the same
        // as the original Sony.

        accessMode.writeCommand(r61523::POWER_SETTING_COMMON);
        accessMode.writeData(0);
        accessMode.writeData(0x55);
        accessMode.writeData(0xc0);
        accessMode.writeData(0x8f);
      }
    };
  }
}
