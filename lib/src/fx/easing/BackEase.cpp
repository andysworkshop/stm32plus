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
     * Constructor
     */

    BackEase::BackEase() {
      // set a sensible default value for the overshoot
      _overshoot=1.70158;
    }

    /*
     * Set the overshoot value
     */

    void BackEase::setOvershoot(float overshoot_) {
      _overshoot=overshoot_;
    }

    /*
     * Ease in
     */

    float BackEase::easeIn(float time) const {
      time/=_duration;
      return _change * time * time * ((_overshoot + 1) * time - _overshoot);
    }

    /*
     * Ease out
     */

    float BackEase::easeOut(float time) const {
      time=time / _duration - 1;
      return _change * (time * time * ((_overshoot + 1) * time + _overshoot) + 1);
    }

    /*
     * Ease in and out
     */

    float BackEase::easeInOut(float time) const {
      float overshoot;

      overshoot=_overshoot * 1.525;
      time/=_duration / 2;

      if(time < 1)
        return _change / 2 * (time * time * ((overshoot + 1) * time - overshoot));

      time-=2;
      return _change / 2 * (time * time * ((overshoot + 1) * time + overshoot) + 2);
    }
  }
}
