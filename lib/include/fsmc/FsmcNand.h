/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @brief FSMC implementation for NAND flash memory
   */

  template<class TFsmc>
  class FsmcNand : public FSMC_NANDInitTypeDef {

    public:
      FsmcNand(const FsmcNandTiming& timing);
      ~FsmcNand();
  };


  /**
   * Constructor. Setup the FSMC ready for use
   */

  template<class TFsmc>
  inline FsmcNand<TFsmc>::FsmcNand(const FsmcNandTiming& timing) {

    // initialise the FSMC and the NE line (different for each bank)

    TFsmc::initialise();

    // port D alternate function initialisation

    GpioPinInitialiser::initialise(
        GPIOD,
        GPIO_Pin_0 |        // D2
        GPIO_Pin_1 |        // D3
        GPIO_Pin_4 |        // nOE
        GPIO_Pin_5 |        // nWE
        GPIO_Pin_7 |        // NE2
        GPIO_Pin_11 |       // CLE
        GPIO_Pin_12 |       // ALE
        GPIO_Pin_14 |       // D0
        GPIO_Pin_15,        // D1
        Gpio::ALTERNATE_FUNCTION,
        (GPIOSpeed_TypeDef)FsmcPeripheralTraits::GPIO_SPEED,
        Gpio::PUPD_NONE,
        Gpio::PUSH_PULL,
        GpioAlternateFunctionMapper<PERIPHERAL_FSMC,GPIOD_BASE,0>::GPIO_AF);

    // port E alternate function initialisation

    GpioPinInitialiser::initialise(
        GPIOE,
        GPIO_Pin_7 |        // D4
        GPIO_Pin_8 |        // D5
        GPIO_Pin_9 |        // D6
        GPIO_Pin_10,        // D7
        Gpio::ALTERNATE_FUNCTION,
        (GPIOSpeed_TypeDef)FsmcPeripheralTraits::GPIO_SPEED,
        Gpio::PUPD_NONE,
        Gpio::PUSH_PULL,
        GpioAlternateFunctionMapper<PERIPHERAL_FSMC,GPIOE_BASE,0>::GPIO_AF);

    // port D nWAIT input pin, pull up (nWAIT is active low)

    GpioPinInitialiser::initialise(
        GPIOD,
        GPIO_Pin_6,         // nWait
        Gpio::INPUT,
        (GPIOSpeed_TypeDef)FsmcPeripheralTraits::GPIO_SPEED,
        Gpio::PUPD_UP);

    // port G INT2 input pin, pull up (nWAIT is active low)
    // not available on LQFP100

    GpioPinInitialiser::initialise(
        GPIOG,
        GPIO_Pin_6,         // INT2
        Gpio::INPUT,
        (GPIOSpeed_TypeDef)FsmcPeripheralTraits::GPIO_SPEED,
        Gpio::PUPD_UP);

    // initialise how the FSMC will work

    FSMC_Bank=TFsmc::getBank();
    FSMC_Waitfeature=FSMC_Waitfeature_Enable;
    FSMC_MemoryDataWidth=FSMC_MemoryDataWidth_8b;
    FSMC_ECC=FSMC_ECC_Enable;
    FSMC_ECCPageSize=FSMC_ECCPageSize_512Bytes;
    FSMC_TCLRSetupTime=0x00;
    FSMC_TARSetupTime=0x00;
    FSMC_CommonSpaceTimingStruct=const_cast<FsmcNandTiming *>(&timing);
    FSMC_AttributeSpaceTimingStruct=const_cast<FsmcNandTiming *>(&timing);

    FSMC_NANDInit(this);

    // enable it

    FSMC_NANDCmd(TFsmc::getBank(),ENABLE);
  }


  /**
   * Destructor. De-init the FSMC.
   */

  template<class TFsmc>
  inline FsmcNand<TFsmc>::~FsmcNand() {

    FSMC_NANDCmd(TFsmc::getBank(),DISABLE);
    FSMC_NANDDeInit(TFsmc::getBank());
  }
}
