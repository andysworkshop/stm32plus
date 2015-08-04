/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Base class for all feature classes
   */

  class InternalFlashFeatureBase {

    protected:
      InternalFlashPeripheral& _flashPeripheral;

    public:
      InternalFlashFeatureBase(InternalFlashPeripheral& peripheral);
  };


  /**
   * Constructor
   * @param peripheral reference to the internal flash peripheral
   */

  inline InternalFlashFeatureBase::InternalFlashFeatureBase(InternalFlashPeripheral& peripheral)
    : _flashPeripheral(peripheral) {
  }
}
