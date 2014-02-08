/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Generic template implementation of the CRC peripheral.
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

  template<Endian TEndian>
  class CrcPeripheral;
}
