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
     * Constructor
     */

    BackEase::BackEase() {
      // set a sensible default value for the overshoot
      _overshoot=1.70158;
    }

    /*
     * Set the overshoot value
     */

    void BackEase::setOvershoot(double overshoot_) {
      _overshoot=overshoot_;
    }

    /*
     * Ease in
     */

    double BackEase::easeIn(double time_) const {
      time_/=_duration;
      return _change * time_ * time_ * ((_overshoot + 1) * time_ - _overshoot);
    }

    /*
     * Ease out
     */

    double BackEase::easeOut(double time_) const {
      time_=time_ / _duration - 1;
      return _change * (time_ * time_ * ((_overshoot + 1) * time_ + _overshoot) + 1);
    }

    /*
     * Ease in and out
     */

    double BackEase::easeInOut(double time_) const {
      double overshoot;

      overshoot=_overshoot * 1.525;
      time_/=_duration / 2;

      if(time_ < 1)
        return _change / 2 * (time_ * time_ * ((overshoot + 1) * time_ - overshoot));

      time_-=2;
      return _change / 2 * (time_ * time_ * ((overshoot + 1) * time_ + overshoot) + 2);
    }
  }
}
