/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/fx.h"


namespace stm32plus {
  namespace fx {

    /*
     * Ease in
     */

    double SineEase::easeIn(double time_) const {
      return -_change * cos(time_ / _duration * M_PI_2) + _change;
    }

    /*
     * Ease out
     */

    double SineEase::easeOut(double time_) const {
      return _change * sin(time_ / _duration * M_PI_2);
    }

    /*
     * Ease in and out
     */

    double SineEase::easeInOut(double time_) const {
      return -_change / 2 * (cos(M_PI * time_ / _duration) - 1);
    }
  }
}
