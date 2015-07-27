/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * This class provides access to flash pages to use as generic data storage locations.
   */

  class InternalFlashDataStorage {

    protected:
      InternalFlashPeripheral& _flashPeripheral;

    public:
      InternalFlashDataStorage(InternalFlashPeripheral& flashPeripheral);
  };


  /**
   * Constructor
   * @param flashPeripheral The flash peripheral reference to use for the read/erase/program operations
   */

  inline InternalFlashDataStorage::InternalFlashDataStorage(InternalFlashPeripheral& flashPeripheral)
    : _flashPeripheral(flashPeripheral) {
  }
}
