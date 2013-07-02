/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fx {

    /**
     * @brief The motion is defined by a sine wave.
     */

    class SineEase : public EasingBase {
      public:
        /// starts motion from a zero velocity, and then accelerates motion as it executes.
        virtual double easeIn(double time) const override;

        /// starts motion fast, and then decelerates motion to
        /// a zero velocity as it executes
        virtual double easeOut(double time) const override;

        /// Combines the motion of the easeIn and easeOut methods to
        /// to start the motion from a zero velocity, accelerate motion,
        /// then decelerate to a zero velocity
        virtual double easeInOut(double time) const override;
    };
  }
}
