/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @brief FSMC implementation for SRAM memory.
   *
   * The memory is assumed to be connected in the manner documented by ST in AN2784. The STM32
   * package is assumed to be 144 pin and the memory transfers are 16-bit.
   */

  template<class TFsmc>
  class FsmcSram : public FSMC_NORSRAMInitTypeDef {

    public:
      FsmcSram(const FsmcSramTiming& timing,const uint32_t lastAddressLineIndex);
      ~FsmcSram();
  };


  /**
   * Constructor. Initialises the AFIO GPIO pins for the FSMC.
   * @param[in] bank The FSMC bank e.g. FSMC_Bank1_NORSRAM1
   * @param[in] timing The timing structure for this SRAM memory.
   * @param[in] lastAddressLineIndex The last address line needed to fully address this memory. e.g. 18 = A18 = (2**19) 16-bit words
   */

  template<class TFsmc>
  inline FsmcSram<TFsmc>::FsmcSram(const FsmcSramTiming& timing,uint32_t lastAddressLineIndex) {

    extern const uint16_t Sram_pgmap[16],Sram_pfmap[10],Sram_pemap[7],Sram_pdmap[3];

    // initialise the FSMC and the NE line (different for each bank)
    // the FSMC peripheral is present on the F1HD and F4 but not the F3
    // therefore the AF ID is the same for all pins

    TFsmc::initialise();

    // A10 is the first line to appear on port G

    if(lastAddressLineIndex>=10)
      GpioPinInitialiser::initialise(
          GPIOG,
          Sram_pgmap[lastAddressLineIndex-10],
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)FsmcPeripheralTraits::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_FSMC,GPIOG_BASE,0>::GPIO_AF);

    // A0..A9 are on port F

    GpioPinInitialiser::initialise(
        GPIOF,
        Sram_pfmap[lastAddressLineIndex>9 ? 9 : lastAddressLineIndex],
        Gpio::ALTERNATE_FUNCTION,
        (GPIOSpeed_TypeDef)FsmcPeripheralTraits::GPIO_SPEED,
        Gpio::PUPD_NONE,
        Gpio::PUSH_PULL,
        GpioAlternateFunctionMapper<PERIPHERAL_FSMC,GPIOF_BASE,0>::GPIO_AF);

    // A19..A23 are on port E

    if(lastAddressLineIndex>=19)
      GpioPinInitialiser::initialise(
          GPIOE,
          Sram_pemap[lastAddressLineIndex-19],
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)FsmcPeripheralTraits::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_FSMC,GPIOE_BASE,0>::GPIO_AF);

    // A16..A18 are on port D

    if(lastAddressLineIndex>=16)
      GpioPinInitialiser::initialise(
          GPIOD,
          Sram_pdmap[(lastAddressLineIndex>18 ? 18 : lastAddressLineIndex)-16],
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)FsmcPeripheralTraits::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_FSMC,GPIOD_BASE,0>::GPIO_AF);

    // data and control lines

    GpioPinInitialiser::initialise(
        GPIOD,
        GPIO_Pin_0 |    // D2
        GPIO_Pin_1 |    // D3
        GPIO_Pin_4 |    // NOE
        GPIO_Pin_5 |    // NWE
        GPIO_Pin_8 |    // D13
        GPIO_Pin_9 |    // D14
        GPIO_Pin_10 |   // D15
        GPIO_Pin_14 |   // D0
        GPIO_Pin_15,    // D1
        Gpio::ALTERNATE_FUNCTION,
        (GPIOSpeed_TypeDef)FsmcPeripheralTraits::GPIO_SPEED,
        Gpio::PUPD_NONE,
        Gpio::PUSH_PULL,
        GpioAlternateFunctionMapper<PERIPHERAL_FSMC,GPIOD_BASE,0>::GPIO_AF);

    GpioPinInitialiser::initialise(
        GPIOE,
        GPIO_Pin_0 |    // NBL0
        GPIO_Pin_1 |    // NBL1
        GPIO_Pin_7 |    // D4
        GPIO_Pin_8 |    // D5
        GPIO_Pin_9 |    // D6
        GPIO_Pin_10 |   // D7
        GPIO_Pin_11 |   // D8
        GPIO_Pin_12 |   // D9
        GPIO_Pin_13 |   // D10
        GPIO_Pin_14 |   // D11
        GPIO_Pin_15,    // D12
        Gpio::ALTERNATE_FUNCTION,
        (GPIOSpeed_TypeDef)FsmcPeripheralTraits::GPIO_SPEED,
        Gpio::PUPD_NONE,
        Gpio::PUSH_PULL,
        GpioAlternateFunctionMapper<PERIPHERAL_FSMC,GPIOE_BASE,0>::GPIO_AF);

    // initialise how the FSMC will work and then enable it

    FSMC_Bank=TFsmc::getBank();
    FSMC_DataAddressMux=FSMC_DataAddressMux_Disable;
    FSMC_MemoryType=FSMC_MemoryType_SRAM;
    FSMC_MemoryDataWidth=FSMC_MemoryDataWidth_16b;
    FSMC_BurstAccessMode=FSMC_BurstAccessMode_Disable;
    FSMC_WaitSignalPolarity=FSMC_WaitSignalPolarity_Low;
    FSMC_WrapMode=FSMC_WrapMode_Disable;
    FSMC_WaitSignalActive=FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_WriteOperation=FSMC_WriteOperation_Enable;
    FSMC_WaitSignal=FSMC_WaitSignal_Disable;
    FSMC_ExtendedMode=FSMC_ExtendedMode_Disable;
    FSMC_WriteBurst=FSMC_WriteBurst_Disable;
    FSMC_ReadWriteTimingStruct=const_cast<FsmcSramTiming *> (&timing);
    FSMC_WriteTimingStruct=const_cast<FsmcSramTiming *> (&timing);
    FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable;

    // initialise the FSMC and then enable it

    FSMC_NORSRAMInit(this);
    FSMC_NORSRAMCmd(TFsmc::getBank(),ENABLE);
  }

  /**
   * Destructor. De-init the FSMC.
   */

  template<class TFsmc>
  inline FsmcSram<TFsmc>::~FsmcSram() {
    FSMC_NORSRAMCmd(TFsmc::getBank(),DISABLE);
    FSMC_NORSRAMDeInit(TFsmc::getBank());
  }
}
