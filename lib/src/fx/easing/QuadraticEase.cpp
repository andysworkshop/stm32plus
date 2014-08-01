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

    float QuadraticEase::easeIn(float time) const {
      time/=_duration;
      return _change * time * time;
    }

    /*
     * Ease out
     */

    float QuadraticEase::easeOut(float time) const {
      time/=_duration;
      return -_change * time * (time - 2);
    }

    /*
     * Ease in and out
     */

    float QuadraticEase::easeInOut(float time) const {
      time/=_duration / 2;

      if(time < 1)
        return _change / 2 * time * time;

      time--;
      return -_change / 2 * (time * (time - 2) - 1);
    }
  }
}
