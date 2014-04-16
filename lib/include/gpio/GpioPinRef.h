/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {


  /**
   * Simple wrapper class for a port and a pin. Useful for when a class would need to store a reference
   * to a Gpio object but cannot because the Gpio references returned by Gpio.operator[] are generally
   * not safe to hold on to.
   *
   * It's always safe to do a bitwise copy of this class
   */

  class GpioPinRef {

    protected:
      GPIO_TypeDef *_peripheralAddress;
      uint16_t _pin;

    public:
      GpioPinRef() {}
      GpioPinRef(const Gpio& gpio);
      GpioPinRef(GPIO_TypeDef *peripheralAddress,uint16_t pin);

      void set() const;
      void reset() const;
      void setState(bool state) const;
      bool read() const;

      GPIO_TypeDef *getPeripheralAddress() const;
      uint16_t getPin() const;
  };


  /**
   * Constructor
   * @param gpio The Gpio class
   */

  inline GpioPinRef::GpioPinRef(const Gpio& gpio) {
    _peripheralAddress=gpio.getPeripheralAddress();
    _pin=gpio.getSelectedPin();
  }


  /**
   * Constructor
   * @param peripheralAddress GPIO port base
   * @param pin peripheral library compatible pin number
   */

  inline GpioPinRef::GpioPinRef(GPIO_TypeDef *peripheralAddress,uint16_t pin)
    : _peripheralAddress(peripheralAddress),
      _pin(pin) {
  }


  /**
   * Return the port
   */

  inline GPIO_TypeDef* GpioPinRef::getPeripheralAddress() const {
    return _peripheralAddress;
  }


  /**
   * Return the pin
   */

  inline uint16_t GpioPinRef::getPin() const {
    return _pin;
  }


  /**
   * Set the pin
   */

  inline void GpioPinRef::set() const {
    GPIO_WriteBit(_peripheralAddress,_pin,Bit_SET);
  }


  /**
   * Set the selected pin to LOW.
   */

  inline void GpioPinRef::reset() const {
    GPIO_WriteBit(_peripheralAddress,_pin,Bit_RESET);
  }


  /**
   * Set the pin state
   */

  inline void GpioPinRef::setState(bool state) const {
    GPIO_WriteBit(_peripheralAddress,_pin,state ? Bit_SET : Bit_RESET);
  }


  /**
   * Read the selected pin state.
   * @return The pin state.
   */

  inline bool GpioPinRef::read() const {
    return GPIO_ReadInputDataBit(_peripheralAddress,_pin);
  }
}
