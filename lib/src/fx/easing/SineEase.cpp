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

    float SineEase::easeIn(float time) const {
      return -_change * cos(time / _duration * (float)M_PI_2) + _change;
    }

    /*
     * Ease out
     */

    float SineEase::easeOut(float time) const {
      return _change * sin(time / _duration * (float)M_PI_2);
    }

    /*
     * Ease in and out
     */

    float SineEase::easeInOut(float time) const {
      return -_change / 2 * (cos((float)M_PI * time / _duration) - 1);
    }
  }
}
