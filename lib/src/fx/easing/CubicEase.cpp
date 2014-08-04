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
     * Starts motion from zero velocity, and then
     * accelerates motion as it executes.
     */

    float CubicEase::easeIn(float time) const {
      time/=_duration;

      return _change * time * time * time;
    }

    /*
     * Starts motion fast, and then decelerates motion
     * to a zero velocity as it executes.
     */

    float CubicEase::easeOut(float time) const {
      time=time / _duration - 1;

      return _change * (time * time * time + 1);
    }

    /*
     * Combines the motion of the easeIn and easeOut methods
     * to start the motion from zero velocity, accelerates motion,
     * then decelerates back to a zero velocity.
     */

    float CubicEase::easeInOut(float time) const {
      time/=_duration / 2;

      if(time < 1)
        return _change / 2 * time * time * time;

      time-=2;
      return _change / 2 * (time * time * time + 2);
    }
  }
}
