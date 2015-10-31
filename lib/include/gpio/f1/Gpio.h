/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F1
#error This class can only be used with the STM32F1 series
#endif


namespace stm32plus {


  /**
   * @brief Base class for holding common pin functionality. Inherits from the
   * GPIO_InitTypeDef structure
   */

  class Gpio {

    protected:
      GpioPortBase& _portBase;
      GPIO_TypeDef *_peripheralAddress;

      uint16_t _pinIds;
      uint16_t _selectedPin;

    public:

      /**
       * Possible mode types
       */

      enum GpioModeType {
        OUTPUT,
        INPUT,
        ANALOG,
        ALTERNATE_FUNCTION
      };

      /**
       * Possible output types
       */

      enum GpioOutputType {
        OPEN_DRAIN,//!< OPEN_DRAIN
        PUSH_PULL  //!< PUSH_PULL
      };

      /**
       * Possible pull-up / down types
       */

      enum GpioPullUpDownType {
        PUPD_NONE,//!< PP_NONE
        PUPD_UP,  //!< PP_UP
        PUPD_DOWN //!< PP_DOWN
      };

    protected:
      void getPortAndPinSource(uint8_t& portSource,uint8_t& pinSource) const;

    public:
      Gpio(GpioPortBase& portBase);

      void set() const;
      void reset() const;
      void setAll() const;
      void resetAll() const;
      void setState(bool state) const;
      void setStateAll(bool state) const;
      bool read() const;
      void enableExti() const;

      volatile uint32_t *getSetRegister() const;
      volatile uint32_t *getResetRegister() const;
      volatile uint32_t *getOutputRegister() const;

      void setSelectedPin(uint8_t pinNumber);
      uint16_t getSelectedPin() const;

      GPIO_TypeDef *getPeripheralAddress() const;

      static GpioModeType getMode(GPIO_TypeDef *peripheralAddress,uint16_t pin);
      GpioModeType getMode() const;
  };


  /**
   * These two functions are for the variadic parameter pack expansion. When called they will
   * do compile-time recursion to compute the 32-bit pin bit mask from a variadic pack of
   * pin numbers (0..15)
   */

  namespace  {

    /**
     * Recursion termination condition
     */

    template<uint8_t TPin>
    void GpioPinMerge(uint32_t& pins) {
      pins|=1 << TPin;
    }


    /**
     * General recursive caller
     */

    template<uint8_t First,uint8_t Next,uint8_t... Rest>
    void GpioPinMerge(uint32_t& pins) {
      pins|=1 << First;
      GpioPinMerge<Next,Rest...>(pins);
    }
  }


  /**
   * Constructor
   * @param port
   */

  inline Gpio::Gpio(GpioPortBase& portBase)
    : _portBase(portBase),
      _peripheralAddress(portBase.getPeripheralAddress()) {
  }


  /**
   * Get the currently selected pin id
   * @return The pin ID
   */

  inline uint16_t Gpio::getSelectedPin() const {
    return _selectedPin;
  }


  /**
   * Cast to the port typedef
   */

  inline GPIO_TypeDef *Gpio::getPeripheralAddress() const {
    return _peripheralAddress;
  }


  /**
   * Set the selected pin number to one of those that you initialised this class with
   * @param pinNumber The pin number (0..15)
   */

  inline void Gpio::setSelectedPin(uint8_t pinNumber) {
    _selectedPin=1 << pinNumber;
  }


  /**
   * Set the selected pin to HIGH.
   */

  inline void Gpio::set() const {
    GPIO_WriteBit(_peripheralAddress,_selectedPin,Bit_SET);
  }


  /**
   * Set all pins managed by this class to HIGH
   */

  inline void Gpio::setAll() const {
    GPIO_WriteBit(_peripheralAddress,_pinIds,Bit_SET);
  }


  /**
   * Set the selected pin to LOW.
   */

  inline void Gpio::reset() const {
    GPIO_WriteBit(_peripheralAddress,_selectedPin,Bit_RESET);
  }


  /**
   * Set all the pins managed by this class to LOW.
   */

  inline void Gpio::resetAll() const {
    GPIO_WriteBit(_peripheralAddress,_pinIds,Bit_RESET);
  }


  /**
   * Allow setting/resetting of the selected pin from a variable.
   * @param[in] state The new state of the pin.
   */

  inline void Gpio::setState(bool state) const {
    GPIO_WriteBit(_peripheralAddress,_selectedPin,state ? Bit_SET : Bit_RESET);
  }


  /**
   * Allow setting/resetting of all pins managed by this class from a variable.
   * @param[in] state The new state of the pin.
   */

  inline void Gpio::setStateAll(bool state) const {
    GPIO_WriteBit(_peripheralAddress,_pinIds,state ? Bit_SET : Bit_RESET);
  }


  /**
   * Read the selected pin state.
   * @return The pin state.
   */

