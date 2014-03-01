/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Simple carrier class for an implementation of a DMA copier. Used to ensure that we're not restricted
   * to just the common case of the FSMC implementation. i.e. one day DMA-to-GPIO may be implemented
   */

  template<class TDmaCopierImpl>
  class DmaLcdWriter {

    protected:
      TDmaCopierImpl& _impl;

    public:
      DmaLcdWriter(TDmaCopierImpl& impl);

      bool waitUntilComplete();
      void beginCopyToLcd(void *dest,void *source,uint32_t byteCount,uint32_t priority);
  };


  /**
   * Constructor
   * @param impl Reference to the actual implementation
   */

  template<class TDmaCopierImpl>
  inline DmaLcdWriter<TDmaCopierImpl>::DmaLcdWriter(TDmaCopierImpl& impl)
    : _impl(impl) {
  }


  /**
   * Wait until the last transfer completes
   * @return false if the DMA peripheral reports an error
   */

  template<class TDmaCopierImpl>
  inline bool DmaLcdWriter<TDmaCopierImpl>::waitUntilComplete() {
   return _impl.getDma().waitUntilComplete();
  }


  /**
   * Start the DMA copy
   * @param dest Destination register address
   * @param source Source address
   * @param byteCount Number of 8-bit bytes to copy
   * @param priority DMA priority
   */

  template<class TDmaCopierImpl>
  inline void DmaLcdWriter<TDmaCopierImpl>::beginCopyToLcd(void *dest,void *source,uint32_t byteCount,uint32_t priority) {
    _impl.beginCopyToLcd(dest,source,byteCount,priority);
  }
}
