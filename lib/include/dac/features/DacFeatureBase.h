/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Base class for DAC features
   */

  class DacFeatureBase {
    protected:
      Dac& _dac;

    public:
      DacFeatureBase(Dac& dac);
      operator Dac&();
  };


  /**
   * Constructor
   * @param dac
   */

  inline DacFeatureBase::DacFeatureBase(Dac& dac)
    : _dac(dac) {
  }


  /**
   * Cast to Dac reference
   */

  inline DacFeatureBase::operator Dac&() {
    return _dac;
  }
}
