/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F4
#error This class can only be used with the STM32F4 series
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

      volatile uint16_t *getSetRegister() const;
      volatile uint16_t *getResetRegister() const;
      volatile uint32_t *getOutputRegister() const;

      void initialiseAF(uint16_t pinIds,
                        uint8_t afSelection,
                        GPIOSpeed_TypeDef speed=GPIO_Speed_50MHz,
                        Gpio::GpioOutputType outputType=Gpio::PUSH_PULL,
                        Gpio::GpioPullUpDownType pullUpDownType=Gpio::PUPD_NONE);

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
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
    SYSCFG_EXTILineConfig(portSource,pinSource);
  }


  /**
   * Get the port and pin source for EXTI
   * @param[out] portSource The port source
   * @param[out] pinSource The pin source
   */

  inline void Gpio::getPortAndPinSource(uint8_t& portSource,uint8_t& pinSource) const {

    switch(reinterpret_cast<uint32_t>(_peripheralAddress)) {
      case GPIOA_BASE: portSource=EXTI_PortSourceGPIOA; break;
      case GPIOB_BASE: portSource=EXTI_PortSourceGPIOB; break;
      case GPIOC_BASE: portSource=EXTI_PortSourceGPIOC; break;
      case GPIOD_BASE: portSource=EXTI_PortSourceGPIOD; break;
      case GPIOE_BASE: portSource=EXTI_PortSourceGPIOE; break;
      case GPIOH_BASE: portSource=EXTI_PortSourceGPIOH; break;

#if defined(STM32PLUS_F4_HAS_GPIOF_G_I)
      case GPIOF_BASE: portSource=EXTI_PortSourceGPIOF; break;
      case GPIOG_BASE: portSource=EXTI_PortSourceGPIOG; break;
      case GPIOI_BASE: portSource=EXTI_PortSourceGPIOI; break;
#endif

#if defined(STM32PLUS_F4_HAS_GPIOJ_K)
      case GPIOJ_BASE: portSource=EXTI_PortSourceGPIOJ; break;
      case GPIOK_BASE: portSource=EXTI_PortSourceGPIOK; break;
#endif
      default: portSource=0; break;
    }

    switch(_selectedPin) {
      case GPIO_Pin_0: pinSource=EXTI_PinSource0; break;
      case GPIO_Pin_1: pinSource=EXTI_PinSource1; break;
      case GPIO_Pin_2: pinSource=EXTI_PinSource2; break;
      case GPIO_Pin_3: pinSource=EXTI_PinSource3; break;
      case GPIO_Pin_4: pinSource=EXTI_PinSource4; break;
      case GPIO_Pin_5: pinSource=EXTI_PinSource5; break;
      case GPIO_Pin_6: pinSource=EXTI_PinSource6; break;
      case GPIO_Pin_7: pinSource=EXTI_PinSource7; break;
      case GPIO_Pin_8: pinSource=EXTI_PinSource8; break;
      case GPIO_Pin_9: pinSource=EXTI_PinSource9; break;
      case GPIO_Pin_10: pinSource=EXTI_PinSource10; break;
      case GPIO_Pin_11: pinSource=EXTI_PinSource11; break;
      case GPIO_Pin_12: pinSource=EXTI_PinSource12; break;
      case GPIO_Pin_13: pinSource=EXTI_PinSource13; break;
      case GPIO_Pin_14: pinSource=EXTI_PinSource14; break;
      case GPIO_Pin_15: pinSource=EXTI_PinSource15; break;
      default: pinSource=0; break;
    }
  }


  /**
   * Get the register for setting bits.
   * @return The register address
   */

  inline volatile uint16_t *Gpio::getSetRegister() const {
    return reinterpret_cast<volatile uint16_t *>(&_peripheralAddress->BSRRL);
  }


  /**
   * Get the register for clearing bits.
   * @return The register address
   */

  inline volatile uint16_t *Gpio::getResetRegister() const {
    return reinterpret_cast<volatile uint16_t *>(&_peripheralAddress->BSRRH);
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
   * Alternative to template initialisation - initialise programatically
   * @param speed
   * @param TOutputType
   * @param TPullUpDownType
   * @param pinIds
   * @param afSelection
   */

  inline void Gpio::initialiseAF(uint16_t pinIds,
                                 uint8_t afSelection,
                                 GPIOSpeed_TypeDef speed,
                                 Gpio::GpioOutputType outputType,
                                 Gpio::GpioPullUpDownType pullUpDownType) {
    uint8_t source;
    uint32_t bit;
    GPIO_InitTypeDef init;

    (void)pullUpDownType;     // unused

    _pinIds=pinIds;

    init.GPIO_Speed=speed;
    init.GPIO_Mode=GPIO_Mode_AF;
    init.GPIO_OType=outputType==Gpio::PUSH_PULL ? GPIO_OType_PP : GPIO_OType_OD;
    init.GPIO_PuPd=pullUpDownType==PUPD_NONE ? GPIO_PuPd_NOPULL : (pullUpDownType==PUPD_UP ? GPIO_PuPd_UP : GPIO_PuPd_DOWN);
    init.GPIO_Pin=pinIds;

    GPIO_Init(_peripheralAddress,&init);

    // need to configure each of the selected pins for AF

    for(bit=1,source=0;bit<=0x8000;bit<<=1,source++) {
      if((pinIds & bit)!=0) {
        GPIO_PinAFConfig(_peripheralAddress,source,afSelection);
        _portBase.setPinHandler(source,this);
      }
    }
  }


  /**
   * Get the pin mode type (input,output,analog,alternate function)
   * @param peripheralAddress the peripheral register address
   * @param pin the pin bitmask
   * @return The mode type
   */

  inline Gpio::GpioModeType Gpio::getMode(GPIO_TypeDef *peripheralAddress,uint16_t pin) {

    uint8_t pinIndex;

    pinIndex=bithacks::firstSetBit(pin);

    switch((peripheralAddress->MODER >> (pinIndex*2)) & 0x3) {

      case 0:
        return Gpio::INPUT;

      case 1:
        return Gpio::OUTPUT;

      case 2:
        return Gpio::ALTERNATE_FUNCTION;

      case 3:
      default:
        return Gpio::ANALOG;
    }
  }


  /**
   * Get the pin mode type (input,output,analog,alternate function)
   * @return the mode type
   */

  inline Gpio::GpioModeType Gpio::getMode() const {
    return getMode(_peripheralAddress,_selectedPin);
  }
}
