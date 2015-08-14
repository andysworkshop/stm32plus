/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fx {

    /**
     * Elastic easing function. The motion is defined by
     * an exponentially decaying sine wave.
     */

    template<class TDataType>
    class ElasticEaseT : public EasingBaseT<TDataType> {

      protected:
        TDataType _period;
        TDataType _amplitude;

      public:
        ElasticEaseT();
        ElasticEaseT(TDataType period,TDataType amplitude);
        virtual ~ElasticEaseT() {}

        virtual TDataType easeIn(TDataType time) const override;
        virtual TDataType easeOut(TDataType time) const override;
        virtual TDataType easeInOut(TDataType time) const override;

        void setPeriod(TDataType period);
        void setAmplitude(TDataType amplitude);

        TDataType getPeriod() const;
        TDataType getAmplitude() const;
    };


    /**
     * Compatibility typedef
     */

    typedef ElasticEaseT<float> ElasticEase;


    /**
     * constructor - sets zero values
     */

    template<class TDataType>
    inline ElasticEaseT<TDataType>::ElasticEaseT() {
      _amplitude=_period=0;
    }


    /**
     * constructor - sets values from parameters
     * @param period the period
     * @param amplitude the amplitude
     */

    template<class TDataType>
    inline ElasticEaseT<TDataType>::ElasticEaseT(TDataType period,TDataType amplitude)
      : _amplitude(amplitude),
        _period(period) {
    }


    /**
     * Starts motion slowly, and then accelerates motion as it executes.
     * @param time the current animation time
     * @return the position at the time
     */

    template<class TDataType>
    inline TDataType ElasticEaseT<TDataType>::easeIn(TDataType time) const {
      TDataType p,a,s;

      if(time == 0)
        return 0;

      time/=this->_duration;

      if(time == 1)
        return this->_change;

      if(_period == 0)
        p=this->_duration * 0.3f;
      else
        p=_period;

      a=_amplitude;
      if(a == 0 || a < fabs(this->_change)) {
        a=this->_change;
        s=p / 4;
      } else
        s=p / (2 * (TDataType)M_PI) * asin(this->_change / a);

      time-=1;
      return -(a * pow(2,10 * time) * sin((time * this->_duration - s) * (2 * (TDataType)M_PI) / p));
    }


    /**
     * Starts motion fast, and then decelerates motion as it executes
     * @param time the current animation time
     * @return the position at the time
     */

    template<class TDataType>
    inline TDataType ElasticEaseT<TDataType>::easeOut(TDataType time) const {
      TDataType p,a,s;

      if(time == 0)
        return 0;

      time/=this->_duration;
      if(time == 1)
        return this->_change;

      if(_period == 0)
        p=this->_duration * 0.3f;
      else
        p=_period;

      a=_amplitude;
      if(a == 0 || a < fabs(this->_change)) {
        a=this->_change;
        s=p / 4;
      } else
        s=p / (2 * (TDataType)M_PI) * asin(this->_change / a);

      return a * pow(2,-10 * time) * sin((time * this->_duration - s) * (2 * (TDataType)M_PI) / p) + this->_change;
    }


    /**
     * combines the motion of the easeIn and easeOut methods
     * to start the motion slowly, accelerate motion, then decelerate
     * @param time the current animation time
     * @return the position at the time
     */

    template<class TDataType>
    inline TDataType ElasticEaseT<TDataType>::easeInOut(TDataType time) const {
      TDataType p,a,s;

      if(time == 0)
        return 0;

      time/=this->_duration / 2;
      if(time == 2)
        return this->_change;

      if(_period == 0)
        p=this->_duration * (0.3f * 1.5f);
      else
        p=_period;

      a=_amplitude;
      if(a == 0 || a < fabs(this->_change)) {
        a=this->_change;
        s=p / 4;
      } else
        s=p / (2 * (TDataType)M_PI) * asin(this->_change / a);

      if(time < 1) {
        time-=1;
        return -0.5f * (a * pow(2,10 * time) * sin((time * this->_duration - s) * (2 * (TDataType)M_PI) / p));
      }

      time-=1;
      return a * pow(2,-10 * time) * sin((time * this->_duration - s) * (2 * (TDataType)M_PI) / p) * 0.5f + this->_change;
    }


    /**
     * Set the period
     */

    template<class TDataType>
    inline void ElasticEaseT<TDataType>::setPeriod(TDataType period) {
      _period=period;
    }


    /**
     * Set the amplitude
     */

    template<class TDataType>
    inline void ElasticEaseT<TDataType>::setAmplitude(TDataType amplitude) {
      _amplitude=amplitude;
    }


    /**
     * Get the period
     * @return the period
     */

    template<class TDataType>
    inline TDataType ElasticEaseT<TDataType>::getPeriod() const {
      return _period;
    }


    /**
     * Get the amplitude
     * @return the amplitude
     */

    template<class TDataType>
    TDataType ElasticEaseT<TDataType>::getAmplitude() const {
      return _amplitude;
    }
  }
}
