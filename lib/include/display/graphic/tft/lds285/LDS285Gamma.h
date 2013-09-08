/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {

    /**
     * @brief Gamma values for the LDS285
     *
     * This device support 4 fixed gamma curves, so we just need a word
     * to hold the index of the current curve
     */

    class LDS285Gamma : public DisplayDeviceGamma {

      public:

      /**
       * Constructor. The LDS285 takes 1 gamma value index. The constructor must be supplied with that value
       * that can range between 0..3
       */

        LDS285Gamma(uint16_t gammaIndex) : DisplayDeviceGamma(1) {
          _gamma[0]=gammaIndex;
        }
    };

  }
}
