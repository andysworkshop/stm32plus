/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {


  /**
   * Convenient extension of the heap monitor to output to a usart
   */

  class UsartHeapMonitor : public HeapMonitor {

    protected:
      UsartPollingOutputStream *_usartOutputStream;

    public:
      UsartHeapMonitor();
      void start(RtcSecondInterruptFeature& rtc,uint32_t frequency,Usart& usart);
  };


  /**
   * Constructor
   */

  inline UsartHeapMonitor::UsartHeapMonitor()
    : _usartOutputStream(nullptr) {
  }


  /**
   * Start it off
   * @param rtc RTC instance
   * @param usart The usart to use
   * @param frequency
   */

  inline void UsartHeapMonitor::start(RtcSecondInterruptFeature& rtc,uint32_t frequency,Usart& usart) {

    if(_usartOutputStream)
      delete _usartOutputStream;

    _usartOutputStream=new UsartPollingOutputStream(usart);
    HeapMonitor::start(rtc,*_usartOutputStream,frequency);
  }
}
