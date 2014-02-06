/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/adc.h"


using namespace stm32plus;


/**
 *
 * Compatible MCU:
 *   STM32F1
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F100RBT6
 *   STM32F103ZET6
 *   STM32F407VGT6
 *   STM32F107VCT6
 */

class AdcSingle {

  public:

    void run() {

      Adc1<
        AdcClockPrescalerFeature<2>,
        AdcResolutionFeature<12>,
        Adc3CycleRegularChannelFeature<1>
        > adc;
    }
};


/*
 * Main entry point
 */

int main() {

  AdcSingle adc;
  adc.run();

  // not reached
  return 0;
}
