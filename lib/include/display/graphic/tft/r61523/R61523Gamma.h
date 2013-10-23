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

    class R61523Gamma : public DisplayDeviceGamma<R61523Gamma,uint8_t> {

      public:
        R61523Gamma();

        uint8_t getMaximumValue(uint16_t index) const;
    };


    /**
     * Constructor. The R61523 takes 6 arrays of 13 integers each. See the datasheet
     * description of the gamma correction function for how these work.
     */

    inline R61523Gamma::R61523Gamma(
                uint8_t *redPositive,
                uint8_t *redNegative,
                uint8_t *greenPositive,
                uint8_t *greenNegative,
                uint8_t *bluePositive,
                uint8_t *blueNegative)
     : DisplayDeviceGamma(78) {

      memcpy(&_gamma[0*13],redPositive,13);
      memcpy(&_gamma[1*13],redNegative,13);
      memcpy(&_gamma[2*13],greenPositive,13);
      memcpy(&_gamma[3*13],greenNegative,13);
      memcpy(&_gamma[4*13],bluePositive,13);
      memcpy(&_gamma[5*13],blueNegative,13);
    }


    /**
     * Get the maximum value for an index
     * @return the max value
     */

    inline uint8_t R61523Gamma::getMaximumValue(uint16_t index) const {
      return index==0 || index==1 || index==11 || index==12 ? 31 : 15;
    }
  }
}
