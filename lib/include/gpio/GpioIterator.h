/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {


  /**
   * STL compatible forward iterator that can be used to iterate over GpioPinRef
   * instances in a port. Instances of this iterator should be obtained from the
   * GpioPort object using the begin() and end() methods. Basic usage example:
   *
   *   GpioC<DefaultDigitalInputFeature<1,7,13>,DefaultDigitalOutputFeature<8,9,15>> pc;
   *
   *    for(auto it=pc.begin();it!=pc.end();it++) {
   *      if(it->getMode()==Gpio::OUTPUT) {
   *        // do something
   *      }
   *    }
   *
   *  This iterator is compatible with algorithms in the <algorithm> and <util/StdExt.h> header
   *  that take a forward iterator. In common with the STL iterators you are expected to
   *  be diligent in checking the start and end position using begin() and end() because
   *  the increment and decrement operators will not check for you.
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
     * @return reference to GpioPinRef object at position
     */

    GpioPinRef& operator*() {
      Gpio& r(*_pinHandlers[_index]);
      r.setSelectedPin(_index);
      _current=r;
      return _current;
    }


    /**
     * Pointer operator
     * @return address of the current object at the index
     */

    GpioPinRef *operator->() {
      return &(operator*());
    }


    /**
     * Increment the iterator
     * @return self reference
     */

    GpioIterator& operator++() {
      increment();
      return *this;
    }


    /**
     * Increment the iterator
     * @return self reference
     */

    GpioIterator& operator++(int) {
      GpioIterator& tmp=*this;
      increment();
      return tmp;
    }


    /**
     * Decrement the iterator
     * @return self reference
     */

    GpioIterator& operator--() {
      decrement();
      return *this;
    }


    /**
     * Decrement the iterator
     * @return self reference
     */

    GpioIterator& operator--(int) {
      GpioIterator& tmp=*this;
      increment();
      return tmp;
    }


    /**
     * Equality comparison
     * @param rhs the other iterator
     * @return true if the iterators are logically equivalent
     */

    bool operator==(const GpioIterator& rhs) const {
      return _peripheralAddress==rhs._peripheralAddress && _index==rhs._index;
    }


    /**
     * Inequality comparison
     * @param rhs the other iterator
     * @return true if the iterators are not logically equivalent
     */

    bool operator!=(const GpioIterator& rhs) const {
      return _peripheralAddress!=rhs._peripheralAddress || _index!=rhs._index;
    }


    /**
     * Increment the iterator to the next active pin
     */

    void increment() {
      do {
        _index++;
      } while(_pinHandlers[_index]==nullptr && _index!=16);
    }


    /**
     * Decrement the iterator to the next active pin
     */

    void decrement() {
      do {
      _index--;
      } while(_pinHandlers[_index]==nullptr);
    }
  };
}
