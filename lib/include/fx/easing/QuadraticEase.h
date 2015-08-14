/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fx {

    /**
     * @brief quadratic equation based easing.
     * The acceleration of motion for a Quad easing equation is
     * slower than for a Cubic or Quart easing equation.
     */

    template<class TDataType>
    class QuadraticEaseT : public EasingBaseT<TDataType> {

      public:
        virtual ~QuadraticEaseT() {}

        virtual TDataType easeIn(TDataType time) const override;
        virtual TDataType easeOut(TDataType time) const override;
        virtual TDataType easeInOut(TDataType time) const override;
    };


    /**
     * Compatibility typedef
     */

    typedef QuadraticEaseT<float> QuadraticEase;


    /**
     * Ease in
     * starts motion from a zero velocity, and then accelerates motion as it executes.
     * @param time the current animation time
     * @return the position at the time
     */

    template<class TDataType>
    inline TDataType QuadraticEaseT<TDataType>::easeIn(TDataType time) const {
      time/=this->_duration;
      return this->_change * time * time;
    }


    /**
     * starts motion fast, and then decelerates motion to
     * a zero velocity as it executes
     * @param time the current animation time
     * @return the position at the time
     */

    template<class TDataType>
    inline TDataType QuadraticEaseT<TDataType>::easeOut(TDataType time) const {
      time/=this->_duration;
      return -this->_change * time * (time - 2);
    }


    /**
     * Combines the motion of the easeIn and easeOut methods to
     * to start the motion from a zero velocity, accelerate motion,
     * then decelerate to a zero velocity
     * @param time the current animation time
     * @return the position at the time
     */

    template<class TDataType>
    inline TDataType QuadraticEaseT<TDataType>::easeInOut(TDataType time) const {
      time/=this->_duration / 2;

      if(time < 1)
        return this->_change / 2 * time * time;

      time--;
      return -this->_change / 2 * (time * (time - 2) - 1);
    }
  }
}
