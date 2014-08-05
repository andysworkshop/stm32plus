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

    float BounceEase::easeIn(float time) const {
      return _change - easeOut(_duration - time);
    }

    /*
     * Ease out
     */

    float BounceEase::easeOut(float time) const {
      time/=_duration;

      if(time < (1 / 2.75f))
        return _change * (7.5625f * time * time);

      if(time < (2 / 2.75f)) {
        time-=1.5f / 2.75f;
        return _change * (7.5625f * time * time + 0.75f);
      }

      if(time < (2.5f / 2.75f)) {
        time-=2.25f / 2.75f;
        return _change * (7.5625f * time * time + 0.9375f);
      }

      time-=2.625f / 2.75f;
      return _change * (7.5625f * time * time + 0.984375f);
    }

    /*
     * Ease in and out
     */

    float BounceEase::easeInOut(float time) const {
      if(time < _duration / 2)
        return easeIn(time * 2) * 0.5f;
      else
        return easeOut(time * 2 - _duration) * 0.5f + _change * 0.5f;
    }
  }
}
