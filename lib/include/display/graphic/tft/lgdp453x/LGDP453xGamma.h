/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include "display/graphic/gamma/DisplayDeviceGamma.h"

namespace stm32plus {
  namespace display {

    /**
     * @brief Gamma values for the LGDP453x
     *
     * Sample for STM32 dev board display:
     * 0x7,0x302,0x105,0x206,0x808,0x206,0x504,0x7,0x105,0x808
     */

    struct LGDP453xGamma : DisplayDeviceGamma<LGDP453xGamma,uint16_t> {

    /**
     * Constructor. The LGDP453x takes 10 gamma values. The constructor must be supplied with
     * those values.
     */

      LGDP453xGamma(
          uint16_t g1_,uint16_t g2_,uint16_t g3_,uint16_t g4_,uint16_t g5_,
          uint16_t g6_,uint16_t g7_,uint16_t g8_,uint16_t g9_,uint16_t g10_) : DisplayDeviceGamma(10) {

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
      }


      /**
       * Get the maximum value that a gamma value can have
       * @return the maximum value
       */

      static constexpr uint16_t getMaximumValue(uint16_t /* index */) {
        return 4095;
      }
    };
  }
}