  inline bool Gpio::read() const {
    return GPIO_ReadInputDataBit(_peripheralAddress,_selectedPin);
  }


  /**
   * Enable EXTI for this port/selected pin
   */

  inline void Gpio::enableExti() const {

    uint8_t portSource,pinSource;

    getPortAndPinSource(portSource,pinSource);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    GPIO_EXTILineConfig(portSource,pinSource);
  }


  /**
   * Get the port and pin source for EXTI
   * @param[out] portSource The port source
   * @param[out] pinSource The pin source
   */

  inline void Gpio::getPortAndPinSource(uint8_t& portSource,uint8_t& pinSource) const {

    switch(reinterpret_cast<uint32_t>(_peripheralAddress)) {
      case GPIOA_BASE: portSource=GPIO_PortSourceGPIOA; break;
      case GPIOB_BASE: portSource=GPIO_PortSourceGPIOB; break;
      case GPIOC_BASE: portSource=GPIO_PortSourceGPIOC; break;
      case GPIOD_BASE: portSource=GPIO_PortSourceGPIOD; break;
      case GPIOE_BASE: portSource=GPIO_PortSourceGPIOE; break;
      case GPIOF_BASE: portSource=GPIO_PortSourceGPIOF; break;
      case GPIOG_BASE: portSource=GPIO_PortSourceGPIOG; break;
      default: portSource=0; break;
    }

    switch(_selectedPin) {
      case GPIO_Pin_0: pinSource=GPIO_PinSource0; break;
      case GPIO_Pin_1: pinSource=GPIO_PinSource1; break;
      case GPIO_Pin_2: pinSource=GPIO_PinSource2; break;
      case GPIO_Pin_3: pinSource=GPIO_PinSource3; break;
      case GPIO_Pin_4: pinSource=GPIO_PinSource4; break;
      case GPIO_Pin_5: pinSource=GPIO_PinSource5; break;
      case GPIO_Pin_6: pinSource=GPIO_PinSource6; break;
      case GPIO_Pin_7: pinSource=GPIO_PinSource7; break;
      case GPIO_Pin_8: pinSource=GPIO_PinSource8; break;
      case GPIO_Pin_9: pinSource=GPIO_PinSource9; break;
      case GPIO_Pin_10: pinSource=GPIO_PinSource10; break;
      case GPIO_Pin_11: pinSource=GPIO_PinSource11; break;
      case GPIO_Pin_12: pinSource=GPIO_PinSource12; break;
      case GPIO_Pin_13: pinSource=GPIO_PinSource13; break;
      case GPIO_Pin_14: pinSource=GPIO_PinSource14; break;
      case GPIO_Pin_15: pinSource=GPIO_PinSource15; break;
      default: pinSource=0; break;
    }
  }


  /**
   * Get the register for setting bits. As per the docs only 32-bit
   * access is permitted.
   * @return The register address
   */

  inline volatile uint32_t *Gpio::getSetRegister() const {
    return reinterpret_cast<volatile uint32_t *>(&_peripheralAddress->BSRR);
  }


  /**
   * Get the register for clearing bits. As per the docs only 32-bit
   * access is permitted.
   * @return The register address
   */

  inline volatile uint32_t *Gpio::getResetRegister() const {
    return reinterpret_cast<volatile uint32_t *>(&_peripheralAddress->BRR);
  }


  /**
   * Get the register for writing data. As per the docs only 32-bit
   * access is permitted.
   * @return
   */

  inline volatile uint32_t *Gpio::getOutputRegister() const {
    return reinterpret_cast<volatile uint32_t *>(&_peripheralAddress->ODR);
  }


  /**
   * Get the pin mode type (input,output,analog,alternate function)
   * @param peripheralAddress the peripheral register address
   * @param pin the pin bitmask
   * @return The mode type
   */

  inline Gpio::GpioModeType Gpio::getMode(GPIO_TypeDef *peripheralAddress,uint16_t pin) {

    uint8_t pinIndex;
    uint32_t regval;

    // two registers on the F1. CRL for pins 0..7 and CRH for pins 8..15

    if((pinIndex=bithacks::firstSetBit(pin))<8)
      regval=(peripheralAddress->CRL >> (pinIndex*4)) & 0xF;
    else
      regval=(peripheralAddress->CRH >> ((pinIndex-8)*4)) & 0xF;

    // MODE bits differentiate input/output, CNF bits differentiate subtypes

    if((regval & 0x3)==0)
      return (regval & 0xC)==0 ? Gpio::ANALOG : Gpio::INPUT;
    else
      return (regval & 0xC)==0 || (regval & 0xC)==4 ? Gpio::OUTPUT : Gpio::ALTERNATE_FUNCTION;
  }


  /**
   * Get the pin mode type (input,output,analog,alternate function)
   * @return the mode type
   */

  inline Gpio::GpioModeType Gpio::getMode() const {
    return getMode(_peripheralAddress,_selectedPin);
  }
}
