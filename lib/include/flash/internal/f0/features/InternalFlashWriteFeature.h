/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#ifndef STM32PLUS_F0
#error This class can only be used with the STM32F0 series
#endif


namespace stm32plus {


  /*
   * Feature class to provide program/erase operations
   */

  class InternalFlashWriteFeature {

    public:
      InternalFlashWriteFeature();

      void lock() const;
      void unlock() const;
  };


  /**
   * Constructor: unlock the flash memory
   */

  inline InternalFlashWriteFeature::InternalFlashWriteFeature() {
    unlock();
  }


  /**
   * Lock the flash memory
   */

  inline void InternalFlashWriteFeature::lock() const {
    FLASH_Lock();
  }


  /**
   * Unlock the flash memory
   */

  inline void InternalFlashWriteFeature::unlock() const {
    FLASH_Unlock();
  }
}
