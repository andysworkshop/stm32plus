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

      /**
       * LockManager class can be declared on the stack to guard an area of code
       * where the flash is to be written and to ensure that it gets locked regardless
       * of the exit point of the guarded code.
       */

      struct LockManager {

        LockManager() {
          FLASH_Lock();
        }

        ~LockManager() {
          FLASH_Unlock();
        }
      };

    public:
      void lock() const;
      void unlock() const;
  };


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
