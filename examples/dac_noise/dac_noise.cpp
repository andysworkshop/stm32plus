/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/dac.h"


using namespace stm32plus;


/**
 * Demonstration of DAC channel 1 producing a pseudo-
 * random noise sequence. The output can be seen on PA4.
 * Attach an oscilloscope to see the noise wave or take
 * a look at waveform_f1.png in the example subdirectory
 * to see how it looks on the STM32F103ZET6.
 *
 * Compatible MCU:
 *   STM32F1
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F407VGT6
 *   STM32F103ZET6
 */

class DacNoiseTest {

  public:

    void run() {

      /*
       * Set up the DAC with a software trigger and noise generation. No alignment feature
       * is necessary because we'll never be writing data - it's generated for us courtesy
       * of the mask setting in the parameters class.
       */

      Dac1<>::Parameters params;
      params.dac_trigger=DAC_Trigger_Software;
      params.dac_waveGeneration=DAC_WaveGeneration_Noise;

      Dac1<> dac(params);

      // continually trigger the noise conversion as fast as possible

      for(;;)
        dac.triggerOn();
    }
};


/*
 * Main entry point
 */

int main() {

  DacNoiseTest test;
  test.run();

  // not reached
  return 0;
}
