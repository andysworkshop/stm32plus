/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * The null SdCardFeatures are used to provide default template types for
   * SdCardFeaturees that allow a variable number of template-based composition of
   * features where those features must also be base SdCardFeatures. e.g:
   *
   * template<typename T=NullSdCardFeature1,typename U=NullSdCardFeature2>
   * SdCardFeature MySdCardFeature : public T, public U { ... };
   */

  struct NullSdCardFeature0 { NullSdCardFeature0(SdCardBase&) {} };
  struct NullSdCardFeature1 { NullSdCardFeature1(SdCardBase&) {} };
  struct NullSdCardFeature2 { NullSdCardFeature2(SdCardBase&) {} };
  struct NullSdCardFeature3 { NullSdCardFeature3(SdCardBase&) {} };
  struct NullSdCardFeature4 { NullSdCardFeature4(SdCardBase&) {} };
  struct NullSdCardFeature5 { NullSdCardFeature5(SdCardBase&) {} };
  struct NullSdCardFeature6 { NullSdCardFeature6(SdCardBase&) {} };
  struct NullSdCardFeature7 { NullSdCardFeature7(SdCardBase&) {} };
  struct NullSdCardFeature8 { NullSdCardFeature8(SdCardBase&) {} };
  struct NullSdCardFeature9 { NullSdCardFeature9(SdCardBase&) {} };
}
