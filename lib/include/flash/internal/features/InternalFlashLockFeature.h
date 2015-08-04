/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /*
   * Feature class to provide lock/unlock capability
   */

  class InternalFlashLockFeature : public InternalFlashFeatureBase {

    public:

      /**
       * LockManager class can be declared on the stack to guard an area of code
       * where the flash is to be written and to ensure that it gets locked regardless
       * of the exit point of the guarded code.
       */

      struct LockManager {

        LockManager() {
          FLASH_Unlock();
        }

        ~LockManager() {
          FLASH_Lock();
        }
      };

    public:
      InternalFlashLockFeature(InternalFlashPeripheral& flashPeripheral);

    public:
      void lock() const;
      void unlock() const;
  };


  /**
   * Constructor
   * @param flashPeripheral reference to the peripheral class
   */

  inline InternalFlashLockFeature::InternalFlashLockFeature(InternalFlashPeripheral& flashPeripheral)
    : InternalFlashFeatureBase(flashPeripheral) {
  }


  /**
   * Lock the flash memory
   */

  inline void InternalFlashLockFeature::lock() const {
    FLASH_Lock();
  }


  /**
   * Unlock the flash memory
   */

  inline void InternalFlashLockFeature::unlock() const {
    FLASH_Unlock();
  }
}
