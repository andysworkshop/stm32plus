/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Base class for sdcard features
   */

  class SdCardFeatureBase {

    protected:
      SdCardBase& _sdcard;

    public:
      SdCardFeatureBase(SdCardBase& sdcard);
      operator SdCardBase&();
  };


  /**
   * Constructor
   * @param SdCard
   */

  inline SdCardFeatureBase::SdCardFeatureBase(SdCardBase& sdcard)
    : _sdcard(sdcard) {
  }


  /**
   * Cast to SdCard reference
   */

  inline SdCardFeatureBase::operator SdCardBase&() {
    return _sdcard;
  }
}
