/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


// ensure the MCU series is correct
#ifndef STM32PLUS_F0
#error This class can only be used with the STM32F0 series
#endif


namespace stm32plus {

  /**
   * @brief Base class for DMA subclasses.
   */

  class Dma {

    public:

      typedef DMA_Channel_TypeDef DMA_PeripheralType;

      /**
       * Error codes
       */

      enum {
        /// Transfer error
        E_TRANSFER_ERROR=1
      };

    protected:
      DMA_PeripheralType *_peripheralAddress;
      uint32_t _completeFlag;
      uint32_t _halfCompleteFlag;
      uint32_t _errorFlag;

    protected:
      Dma(DMA_PeripheralType *peripheralAddress,uint32_t completeFlag,uint32_t halfCompleteFlag,uint32_t errorFlag);

    public:
      void clearCompleteFlag() const;
      void clearHalfCompleteFlag() const;
      void clearErrorFlag() const;

      bool waitUntilComplete() const;

      bool isComplete() const;
      bool isHalfComplete() const;
      bool isError() const;

      operator DMA_PeripheralType *();
  };


  /**
   * Constructor.
   * @param[in] channel The DMA channel that we are using.
   * @param[in] interruptFlags If there is an observer then these are the interrupts that we are observing.
   */

  inline Dma::Dma(DMA_PeripheralType *peripheralAddress,uint32_t completeFlag,uint32_t halfCompleteFlag,uint32_t errorFlag) {

    // save channel and get peripheral index

    _peripheralAddress=peripheralAddress;
    _completeFlag=completeFlag;
    _halfCompleteFlag=halfCompleteFlag;
    _errorFlag=errorFlag;
  }


  /**
   * Check for complete
   * @return
   */

  inline bool Dma::isComplete() const {

    if(DMA_GetFlagStatus(_completeFlag)) {
      clearCompleteFlag();
      return true;
    }

    return false;
  }


  /**
   * Check for half-complete
   * @return
   */

  inline bool Dma::isHalfComplete() const {

    if(DMA_GetFlagStatus(_halfCompleteFlag)) {
      clearHalfCompleteFlag();
      return true;
    }

    return false;
  }


  /**
   * Check for error
   * @return
   */

  inline bool Dma::isError() const {

    if(DMA_GetFlagStatus(_errorFlag)) {
      clearErrorFlag();
      errorProvider.set(ErrorProvider::ERROR_PROVIDER_DMA,E_TRANSFER_ERROR);
      return true;
    }

    return false;
  }


  /**
   * Block the CPU and Wait until the DMA transfer is complete. There must be a DMA transfer
   * started on this channel or this function will hang forever.
   *
   * @return true if the transfer completes OK, false if there is a DMA error while waiting
   */

  inline bool Dma::waitUntilComplete() const {

    while(!DMA_GetFlagStatus(_completeFlag)) {
      if(DMA_GetFlagStatus(_errorFlag)) {
        DMA_ClearFlag(_errorFlag);
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_DMA,E_TRANSFER_ERROR);
      }
    }

    clearCompleteFlag();
    return true;
  }


  /**
   * Clear the complete flag. You need to do this manually if you trigger more DMA transfers
   * from within a DMA interrupt handler
   */

  inline void Dma::clearCompleteFlag() const {
    DMA_ClearFlag(_completeFlag);
  }


  /**
   * Clear the complete flag. You need to do this manually if you trigger more DMA transfers
   * from within a DMA interrupt handler
   */

  inline void Dma::clearHalfCompleteFlag() const {
    DMA_ClearFlag(_halfCompleteFlag);
  }


  /**
   * Clear the error flag
   */

  inline void Dma::clearErrorFlag() const {
    DMA_ClearFlag(_errorFlag);
  }


  /**
   * Peripheral address cast operator
   */

  inline Dma::operator DMA_PeripheralType *() {
    return _peripheralAddress;
  }
}
