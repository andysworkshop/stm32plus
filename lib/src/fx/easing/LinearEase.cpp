/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/fx.h"


namespace stm32plus {
  namespace fx {

    /*
     * Ease in
     */

    float LinearEase::easeIn(float time) const {
      return _change * time / _duration;
    }

    /*
     * Ease out
     */

    float LinearEase::easeOut(float time) const {
      return easeIn(time);
    }

    /*
     * Ease in and out
     */

    float LinearEase::easeInOut(float time) const {
      return easeIn(time);
    }
  }
}
