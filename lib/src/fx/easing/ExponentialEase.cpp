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

    double ExponentialEase::easeIn(double time_) const {
      return time_ == 0 ? 0 : _change * pow(2,10 * (time_ / _duration - 1));
    }

    /*
     * Ease out
     */

    double ExponentialEase::easeOut(double time_) const {
      return time_ == _duration ? _change : _change * (-pow(2,-10 * time_ / _duration) + 1);
    }

    /*
     * Ease in and out
     */

    double ExponentialEase::easeInOut(double time_) const {
      if(time_ == 0)
        return 0;

      if(time_ == _duration)
        return _change;

      time_/=_duration / 2;

      if(time_ < 1)
        return _change / 2 * pow(2,10 * (time_ - 1));

      time_--;
      return _change / 2 * (-pow(2,-10 * time_) + 2);
    }
  }
}
