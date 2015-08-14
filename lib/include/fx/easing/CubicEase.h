/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fx {

    /**
     * @brief x^3 based easing.
     * The acceleration of motion for a Cubic easing
     * equation is greater than for a Quad easing equation.
     */

    template<class TDataType>
    class CubicEaseT : public EasingBaseT<TDataType> {

      public:
        virtual ~CubicEaseT() {}

        virtual TDataType easeIn(TDataType time) const override;
        virtual TDataType easeOut(TDataType time) const override;
        virtual TDataType easeInOut(TDataType time) const override;
    };


    /**
     * Compatibility typedef
     */

    typedef CubicEaseT<float> CubicEase;


    /**
     * Starts the motion by backtracking, then reversing
     * direction and moving toward the target
     * accelerates motion as it executes.
     * @param time the current animation time
     * @return the position at the time
     */

    template<class TDataType>
    inline TDataType CubicEaseT<TDataType>::easeIn(TDataType time) const {
      time/=this->_duration;

      return this->_change * time * time * time;
    }


    /**
     * Starts the motion by moving towards the target, overshooting
     * it slightly, and then reversing direction back toward the target
     * @param time the current animation time
     * @return the position at the time
     */

    template<class TDataType>
    inline TDataType CubicEaseT<TDataType>::easeOut(TDataType time) const {
      time=time / this->_duration - 1;

      return this->_change * (time * time * time + 1);
    }


    /**
     * Combines the motion of the easeIn and easeOut methods to
     * start the motion by backtracking, then reversing direction
     * and moving toward target, overshooting target slightly,
     * reversing direction again, and then moving back toward the target
     * @param time the current animation time
     * @return the position at the time
     */

    template<class TDataType>
    inline TDataType CubicEaseT<TDataType>::easeInOut(TDataType time) const {
      time/=this->_duration / 2;

      if(time < 1)
        return this->_change / 2 * time * time * time;

      time-=2;
      return this->_change / 2 * (time * time * time + 2);
    }
  }
}
