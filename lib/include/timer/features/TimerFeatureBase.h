/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Base class for timer features
   */

  class TimerFeatureBase {
    protected:
      Timer& _timer;

    public:
      TimerFeatureBase(Timer& timer);
      operator Timer&();
  };


  /**
   * Constructor
   * @param timer The base timer class reference
   */

  inline TimerFeatureBase::TimerFeatureBase(Timer& timer)
    : _timer(timer) {
  }


  /**
   * Cast to Timer reference
   */

  inline TimerFeatureBase::operator Timer&() {
    return _timer;
  }
}
