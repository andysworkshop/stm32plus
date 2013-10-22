/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace display {
  namespace r61523 {

    /**
     * Possible frequencies for the backlight
     */

    enum R61523BacklightFrequency {
      FREQUENCY_33_3 = 0x00,
      FREQUENCY_27_4 = 0x01,
      FREQUENCY_18_3 = 0x02,
      FREQUENCY_13_7 = 0x03,
      FREQUENCY_6_86 = 0x07,
      FREQUENCY_3_43 = 0x0f,
      FREQUENCY_1_72 = 0x1f,
      FREQUENCY_0_86 = 0x3f,
      FREQUENCY_0_43 = 0x7f,
      FREQUENCY_0_21 = 0xff
    };


    /**
     * The R61523 supports direct control of the PWM LED pin as well as a combination of
     * the calculated required backlight and the PWM register setting. This is the mode
     * where you directly control the PWM output on the BLPWM pin
     * @tparam TFrequency The desired frequency for the PWM signal from the available enumeration
     * @tparam TSmooth true if changes are to be applied gradually over approx 500ms in hardware
     * @tparam TPolarity 0 means signal high is lit, 1 means signal low is lit.
     */

    template<bool TSmooth=true,R61523BacklightFrequency TFrequency=FREQUENCY_13_7,uint8_t TPolarity=0>
    class R61523PwmBacklight {

      public:
        R61523PwmBacklight();

        void fadeTo(uint16_t newPercentage,int msPerStep);
    };


    /**
     * Constructor
     */

    template<bool TSmooth,R61523BacklightFrequency TFrequency,uint8_t TPolarity>
    inline R61523PwmBacklight::R61523PwmBacklight() {
    }


    /**
     * Fade up or down to the supplied percentage waiting for msPerStep millis between each step.
     * If TSmooth is true then this parameter is ignored because the R61523 will go smooth for us.
     * @param newPercentage 0..100
     * @param msPerStep Milliseconds to pause between each step
     */

    template<bool TSmooth,R61523BacklightFrequency TFrequency,uint8_t TPolarity>
    inline void R61523PwmBacklight::fadeTo(uint16_t newPercentage,int msPerStep) {
    }
  }
}


// include the specialisations

#include "R61523PwmBacklight.h"
