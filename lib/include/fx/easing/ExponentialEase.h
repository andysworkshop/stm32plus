/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fx {

    /**
     * @brief The motion is defined by an exponentially decaying sine wave
     */

    template<class TDataType>
    class ExponentialEaseT : public EasingBaseT<TDataType> {

      public:
        virtual ~ExponentialEaseT() {}

        virtual TDataType easeIn(TDataType time) const override;
        virtual TDataType easeOut(TDataType time) const override;
        virtual TDataType easeInOut(TDataType time) const override;
    };


    /**
     * Compatibility typedef
     */

    typedef ExponentialEaseT<float> ExponentialEase;


    /**
     * starts motion slowly, and then accelerates motion as it executes
     * @param time the current animation time
     * @return the position at the time
     */

    template<class TDataType>
    inline TDataType ExponentialEaseT<TDataType>::easeIn(TDataType time) const {
      return time == 0 ? 0 : this->_change * pow(2,10 * (time / this->_duration - 1));
    }


    /**
     * starts motion fast, and then decelerates motion as it executes
     * @param time the current animation time
     * @return the position at the time
     */

    template<class TDataType>
    inline TDataType ExponentialEaseT<TDataType>::easeOut(TDataType time) const {
      return time == this->_duration ? this->_change : this->_change * (-pow(2,-10 * time / this->_duration) + 1);
    }


    /**
     * combines the motion of the easeIn and easeOut methods to start the
     * motion slowly, accelerate motion, then decelerate
     * @param time the current animation time
     * @return the position at the time
     */

    template<class TDataType>
    inline TDataType ExponentialEaseT<TDataType>::easeInOut(TDataType time) const {
      if(time == 0)
        return 0;

      if(time == this->_duration)
        return this->_change;

      time/=this->_duration / 2;

      if(time < 1)
        return this->_change / 2 * pow(2,10 * (time - 1));

      time--;
      return this->_change / 2 * (-pow(2,-10 * time) + 2);
    }
  }
}
