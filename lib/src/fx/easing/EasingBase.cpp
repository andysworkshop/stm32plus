/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/fx.h"


namespace stm32plus {
  namespace fx {

    /**
     * Default constructor
     */

    EasingBase::EasingBase() {
      _change=0;
    }

    /**
     * Set the duration
     * @param[in] duration The duration
     */

    void EasingBase::setDuration(float duration) {
      _duration=duration;
    }

    /**
     * Set the total change in position
     * @param[in] totalChangeInPosition The total change in position.
     */

    void EasingBase::setTotalChangeInPosition(float totalChangeInPosition) {
      _change=totalChangeInPosition;
    }
  }
}
