/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Base class for SPI features
   */

  class SpiFeatureBase {
    protected:
      Spi& _spi;

    public:
      SpiFeatureBase(Spi& spi);
      operator Spi&();
  };


  /**
   * Constructor
   * @param spi
   */

  inline SpiFeatureBase::SpiFeatureBase(Spi& spi)
    : _spi(spi) {
  }


  /**
   * Cast to Spi reference
   */

  inline SpiFeatureBase::operator Spi&() {
    return _spi;
  }
}
