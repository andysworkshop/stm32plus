/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {

    /*
     * Gamma values for the R61523
     */

    struct R61523Gamma : public DisplayDeviceGamma<R61523Gamma,uint8_t> {

      /**
       * Constructor
       * @param levels
       */

      R61523Gamma(uint8_t *levels)
       : DisplayDeviceGamma(13) {
        memcpy(&_gamma[0],levels,13);
      }


      /**
       * Get the maximum value for an index
       * @return the index maximum
       */

      static uint8_t getMaximumValue(uint16_t index) {
        return index==0 || index==1 || index==11 || index==12 ? 31 : 15;
      }
    };
  }
}
