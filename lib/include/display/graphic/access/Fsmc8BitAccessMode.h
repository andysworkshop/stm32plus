/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {

    /**
     * @brief FSMC implementation for TFT LCD panels controlled by an 8080 interface.
     *
     * The 8080 LCD is assumed to be in 8 bit mode connected to the 8 data pins, RW, RD, NE.
     *
     * The RS (register select) operation is achieved by using an additional address line. e.g. A16.
     * A write to an address that keeps A16 low will active RS (usually active low). A write that sets
     * A16 high will set RS high and achieve a data write to the controller.
     */

    template<class TFsmc>
    class Fsmc8BitAccessMode : public FSMC_NORSRAMInitTypeDef {

      protected:
        volatile uint8_t *_dataAddress;
        volatile uint8_t *_registerAddress;
        GpioPinRef _resetPin;

      protected:
        void initialise(const Fsmc8080LcdTiming& readTiming,const Fsmc8080LcdTiming& writeTiming,uint16_t registerAddressLine);

      public:
        Fsmc8BitAccessMode(const Fsmc8080LcdTiming& timing,uint16_t registerAddressLine,const GpioPinRef& resetPin);
        Fsmc8BitAccessMode(const Fsmc8080LcdTiming& readTiming,const Fsmc8080LcdTiming& writeTiming,uint16_t registerAddressLine,const GpioPinRef& resetPin);

        void enable(bool enable);

        void reset();

        void writeCommand(uint8_t command) const;
        void writeCommand(uint8_t command,uint8_t parameter) const;
        void writeData(uint8_t value) const;
        void writeDataAgain(uint8_t value) const;
        void writeMultiData(uint32_t howMuch,uint8_t value) const;
        uint8_t readData() const;

        volatile uint8_t *getDataAddress() const;
        void rawTransfer(const void *buffer,uint32_t numWords) const;
        static void getDmaTransferSizes(uint32_t& peripheralsize,uint32_t& memsize);
    };


    /**
     * Write a command to the register address
     * @param command The command to write
     */

    template<class TFsmc>
    inline void Fsmc8BitAccessMode<TFsmc>::writeCommand(uint8_t command) const {
      *_registerAddress=command;
    }


    /**
     * Write a command to the panel that takes a parameter
     * @param command The command to write
     * @param parameter The parameter to the command
     */

    template<class TFsmc>
    inline void Fsmc8BitAccessMode<TFsmc>::writeCommand(uint8_t command,uint8_t parameter) const {
      *_registerAddress=command;
      *_dataAddress=parameter;
    }


    /**
     * Write a data value to the panel
     * @param value The data value to write
     */

    template<class TFsmc>
    inline void Fsmc8BitAccessMode<TFsmc>::writeData(uint8_t value) const {
      *_dataAddress=value;
    }


    /**
     * Write multiple data. A possible optimisation point that we can't take advantage of
     * in this access mode
     * @param howMuch How many pixels
     * @param value The pixel
     */

    template<class TFsmc>
    inline void Fsmc8BitAccessMode<TFsmc>::writeMultiData(uint32_t howMuch,uint8_t value) const {
      while(howMuch--)
        writeData(value);
    }


    /**
     * Write same data value again to the panel - no optimisation possible here
     * @param value The data value to write
     */

    template<class TFsmc>
    inline void Fsmc8BitAccessMode<TFsmc>::writeDataAgain(uint8_t value) const {
      *_dataAddress=value;
    }


    /**
     * Read a data value from the panel
     * @return The value read
     */

    template<class TFsmc>
    inline uint8_t Fsmc8BitAccessMode<TFsmc>::readData() const {
      return *_dataAddress;
    }


    /**
     * Get the data address
     * @return The data address
     */

    template<class TFsmc>
    inline volatile uint8_t *Fsmc8BitAccessMode<TFsmc>::getDataAddress() const {
      return _dataAddress;
    }


    /**
     * Constructor. Initialises the AFIO GPIO pins for the FSMC used to control an 8080 LCD.
     * @param[in] timing The timing structure for this LCD panel in both read and write mode
     * @param[in] registerAddressLine The address line index used to switch between data and register select. e.g. 16 = A16.
     * @param[in] resetPin The GPIO pin that corresponds to the TFT reset line.
     */

    template<class TFsmc>
    inline Fsmc8BitAccessMode<TFsmc>::Fsmc8BitAccessMode(const Fsmc8080LcdTiming& timing,
                                                         uint16_t registerAddressLine,
                                                         const GpioPinRef& resetPin) : _resetPin(resetPin) {
      initialise(timing,timing,registerAddressLine);
    }


    /**
     * Constructor. Initialises the AFIO GPIO pins for the FSMC used to control an 8080 LCD.
     * @param[in] readTiming The timing structure for this LCD panel in read mode
     * @param[in] writeTiming The timing structure for this LCD panel in write mode
     * @param[in] registerAddressLine The address line index used to switch between data and register select. e.g. 16 = A16.
     * @param[in] resetPin The GPIO pin that corresponds to the TFT reset line.
     */

    template<class TFsmc>
    inline Fsmc8BitAccessMode<TFsmc>::Fsmc8BitAccessMode(const Fsmc8080LcdTiming& readTiming,
                                                         const Fsmc8080LcdTiming& writeTiming,
                                                         uint16_t registerAddressLine,
                                                         const GpioPinRef& resetPin) : _resetPin(resetPin) {
      initialise(readTiming,writeTiming,registerAddressLine);
    }


    /**
     * Main initialiser. Initialises the AFIO GPIO pins for the FSMC used to control an 8080 LCD.
     * @param[in] readTiming The timing structure for this LCD panel in read mode
     * @param[in] writeTiming The timing structure for this LCD panel in write mode
     * @param[in] registerAddressLine The address line index used to switch between data and register select. e.g. 16 = A16.
     * @param[in] resetPin The GPIO pin that corresponds to the TFT reset line.
     */

    template<class TFsmc>
    inline void Fsmc8BitAccessMode<TFsmc>::initialise(const Fsmc8080LcdTiming& readTiming,
                                                      const Fsmc8080LcdTiming& writeTiming,
                                                      uint16_t registerAddressLine) {

      // initialise the FSMC and the NE line (different for each bank)

      TFsmc::initialise();

      // initialise the addressing, data and control lines

      GpioPinInitialiser::initialise(
          GPIOD,
          GPIO_Pin_0 |    // D2
          GPIO_Pin_1 |    // D3
          GPIO_Pin_4 |    // NOE
          GPIO_Pin_5 |    // NWE
          GPIO_Pin_14 |   // D0
          GPIO_Pin_15,    // D1
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)FsmcPeripheralTraits::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_FSMC,GPIOD_BASE,0>::GPIO_AF);

      GpioPinInitialiser::initialise(
          GPIOE,
          GPIO_Pin_7 |      // D4
          GPIO_Pin_8 |      // D5
          GPIO_Pin_9 |      // D6
          GPIO_Pin_10,      // D7
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)FsmcPeripheralTraits::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_FSMC,GPIOE_BASE,0>::GPIO_AF);

      // initialise how the FSMC will work and then enable it

      FSMC_Bank=TFsmc::getBank();
      FSMC_DataAddressMux=FSMC_DataAddressMux_Disable;
      FSMC_MemoryType=FSMC_MemoryType_SRAM;
      FSMC_MemoryDataWidth=FSMC_MemoryDataWidth_8b;
      FSMC_BurstAccessMode=FSMC_BurstAccessMode_Disable;
      FSMC_WaitSignalPolarity=FSMC_WaitSignalPolarity_Low;
      FSMC_WrapMode=FSMC_WrapMode_Disable;
      FSMC_WaitSignalActive=FSMC_WaitSignalActive_BeforeWaitState;
      FSMC_WriteOperation=FSMC_WriteOperation_Enable;
      FSMC_WaitSignal=FSMC_WaitSignal_Disable;
      FSMC_WriteBurst=FSMC_WriteBurst_Disable;
      FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable;

      // extended mode is required only if the user has provided separate
      // timings for read and write

      if(&readTiming==&writeTiming) {
        FSMC_ExtendedMode=FSMC_ExtendedMode_Disable;
        FSMC_ReadWriteTimingStruct=const_cast<Fsmc8080LcdTiming *>(&writeTiming);
        FSMC_WriteTimingStruct=nullptr;
      }
      else {
        FSMC_ExtendedMode=FSMC_ExtendedMode_Enable;
        FSMC_ReadWriteTimingStruct=const_cast<Fsmc8080LcdTiming *>(&readTiming);
        FSMC_WriteTimingStruct=const_cast<Fsmc8080LcdTiming *>(&writeTiming);
      }

      // initialise the FSMC and then enable it

      FSMC_NORSRAMInit(this);

      // calculate the data address

      _dataAddress=TFsmc::template getBaseAddress<uint8_t>()+(1<<registerAddressLine);
      _registerAddress=TFsmc::template getBaseAddress<uint8_t>();

      enable(true);
    }


    /**
     * enable or disable the peripheral
     */

    template<class TFsmc>
    inline void Fsmc8BitAccessMode<TFsmc>::enable(bool enable) {
      FSMC_NORSRAMCmd(TFsmc::getBank(),enable ? ENABLE : DISABLE);
    }


    /**
     * Hard-reset the panel
     */

    template<class TFsmc>
    inline void Fsmc8BitAccessMode<TFsmc>::reset() {

      // let the power stabilise

      MillisecondTimer::delay(10);

      // reset sequence

      _resetPin.set();
      MillisecondTimer::delay(5);
      _resetPin.reset();
      MillisecondTimer::delay(50);
      _resetPin.set();
      MillisecondTimer::delay(50);
    }


    /**
     * Get the width of a DMA transfer for this FSMC
     * @return 8 bits
     */

    template<class TFsmc>
    inline void Fsmc8BitAccessMode<TFsmc>::getDmaTransferSizes(uint32_t& peripheralsize,uint32_t& memsize) {
      peripheralsize=DMA_PeripheralDataSize_Byte;
      memsize=DMA_MemoryDataSize_Byte;
    }


    /**
     * Bulk copy data to the panel
     * @param buffer data source
     * @param numWords number of words to transfer
     */

    template<class TFsmc>
    inline void Fsmc8BitAccessMode<TFsmc>::rawTransfer(const void *buffer,uint32_t numWords) const {

      const uint8_t *ptr=static_cast<const uint8_t *>(buffer);

      // shift all the pixels

      while(numWords--)
        *_dataAddress=*ptr++;
    }
  }
}
