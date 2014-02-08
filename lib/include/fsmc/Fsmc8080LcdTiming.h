/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * 8080 LCD timing just needs address and data setup
   */

  class Fsmc8080LcdTiming : public FsmcAddressDataSetupTiming {
    public:

      /**
       * We change the access mode to mode B for the 8080 LCD devices.
       */

    Fsmc8080LcdTiming(uint32_t addressSetup,uint32_t dataSetup)
      : FsmcAddressDataSetupTiming(addressSetup,dataSetup) {
        FSMC_AccessMode=FSMC_AccessMode_B;    // doesn't really matter. mode B toggles NADV for mux'd memories with a latch
      }
  };
}
