/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {


  /*
   * STL compatible forward iterator that can be used to iterate over GpioPinRef
   * instances in a port
   */

  struct GpioIterator {

    Gpio **_pinHandlers;
    GPIO_TypeDef *_peripheralAddress;
    uint8_t _index;
    GpioPinRef _current;


    /**
     * Default constructor
     */

    GpioIterator() {}


    /**
     * Construct with initial parameters
     * @param pinHandlers The base of the sparse array of Gpio pointers
     * @param peripheralAddress The GPIO peripheral address
     * @param index The initial index for this iterator
     */

    GpioIterator(Gpio **pinHandlers,GPIO_TypeDef *peripheralAddress,uint8_t index) {
      _pinHandlers=pinHandlers;
      _peripheralAddress=peripheralAddress;
      _index=index;
    }


    /**
     * Copy constructor
     * @param it The iterator to copy from
     */

    GpioIterator(const GpioIterator& it) {
      _pinHandlers=it._pinHandlers;
      _peripheralAddress=it._peripheralAddress;
      _index=it._index;
    }


    /**
     * Dereference operator
     * @return reference to Gpio object at position
     */

    GpioPinRef& operator*() {
      Gpio& r(*_pinHandlers[_index]);
      r.setSelectedPin(_index);
      _current=r;
      return _current;
    }


    /**
     * Pointer operator
     * @return
     */

    GpioPinRef *operator->() {
      return &(operator*());
    }

    GpioIterator& operator++() {
      increment();
      return *this;
    }

    GpioIterator& operator++(int) {
      GpioIterator& tmp=*this;
      increment();
      return tmp;
    }

    GpioIterator& operator--() {
      decrement();
      return *this;
    }

    GpioIterator& operator--(int) {
      GpioIterator& tmp=*this;
      increment();
      return tmp;
    }


    bool operator==(const GpioIterator& rhs) const {
      return _peripheralAddress==rhs._peripheralAddress && _index==rhs._index;
    }

    bool operator!=(const GpioIterator& rhs) const {
      return _peripheralAddress!=rhs._peripheralAddress || _index!=rhs._index;
    }


    void increment() {
      do {
        _index++;
      } while(_pinHandlers[_index]==nullptr && _index!=16);
    }

    void decrement() {
      do {
      _index--;
      } while(_pinHandlers[_index]==nullptr);
    }
  };
}
