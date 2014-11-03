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
   * @brief Timing class for NAND flash devices
   */

  struct FsmcNandTiming : FSMC_NAND_PCCARDTimingInitTypeDef {

    FsmcNandTiming(
        uint32_t setupTime,
        uint32_t waitSetupTime,
        uint32_t holdSetupTime,
        uint32_t hizSetupTime) {

      // setup the parameters

      FSMC_SetupTime=setupTime;
      FSMC_WaitSetupTime=waitSetupTime;
      FSMC_HoldSetupTime=holdSetupTime;
      FSMC_HiZSetupTime=hizSetupTime;
    }
  };
}
