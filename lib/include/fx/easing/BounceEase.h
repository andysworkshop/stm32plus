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

    template<class TDataType>
    class BounceEaseT : public EasingBaseT<TDataType> {

      public:
        virtual ~BounceEaseT() {}

        virtual TDataType easeIn(TDataType time) const override;
        virtual TDataType easeOut(TDataType time) const override;
        virtual TDataType easeInOut(TDataType time) const override;
    };


    /**
     * Compatibility typedef
     */

    typedef BounceEaseT<float> BounceEase;


    /**
     * starts the bounce motion slowly, then accelerates motion as it executes
     * @param time the current animation time
     * @return the position at the time
     */

    template<class TDataType>
    inline TDataType BounceEaseT<TDataType>::easeIn(TDataType time) const {
      return this->_change - easeOut(this->_duration - time);
    }


    /**
     * starts the bounce motion fast, and then decelerates motion as it executes.
     * @param time the current animation time
     * @return the position at the time
     */

    template<class TDataType>
    inline TDataType BounceEaseT<TDataType>::easeOut(TDataType time) const {
      time/=this->_duration;

      if(time < (1 / 2.75f))
        return this->_change * (7.5625f * time * time);

      if(time < (2 / 2.75f)) {
        time-=1.5f / 2.75f;
        return this->_change * (7.5625f * time * time + 0.75f);
      }

      if(time < (2.5f / 2.75f)) {
        time-=2.25f / 2.75f;
        return this->_change * (7.5625f * time * time + 0.9375f);
      }

      time-=2.625f / 2.75f;
      return this->_change * (7.5625f * time * time + 0.984375f);
    }


    /**
     * combines the motion of the easeIn and easeOut methods to start the bounce motion slowly,
     * accelerate motion, then decelerate.
     * @param time the current animation time
     * @return the position at the time
     */

    template<class TDataType>
    inline TDataType BounceEaseT<TDataType>::easeInOut(TDataType time) const {
      if(time < this->_duration / 2)
        return easeIn(time * 2) * 0.5f;
      else
        return easeOut(time * 2 - this->_duration) * 0.5f + this->_change * 0.5f;
    }
  }
}
