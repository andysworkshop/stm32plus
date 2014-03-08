/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


/*
 * Construct a type that will give us multi-adc using channels 0,1 and operating
 * in simultaneous mode.
 */

typedef Adc1<
    AdcClockPrescalerFeature<2>,                    // prescaler of 2
    AdcResolutionFeature<8>,                        // 8 bit resolution
    Adc1Cycle144RegularChannelFeature<0>,           // using channel 0 on ADC1 with 144-cycle latency
    AdcContinuousModeFeature,                       // continuous mode
    DualAdcRegularSimultaneousDmaMode1Feature<      // regular simultaneous multi mode
      Adc2<                                         // the second ADC
        AdcClockPrescalerFeature<2>,                // prescaler of 2
        AdcResolutionFeature<8>,                    // 8 bit resolution
        Adc2Cycle144RegularChannelFeature<1>,       // using channel 1 on ADC2 with 144-cycle latency
        AdcScanModeFeature<>                        // scan mode with EOC after each group
      >,
      5                                             // 5 cycle min delay
    >
  > AdcContinuous;
