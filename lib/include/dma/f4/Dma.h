/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


// ensure the MCU series is correct
#ifndef STM32PLUS_F4
#error This class can only be used with the STM32F4 series
#endif


namespace stm32plus {

  /**
   * @brief Base DMA implementation for the F4.
   */

  class Dma {

    public:

      typedef DMA_Stream_TypeDef DMA_PeripheralType;

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
      uint32_t _channelNumber;

    protected:
      Dma(DMA_PeripheralType *peripheralAddress,uint32_t channelNumber,uint32_t completeFlag,uint32_t halfCompleteFlag,uint32_t errorFlag);

    public:
      void clearCompleteFlag() const;
      void clearHalfCompleteFlag() const;
      void clearErrorFlag() const;

      bool waitUntilComplete() const;

      bool isComplete() const;
      bool isHalfComplete() const;
      bool isError() const;

      uint32_t getChannelNumber() const;

      operator DMA_PeripheralType *();
  };


  /**
   * Constructor.
   * @param[in] channel The DMA channel that we are using.
   * @param[in] interruptFlags If there is an observer then these are the interrupts that we are observing.
   */

  inline Dma::Dma(DMA_PeripheralType *peripheralAddress,uint32_t channelNumber,uint32_t completeFlag,uint32_t halfCompleteFlag,uint32_t errorFlag) {

    // save channel and get peripheral index

    _peripheralAddress=peripheralAddress;
    _completeFlag=completeFlag;
    _halfCompleteFlag=halfCompleteFlag;
    _errorFlag=errorFlag;
    _channelNumber=channelNumber;
  }


  /**
   * Check for complete
   * @return
   */

  inline bool Dma::isComplete() const {

    if(DMA_GetFlagStatus(_peripheralAddress,_completeFlag)) {
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

    if(DMA_GetFlagStatus(_peripheralAddress,_halfCompleteFlag)) {
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

    if(DMA_GetFlagStatus(_peripheralAddress,_errorFlag)) {
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

    while(!DMA_GetFlagStatus(_peripheralAddress,_completeFlag)) {
      if(DMA_GetFlagStatus(_peripheralAddress,_errorFlag)) {
        DMA_ClearFlag(_peripheralAddress,_errorFlag);
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
    DMA_ClearFlag(_peripheralAddress,_completeFlag);
  }


  /**
   * Clear the complete flag. You need to do this manually if you trigger more DMA transfers
   * from within a DMA interrupt handler
   */

  inline void Dma::clearHalfCompleteFlag() const {
    DMA_ClearFlag(_peripheralAddress,_halfCompleteFlag);
  }


  /**
   * Clear the error flag
   */

  inline void Dma::clearErrorFlag() const {
    DMA_ClearFlag(_peripheralAddress,_errorFlag);
  }


  /**
   * Peripheral address cast operator
   */

  inline Dma::operator DMA_PeripheralType *() {
    return _peripheralAddress;
  }


  /**
   * Get the channel number
   * @return
   */

  inline uint32_t Dma::getChannelNumber() const {
    return _channelNumber;
  }
}
