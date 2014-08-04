/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fx {

    /**
     * Bouncing easing function
     */

    class BounceEase : public EasingBase {

      public:
        /// starts the bounce motion slowly,
        /// then accelerates motion as it executes
        virtual float easeIn(float time) const override;

        /// starts the bounce motion fast,
        /// and then decelerates motion as it executes.
        virtual float easeOut(float time) const override;

        /// combines the motion of the easeIn and easeOut
        /// methods to start the bounce motion slowly,
        /// accelerate motion, then decelerate.
        virtual float easeInOut(float time) const override;
    };
  }
}
