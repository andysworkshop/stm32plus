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

    float ExponentialEase::easeIn(float time) const {
      return time == 0 ? 0 : _change * pow(2,10 * (time / _duration - 1));
    }

    /*
     * Ease out
     */

    float ExponentialEase::easeOut(float time) const {
      return time == _duration ? _change : _change * (-pow(2,-10 * time / _duration) + 1);
    }

    /*
     * Ease in and out
     */

    float ExponentialEase::easeInOut(float time) const {
      if(time == 0)
        return 0;

      if(time == _duration)
        return _change;

      time/=_duration / 2;

      if(time < 1)
        return _change / 2 * pow(2,10 * (time - 1));

      time--;
      return _change / 2 * (-pow(2,-10 * time) + 2);
    }
  }
}
