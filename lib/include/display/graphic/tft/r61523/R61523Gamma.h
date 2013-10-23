/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
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
       * @param redPositive
       * @param redNegative
       * @param greenPositive
       * @param greenNegative
       * @param bluePositive
       * @param blueNegative
       */

      R61523Gamma(uint8_t *redPositive,
                  uint8_t *redNegative,
                  uint8_t *greenPositive,
                  uint8_t *greenNegative,
                  uint8_t *bluePositive,
                  uint8_t *blueNegative)
       : DisplayDeviceGamma(6*13) {

        memcpy(&_gamma[0*13],redPositive,13);
        memcpy(&_gamma[1*13],redNegative,13);
        memcpy(&_gamma[2*13],greenPositive,13);
        memcpy(&_gamma[3*13],greenNegative,13);
        memcpy(&_gamma[4*13],bluePositive,13);
        memcpy(&_gamma[5*13],blueNegative,13);
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
