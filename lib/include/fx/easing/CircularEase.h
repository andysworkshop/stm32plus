/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fx {


    /**
     * Circular easing
     */

    template<typename TDataType>
    class CircularEaseT : public EasingBaseT<TDataType> {

      public:
        virtual ~CircularEaseT() {}

        virtual TDataType easeIn(TDataType time) const override;
        virtual TDataType easeOut(TDataType time) const override;
        virtual TDataType easeInOut(TDataType time) const override;
    };


    /**
     * Compatibility typedef
     */

    typedef CircularEaseT<float> CircularEase;


    /**
     * starts motion slowly, and then accelerates motion as it executes
     * @param time the current animation time
     * @return the position at the time
     */

    template<typename TDataType>
    inline TDataType CircularEaseT<TDataType>::easeIn(TDataType time) const {

      time/=this->_duration;
      return -this->_change * (sqrt(1 - time * time) - 1);
    }


    /**
     * starts motion fast, and then decelerates motion as it executes.
     * @param time the current animation time
     * @return the position at the time
     */

    template<typename TDataType>
    inline TDataType CircularEaseT<TDataType>::easeOut(TDataType time) const {

      time=time / this->_duration - 1;
      return this->_change * sqrt(1 - time * time);
    }


    /**
     * combines the motion of the easeIn and easeOut methods to start the motion slowly, accelerate
     * motion, then decelerate.
     * @param time the current animation time
     * @return the position at the time
     */

    template<typename TDataType>
    inline TDataType CircularEaseT<TDataType>::easeInOut(TDataType time) const {

      time/=this->_duration / 2;

      if(time < 1)
        return -this->_change / 2 * (sqrt(1 - time * time) - 1);

      time-=2;
      return this->_change / 2 * (sqrt(1 - time * time) + 1);
    }
  }
}
