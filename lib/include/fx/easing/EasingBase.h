/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fx {

    /**
     * Base class for easing functions
     */

    class EasingBase {

      protected:
        float _change;
        float _duration;

      public:
        /// default constructor
        EasingBase();
        virtual ~EasingBase() {}

        /**
         * Ease a transition in
         * @param[in] time The time to do the transition.
         */

        virtual float easeIn(float time) const=0;

        /**
         * Ease a transition out.
         * @param[in] time The time to do the transition.
         */

        virtual float easeOut(float time) const=0;

        /**
         * Ease a transition in and out.
         * @param[in] time The time to do the transition.
         */

        virtual float easeInOut(float time) const=0;

        void setDuration(float duration);
        void setTotalChangeInPosition(float totalChangeInPosition);
    };
  }
}
