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
      uint8_t getPinIndex() const;

      GpioPinRef& operator=(const GpioPinRef& src);
      GpioPinRef& operator=(const Gpio& src);

      bool operator==(const GpioPinRef& src) const;
      bool operator!=(const GpioPinRef& src) const;

      Gpio::GpioModeType getMode() const;
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
   * @return The GPIO_TypeDef port address
   */

  inline GPIO_TypeDef* GpioPinRef::getPeripheralAddress() const {
    return _peripheralAddress;
  }


  /**
   * Return the pin. This is the pin bit mask (1,2,4,8,16...)
   * @return The pin bit mask.
   */

  inline uint16_t GpioPinRef::getPin() const {
    return _pin;
  }


  /**
   * Set the pin to HIGH
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


  /**
   * Assignment operator from GpioPinRef
   * @param src the object to copy from
   * @return self
   */

  inline GpioPinRef& GpioPinRef::operator=(const GpioPinRef& src) {
    _peripheralAddress=src.getPeripheralAddress();
    _pin=src.getPin();
    return *this;
  }



  /**
   * Assignment operator from Gpio
   * @param src the object to copy from
   * @return self
   */

  inline GpioPinRef& GpioPinRef::operator=(const Gpio& src) {
    _peripheralAddress=src.getPeripheralAddress();
    _pin=src.getSelectedPin();
    return *this;
  }


  /**
   * Equality comparison operator
   * @param src the object to compare to
   * @return true if equal
   */

  inline bool GpioPinRef::operator==(const GpioPinRef& src) const {
    return _peripheralAddress==src._peripheralAddress && _pin==src._pin;
  }


  /**
   * Inequality comparison operator
   * @param src the object to compare to
   * @return true if not equal
   */

  inline bool GpioPinRef::operator!=(const GpioPinRef& src) const {
    return _peripheralAddress!=src._peripheralAddress || _pin!=src._pin;
  }


  /**
   * Get the pin index (0..15)
   * @return the pin index in the port
   */

  inline uint8_t GpioPinRef::getPinIndex() const {
    return bithacks::firstSetBit(_pin);
  }


  /**
   * Get the pin mode type (input,output,analog,alternate function)
   * @return the mode type
   */

  inline Gpio::GpioModeType GpioPinRef::getMode() const {
    return Gpio::getMode(_peripheralAddress,_pin);
  }
}
