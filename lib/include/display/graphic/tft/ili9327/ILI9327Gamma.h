/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {

    /**
     * @brief Gamma values for the ILI9327
     */

    struct ILI9327Gamma : DisplayDeviceGamma<ILI9327Gamma,uint16_t> {

    /**
     * Constructor. The ILI9327 takes 15 gamma values. The constructor must be supplied with
     * those values.
     */

      ILI9327Gamma(
          uint16_t g1_,uint16_t g2_,uint16_t g3_,uint16_t g4_,uint16_t g5_,uint16_t g6_,uint16_t g7_,uint16_t g8_,
          uint16_t g9_,uint16_t g10_,uint16_t g11_,uint16_t g12_,uint16_t g13_,uint16_t g14_,uint16_t g15_)
            : DisplayDeviceGamma(15) {

        _gamma[0]=g1_;
        _gamma[1]=g2_;
        _gamma[2]=g3_;
        _gamma[3]=g4_;
        _gamma[4]=g5_;
        _gamma[5]=g6_;
        _gamma[6]=g7_;
        _gamma[7]=g8_;
        _gamma[8]=g9_;
        _gamma[9]=g10_;
        _gamma[10]=g11_;
        _gamma[11]=g12_;
        _gamma[12]=g13_;
        _gamma[13]=g14_;
        _gamma[14]=g15_;
      }


      /**
       * Get the maximum value that a gamma value can have
       * @return the maximum value
       */

      static constexpr uint8_t getMaximumValue(uint16_t /* index */) {
        return 255;
      }
    };
  }
}
