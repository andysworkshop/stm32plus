/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/fx.h"


namespace stm32plus {
  namespace fx {

    /*
     * Constructor
     */

    ElasticEase::ElasticEase() {
      // set amplitude and period to zero to trigger use of defaults

      _amplitude=_period=0;
    }

    /*
     * Ease in
     */

    float ElasticEase::easeIn(float time) const {
      float p,a,s;

      if(time == 0)
        return 0;

      time/=_duration;

      if(time == 1)
        return _change;

      if(_period == 0)
        p=_duration * 0.3f;
      else
        p=_period;

      a=_amplitude;
      if(a == 0 || a < fabs(_change)) {
        a=_change;
        s=p / 4;
      } else
        s=p / (2 * (float)M_PI) * asin(_change / a);

      time-=1;
      return -(a * pow(2,10 * time) * sin((time * _duration - s) * (2 * (float)M_PI) / p));
    }

    /*
     * Ease out
     */

    float ElasticEase::easeOut(float time) const {
      float p,a,s;

      if(time == 0)
        return 0;

      time/=_duration;
      if(time == 1)
        return _change;

      if(_period == 0)
        p=_duration * 0.3f;
      else
        p=_period;

      a=_amplitude;
      if(a == 0 || a < fabs(_change)) {
        a=_change;
        s=p / 4;
      } else
        s=p / (2 * (float)M_PI) * asin(_change / a);

      return a * pow(2,-10 * time) * sin((time * _duration - s) * (2 * (float)M_PI) / p) + _change;
    }

    /*
     * Ease in/out
     */

    float ElasticEase::easeInOut(float time) const {
      float p,a,s;

      if(time == 0)
        return 0;

      time/=_duration / 2;
      if(time == 2)
        return _change;

      if(_period == 0)
        p=_duration * (0.3f * 1.5f);
      else
        p=_period;

      a=_amplitude;
      if(a == 0 || a < fabs(_change)) {
        a=_change;
        s=p / 4;
      } else
        s=p / (2 * (float)M_PI) * asin(_change / a);

      if(time < 1) {
        time-=1;
        return -0.5f * (a * pow(2,10 * time) * sin((time * _duration - s) * (2 * (float)M_PI) / p));
      }

      time-=1;
      return a * pow(2,-10 * time) * sin((time * _duration - s) * (2 * (float)M_PI) / p) * 0.5f + _change;
    }

    /*
     * Set the period
     */

    void ElasticEase::setPeriod(float period_) {
      _period=period_;
    }

    /*
     * Set the amplitude
     */

    void ElasticEase::setAmplitude(float amplitude_) {
      _amplitude=amplitude_;
    }
  }
}
