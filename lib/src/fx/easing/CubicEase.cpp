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

    double CubicEase::easeIn(double time_) const {
      time_/=_duration;

      return _change * time_ * time_ * time_;
    }

    /*
     * Starts motion fast, and then decelerates motion
     * to a zero velocity as it executes.
     */

    double CubicEase::easeOut(double time_) const {
      time_=time_ / _duration - 1;

      return _change * (time_ * time_ * time_ + 1);
    }

    /*
     * Combines the motion of the easeIn and easeOut methods
     * to start the motion from zero velocity, accelerates motion,
     * then decelerates back to a zero velocity.
     */

    double CubicEase::easeInOut(double time_) const {
      time_/=_duration / 2;

      if(time_ < 1)
        return _change / 2 * time_ * time_ * time_;

      time_-=2;
      return _change / 2 * (time_ * time_ * time_ + 2);
    }
  }
}
