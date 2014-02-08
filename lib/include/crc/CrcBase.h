/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Base class for the CRC peripheral. Contains most of the implementation. See CrcPeripheral class for
   * usage documentation. Everything in this class is endian-independent.
   *
   * The CRC unit operates on 32 bit words and you are most likely to be operating on
   * a byte stream, hence this class takes bytes as input.
   *
   * reset() is called on construction and you can call it whenever afterwards. You then
   * call addNewData() as many times as you have bytes to use and then call finish to
   * flush any remaining bytes (with configurable padding). Both finish() and addNewData()
   * return the current CRC value. The only valid method call after finish() has been
   * called is reset()
   */

  class CrcBase {
    protected:
      uint32_t _currentWord;
      uint8_t _currentIndex;
      uint32_t _padding;

    public:
      CrcBase(uint8_t padding=0);
      ~CrcBase();

      void reset();
      uint32_t finish() const;

      uint32_t currentCrc() const;
  };


  /**
   * Constructor. Initialise state variables
   * @param padding The byte to pad the final word with when your input stream size is not a multiple of 32 bits.
   */

  inline CrcBase::CrcBase(uint8_t padding) {

    ClockControl<PERIPHERAL_CRC>::On();

    _padding=padding | padding << 8 | padding << 16 | padding << 24;
    reset();
  }


  /*
   * Destructor
   */

  inline CrcBase::~CrcBase() {
    ClockControl<PERIPHERAL_CRC>::Off();
  }


  /**
   * Reset the CRC calculation ready for re-use
   */

  inline void CrcBase::reset() {

    _currentIndex=0;
    _currentWord=_padding;

    CRC_ResetDR();
  }
}
