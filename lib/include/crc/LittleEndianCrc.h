/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Generic template implementation of the CRC peripheral. This peripheral is the
   * same on the F1 and F4 so there are no specialisations.
   *
   * The CRC unit operates on 32 bit words and you are most likely to be operating on
   * a byte stream, hence this class takes bytes as input.
   *
   * reset() is called on construction and you can call it whenever afterwards. You then
   * call addNewData() as many times as you have bytes to use and then call finish to
   * flush any remaining bytes (with configurable padding). Both finish() and addNewData()
   * return the current CRC value. The only valid method call after finish() has been
   * called is reset()
   *
   * The little endian implementation operates in the default mode of the CRC unit.
   */

  template<>
  class CrcPeripheral<LITTLE_ENDIAN> : public CrcBase {

    public:
      struct Parameters {
        uint8_t crc_padding;

        Parameters(uint8_t padding=0) {
          crc_padding=padding;
        }
      };

    public:
      CrcPeripheral(const Parameters& params);
      uint32_t addNewData(uint8_t nextByte);
      uint32_t calculateWordBuffer(uint32_t *buffer,uint32_t count) const;

      uint32_t finish() const;
      uint32_t currentCrc() const;
  };


  /**
   * typedefs for easy use
   */

  typedef CrcPeripheral<LITTLE_ENDIAN> CrcLittleEndian;


  /**
   * Constructor. Initialise state variables
   * @param padding The byte to pad the final word with when your input stream size is not a multiple of 32 bits.
   */

  inline CrcPeripheral<LITTLE_ENDIAN>::CrcPeripheral(const Parameters& params)
    : CrcBase(params.crc_padding) {
  }


  /**
   * Add a new data byte to the calculation and return the current value of the calculation. The current value will change
   * every 4 bytes because of the 32-bit resolution of the peripheral
   * @param nextByte The next byte to add to the calculation
   * @return The current value of the CRC.
   */

  inline uint32_t CrcPeripheral<LITTLE_ENDIAN>::addNewData(uint8_t nextByte) {

    // shift down the current word to make space for the new data and OR it in

    _currentWord>>=8;
    _currentWord|=static_cast<uint32_t>(nextByte) << 24;

    if(_currentIndex++==3) {

      uint32_t crc;

      crc=CRC_CalcCRC(_currentWord);
      _currentWord=_padding;
      _currentIndex=0;

      return crc;
    }

    return currentCrc();
  }


  /**
   * Calculate the CRC of an whole buffer of 32-bit words
   * @param buffer The start of the buffer
   * @param count The number of words
   * @return The CRC
   */

  inline uint32_t CrcPeripheral<LITTLE_ENDIAN>::calculateWordBuffer(uint32_t *buffer,uint32_t count) const {
    return CRC_CalcBlockCRC(buffer,count);
  }


  /**
   * Finish the stream, writing any remaining bytes including the padding
   * @return The final CRC value
   */

  inline uint32_t CrcPeripheral<LITTLE_ENDIAN>::finish() const {

    // if there are no pending bytes then return the current CRC value
    // otherwise add the pending word and return the updated CRC

    if(_currentIndex==0)
      return currentCrc();
    else
      return CRC_CalcCRC(_currentWord);
  }


  /**
   * Return the current CRC value
   * @return The current CRC
   */

  inline uint32_t CrcPeripheral<LITTLE_ENDIAN>::currentCrc() const {
    return CRC_GetCRC();
  }
}
