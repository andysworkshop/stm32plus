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


  /*
   * Feature specific to the F0 that allows you to set when the RNXE flag gets set. For compatibility
   * with the F1 and F4 you must set this to QF.
   */

  template<uint16_t TThreshold>
  struct SpiFifoNotifyFeature : public SpiFeatureBase {

    /*
     * Constructor
     */

    SpiFifoNotifyFeature(Spi& spi)
      : SpiFeatureBase(spi) {
      static_assert(TThreshold==SPI_RxFIFOThreshold_HF || TThreshold==SPI_RxFIFOThreshold_QF,"Threshold must be QF or HF");
      SPI_RxFIFOThresholdConfig(spi,TThreshold);
    }
  };


  /*
   * Typedefs for the two possibilities
   */

  typedef SpiFifoNotifyFeature<SPI_RxFIFOThreshold_QF> SpiFifoNotifyQuarterFullFeature;
  typedef SpiFifoNotifyFeature<SPI_RxFIFOThreshold_HF> SpiFifoNotifyHalfFullFeature;
}
