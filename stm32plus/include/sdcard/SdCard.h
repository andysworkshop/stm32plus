/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Template feature collection class for the sd-card peripheral.
   */

  template<class TF0=NullSdCardFeature0,class TF1=NullSdCardFeature1,
           class TF2=NullSdCardFeature2,class TF3=NullSdCardFeature3,
           class TF4=NullSdCardFeature4,class TF5=NullSdCardFeature5,
           class TF6=NullSdCardFeature6,class TF7=NullSdCardFeature7,
           class TF8=NullSdCardFeature8,class TF9=NullSdCardFeature9>
  class SdCard : public SdCardBase,
                 public TF0,public TF1,public TF2,public TF3,public TF4,public TF5,public TF6,public TF7,public TF8,public TF9 {

    public:
      SdCard() :
        TF0((SdCardBase&)*this),TF1((SdCardBase&)*this),TF2((SdCardBase&)*this),TF3((SdCardBase&)*this),TF4((SdCardBase&)*this),TF5((SdCardBase&)*this),TF6((SdCardBase&)*this),TF7((SdCardBase&)*this),TF8((SdCardBase&)*this),TF9((SdCardBase&)*this) {
    }
  };
}
