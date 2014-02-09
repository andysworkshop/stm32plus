/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {

    /**
     * @brief Gamma values for the HX8352A
     *
     * This device support 4 fixed gamma curves, so we just need a word
     * to hold the index of the current curve
     */

    struct HX8352AGamma : DisplayDeviceGamma<HX8352AGamma,uint16_t> {

    /**
     * Constructor. The HX8352A takes 12 gamma values. The constructor must be supplied with those values.
     * Example: 0xA0,0x03,0x00,0x45,0x03,0x47,0x23,0x77,0x01,0x1F,0x0F,0x03
     */

      HX8352AGamma(
          uint16_t g1,uint16_t g2,uint16_t g3,uint16_t g4,uint16_t g5,uint16_t g6,
          uint16_t g7,uint16_t g8,uint16_t g9,uint16_t g10,uint16_t g11,uint16_t g12)
            : DisplayDeviceGamma(12) {

        _gamma[0]=g1;
        _gamma[1]=g2;
        _gamma[2]=g3;
        _gamma[3]=g4;
        _gamma[4]=g5;
        _gamma[5]=g6;
        _gamma[6]=g7;
        _gamma[7]=g8;
        _gamma[8]=g9;
        _gamma[9]=g10;
        _gamma[10]=g11;
        _gamma[11]=g12;
      }
    };
  }
}
