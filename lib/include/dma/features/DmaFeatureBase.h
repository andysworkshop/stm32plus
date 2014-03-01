/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Base class for DMA features
   */

  class DmaFeatureBase {

    public:

      enum TransferType {
        TRANSFER_ONESHOT,
        TRANSFER_CIRCULAR
      };

    protected:
      Dma& _dma;
      DMA_InitTypeDef _init;

    public:
      DmaFeatureBase(Dma& dma);

      operator Dma&();
      operator DMA_InitTypeDef&();

      Dma& getDma();
  };


  /**
   * Constructor
   * @param dma
   */

  inline DmaFeatureBase::DmaFeatureBase(Dma& dma)
    : _dma(dma) {
  }


  /**
   * Cast to Dma reference
   */

  inline DmaFeatureBase::operator Dma&() {
    return _dma;
  }


  /*
   * Alternative to the cast operator
   * @return The DMA object
   */

  inline Dma& DmaFeatureBase::getDma() {
    return _dma;
  }


  /**
   * Cast to DMA_InitTypedef to allow modifications
   * before the transfer is started
   */

  inline DmaFeatureBase::operator DMA_InitTypeDef&() {
    return _init;
  }
}
