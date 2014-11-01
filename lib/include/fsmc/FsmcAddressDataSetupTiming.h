/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#if !defined(STM32PLUS_F1_HD) && !defined(STM32PLUS_F4)
#error "Unsupported MCU"
#elif defined(STM32F427_437xx) || defined(STM32F429_439xx)
#error "This MCU uses the FMC not the FSMC"
#endif


namespace stm32plus {

  /**
   * @brief Timing class for devices that just need address and data setup times
   *
   * Inherits from FSMC_NORSRAMTimingInitTypeDef to allow
   * the user to customise it.
   */

  struct FsmcAddressDataSetupTiming : FSMC_NORSRAMTimingInitTypeDef {

      FsmcAddressDataSetupTiming()
        : FSMC_NORSRAMTimingInitTypeDef() {
      }


      FsmcAddressDataSetupTiming(uint32_t addressSetup,uint32_t dataSetup)
        : FSMC_NORSRAMTimingInitTypeDef() {

        FSMC_AddressSetupTime=addressSetup;
        FSMC_DataSetupTime=dataSetup;
      }
  };
}
