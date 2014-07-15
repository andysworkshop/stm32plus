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

    float QuarticEase::easeIn(float time) const {
      time/=_duration;
      return _change * time * time * time * time;
    }

    /*
     * Ease out
     */

    float QuarticEase::easeOut(float time) const {
      time=time / _duration - 1;
      return -_change * (time * time * time * time - 1);
    }

    /*
     * Ease in and out
     */

    float QuarticEase::easeInOut(float time) const {
      time/=_duration / 2;

      if(time < 1)
        return _change / 2 * time * time * time * time;

      time-=2;
      return -_change / 2 * (time * time * time * time - 2);
    }
  }
}
