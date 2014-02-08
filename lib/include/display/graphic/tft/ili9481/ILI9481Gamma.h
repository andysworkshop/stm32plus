/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {

    /**
     * Gamma values for the ILI9481
     *
     * Samples for 3.5 inch QDS LCD: 0,0x30,0x36,0x45,0x4,0x16,0x37,0x75,0x77,0x54,0xf,0x0
     */

    struct ILI9481Gamma : DisplayDeviceGamma<ILI9481Gamma,uint16_t> {

    /**
     * Constructor. The ILI9481 takes 12 gamma values. The constructor must be supplied with
     * those values.
     */

      ILI9481Gamma(
          uint16_t g1_,uint16_t g2_,uint16_t g3_,uint16_t g4_,uint16_t g5_,uint16_t g6_,
          uint16_t g7_,uint16_t g8_,uint16_t g9_,uint16_t g10_,uint16_t g11_,uint16_t g12_)
       : DisplayDeviceGamma(12) {

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
      }


      /**
       * Get the maximum value that a gamma value can have
       * @return the maximum value
       */

      static constexpr uint8_t getMaximumValue(uint16_t /* index */) {
        return 127;
      }
    };
  }
}
