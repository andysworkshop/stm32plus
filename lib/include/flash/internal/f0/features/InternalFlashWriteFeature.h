/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /*
   * Feature class to provide program/erase operations
   */

  class InternalFlashWriteFeature {

    public:
      InternalFlashWriteFeature();

      void lock() const;
  };


  /**
   * Constructor: unlock the flash memory
   */

  inline InternalFlashWriteFeature::InternalFlashWriteFeature() {
    FLASH_Unlock();
  }


  /**
   * Lock the flash memory
   */

  inline void InternalFlashWriteFeature::lock() const {
    FLASH_Lock();
  }
}
