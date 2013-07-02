/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fx {

    /**
     * Elastic easing function. The motion is defined by
     * an exponentially decaying sine wave.
     */

    class ElasticEase : public EasingBase {

      private:
        double _period;
        double _amplitude;

      public:
        /// constructor - sets a default value for the overshoot
        ElasticEase();

        /// Starts motion slowly, and then accelerates motion as it executes.
        virtual double easeIn(double time) const override;

        /// Starts motion fast, and then decelerates motion as it executes
        virtual double easeOut(double time) const override;

        /// combines the motion of the easeIn and easeOut methods
        /// to start the motion slowly, accelerate motion, then decelerate
        virtual double easeInOut(double time) const override;

        /// set the period
        void setPeriod(double period_);

        /// set the amplitude
        void setAmplitude(double amplitude_);
    };
  }
}
