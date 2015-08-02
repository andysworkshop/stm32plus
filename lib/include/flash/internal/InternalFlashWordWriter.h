/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /*
   * Helper "smart pointer" class to permit writing words to the flash using
   * pointer-like semantics. Lock/unlock commands are not done here.
   */

  template<class TFlash>
  class InternalFlashWordWriter {

    protected:
      const TFlash& _flash;
      uint32_t _address;
      bool _errorFlag;

    public:
      InternalFlashWordWriter(const TFlash& flash,const uint32_t address);
      InternalFlashWordWriter(const InternalFlashWordWriter<TFlash>& src);

      bool hasError() const;

      InternalFlashWordWriter<TFlash>& operator*();
      InternalFlashWordWriter<TFlash> operator[](uint32_t idx);
      const InternalFlashWordWriter<TFlash> operator[](uint32_t idx) const;

      InternalFlashWordWriter<TFlash>& operator++();
      InternalFlashWordWriter<TFlash>& operator--();
      InternalFlashWordWriter<TFlash> operator++(int);
      InternalFlashWordWriter<TFlash> operator--(int);
      InternalFlashWordWriter<TFlash>& operator+=(const uint32_t words);
      InternalFlashWordWriter<TFlash>& operator-=(const uint32_t words);
      InternalFlashWordWriter<TFlash>& operator=(const InternalFlashWordWriter<TFlash>& src);
      InternalFlashWordWriter<TFlash>& operator=(uint32_t value);
  };


  /**
   * Constructor
   * @param flash The flash peripheral
   * @param address The flash address
   */

  template<class TFlash>
  inline InternalFlashWordWriter<TFlash>::InternalFlashWordWriter(const TFlash& flash,const uint32_t address)
    : _flash(flash),
      _address(address),
      _errorFlag(false) {
  }


  /**
   * Copy constructor
   * @param src The class to copy from
   */

  template<class TFlash>
  inline InternalFlashWordWriter<TFlash>::InternalFlashWordWriter(const InternalFlashWordWriter<TFlash>& src)
    : _flash(src._flash),
      _address(src._address),
      _errorFlag(src._errorFlag) {
  }


  /**
   * Return reference to self
   * @return *this
   */

  template<class TFlash>
  inline InternalFlashWordWriter<TFlash>& InternalFlashWordWriter<TFlash>::operator*() {
    return *this;
  }


  /*
   * Return a temporary, offset by the index
   * @param idx array index
   * @return a temporary, offset by the index
   */

  template<class TFlash>
  inline InternalFlashWordWriter<TFlash> InternalFlashWordWriter<TFlash>::operator[](uint32_t idx) {

    InternalFlashWordWriter<TFlash> t(*this);

    t+=idx;
    return t;
  }


  /**
   * Const version of the above
   * @param idx array index
   * @return a temporary, offset by the index
   */

  template<class TFlash>
  inline const InternalFlashWordWriter<TFlash> InternalFlashWordWriter<TFlash>::operator[](uint32_t idx) const {

    InternalFlashWordWriter<TFlash> t(*this);

    t+=idx;
    return t;
  }


  /**
   * pre-increment operator
   * @return ourselves offset by a word
   */

  template<class TFlash>
  inline InternalFlashWordWriter<TFlash>& InternalFlashWordWriter<TFlash>::operator++() {
    _address+=4;
    return *this;
  }


  /**
   * Post increment operator
   * @param dummy
   * @return Ourselves, before the update
   */

  template<class TFlash>
  inline InternalFlashWordWriter<TFlash> InternalFlashWordWriter<TFlash>::operator++(int) {
    InternalFlashWordWriter<TFlash> t(*this);
    _address+=4;
    return t;
  }


  /**
   * Pre-decrement operator
   * @return ourselves, decremented by a word
   */

  template<class TFlash>
  inline InternalFlashWordWriter<TFlash>& InternalFlashWordWriter<TFlash>::operator--() {
    _address-=4;
    return *this;
  }


  /**
   * Post-decrement operator
   * @param dummy
   * @return Ourselves, before the update
   */

  template<class TFlash>
  inline InternalFlashWordWriter<TFlash> InternalFlashWordWriter<TFlash>::operator--(int) {
    InternalFlashWordWriter<TFlash> t(*this);
    _address-=4;
    return t;
  }


  /**
   * Increment and assign operator
   * @param words number of words to increment by
   * @return *this
   */

  template<class TFlash>
  inline InternalFlashWordWriter<TFlash>& InternalFlashWordWriter<TFlash>::operator+=(const uint32_t words) {
    _address+=words*4;
    return *this;
  }


  /**
   * Decrement and assign operator
   * @param words number of words to decrement by
   * @return *this
   */

  template<class TFlash>
  inline InternalFlashWordWriter<TFlash>& InternalFlashWordWriter<TFlash>::operator-=(const uint32_t words) {
    _address-=words*4;
    return *this;
  }


  /**
   * Assign another instance of this pointer to ourselves.
   * @param src The pointer to copy from
   * @return *this;
   */

  template<class TFlash>
  inline InternalFlashWordWriter<TFlash>& InternalFlashWordWriter<TFlash>::operator=(const InternalFlashWordWriter<TFlash>& src) {
    _address=src._address;
    return *this;
  }


  /**
   * Write a word to the address pointed to by this pointer. Success/fail is stored in a member variable
   * @param value The word to write
   * @return *this
   */

  template<class TFlash>
  inline InternalFlashWordWriter<TFlash>& InternalFlashWordWriter<TFlash>::operator=(uint32_t value) {
    _errorFlag=!_flash.wordProgram(_address,value);
    return *this;
  }


  /**
   * Get the value of the error flag
   * @return true if there has been an error
   */

  template<class TFlash>
  inline bool InternalFlashWordWriter<TFlash>::hasError() const {
    return _errorFlag;
  }
}
