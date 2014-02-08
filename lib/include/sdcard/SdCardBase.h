/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Base class for SD card classes
   */

  class SdCardBase {

    public:
      SdCardBase();
  };


  /**
   * Constructor
   */

  inline SdCardBase::SdCardBase() {

    // this constructor is called before the features
  }
}

