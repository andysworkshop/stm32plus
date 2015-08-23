/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fx {

    /**
     * @brief x^4 based easing.
     * The acceleration of motion for a Quart easing
     * equation is greater than for a Quad or Cubic.
     */

    template<typename TDataType>
    class QuarticEaseT : public EasingBaseT<TDataType> {

      public:
        virtual ~QuarticEaseT() {}

        virtual TDataType easeIn(TDataType time) const override;
        virtual TDataType easeOut(TDataType time) const override;
        virtual TDataType easeInOut(TDataType time) const override;
    };


    /**
     * Compatibility typedef
     */

    typedef QuarticEaseT<float> QuarticEase;


    /**
     * starts motion from a zero velocity, and then accelerates
     * motion as it executes.
     * @param time the current animation time
     * @return the position at the time
     */

    template<typename TDataType>
    inline TDataType QuarticEaseT<TDataType>::easeIn(TDataType time) const {
      time/=this->_duration;
      return this->_change * time * time * time * time;
    }


    /**
     * Ease out
     * @param time the current animation time
     * @return the position at the time
     */

    template<typename TDataType>
    inline TDataType QuarticEaseT<TDataType>::easeOut(TDataType time) const {
      time=time / this->_duration - 1;
      return -this->_change * (time * time * time * time - 1);
    }


    /**
     * Ease in and out
     * @param time the current animation time
     * @return the position at the time
     */

    template<typename TDataType>
    inline TDataType QuarticEaseT<TDataType>::easeInOut(TDataType time) const {
      time/=this->_duration / 2;

      if(time < 1)
        return this->_change / 2 * time * time * time * time;

      time-=2;
      return -this->_change / 2 * (time * time * time * time - 2);
    }
  }
}
