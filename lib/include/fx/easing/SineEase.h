/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fx {

    /**
     * @brief The motion is defined by a sine wave.
     */

    template<class TDataType>
    class SineEaseT : public EasingBaseT<TDataType> {
      public:
        virtual ~SineEaseT() {}

        virtual TDataType easeIn(TDataType time) const override;
        virtual TDataType easeOut(TDataType time) const override;
        virtual TDataType easeInOut(TDataType time) const override;
    };


    /**
     * Compatibility typedef
     */

    typedef SineEaseT<float> SineEase;


    /**
     * starts motion from a zero velocity, and then accelerates motion as it executes.
     * @param time the current animation time
     * @return the position at the time
     */

    template<class TDataType>
    inline TDataType SineEaseT<TDataType>::easeIn(TDataType time) const {
      return -this->_change * cos(time / this->_duration * (TDataType)M_PI_2) + this->_change;
    }


    /**
     * starts motion fast, and then decelerates motion to a zero velocity as it executes
     * @param time the current animation time
     * @return the position at the time
     */

    template<class TDataType>
    inline TDataType SineEaseT<TDataType>::easeOut(TDataType time) const {
      return this->_change * sin(time / this->_duration * (TDataType)M_PI_2);
    }


    /**
     * Combines the motion of the easeIn and easeOut methods to
     * to start the motion from a zero velocity, accelerate motion,
     * then decelerate to a zero velocity
     * @param time the current animation time
     * @return the position at the time
     */

    template<class TDataType>
    inline TDataType SineEaseT<TDataType>::easeInOut(TDataType time) const {
      return -this->_change / 2 * (cos((TDataType)M_PI * time / this->_duration) - 1);
    }
  }
}
