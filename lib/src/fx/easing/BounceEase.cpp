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

    double BounceEase::easeIn(double time_) const {
      return _change - easeOut(_duration - time_);
    }

    /*
     * Ease out
     */

    double BounceEase::easeOut(double time_) const {
      time_/=_duration;

      if(time_ < (1 / 2.75))
        return _change * (7.5625 * time_ * time_);

      if(time_ < (2 / 2.75)) {
        time_-=1.5 / 2.75;
        return _change * (7.5625 * time_ * time_ + 0.75);
      }

      if(time_ < (2.5 / 2.75)) {
        time_-=2.25 / 2.75;
        return _change * (7.5625 * time_ * time_ + 0.9375);
      }

      time_-=2.625 / 2.75;
      return _change * (7.5625 * time_ * time_ + 0.984375);
    }

    /*
     * Ease in and out
     */

    double BounceEase::easeInOut(double time_) const {
      if(time_ < _duration / 2)
        return easeIn(time_ * 2) * 0.5;
      else
        return easeOut(time_ * 2 - _duration) * 0.5 + _change * 0.5;
    }
  }
}
