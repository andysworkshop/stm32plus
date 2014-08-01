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

    float CircularEase::easeIn(float time) const {
      time/=_duration;

      return -_change * (sqrt(1 - time * time) - 1);
    }

    /*
     * Ease out
     */

    float CircularEase::easeOut(float time) const {
      time=time / _duration - 1;
      return _change * sqrt(1 - time * time);
    }

    /*
     * Ease in and out
     */

    float CircularEase::easeInOut(float time) const {
      time/=_duration / 2;

      if(time < 1)
        return -_change / 2 * (sqrt(1 - time * time) - 1);

      time-=2;
      return _change / 2 * (sqrt(1 - time * time) + 1);
    }

  }
}
