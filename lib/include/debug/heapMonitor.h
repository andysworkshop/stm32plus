/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {


  /**
   * The heap monitor class provides a period dump of the state of the heap
   * to an output stream. The RTC is used to provide the interrupt that we
   * use to get the heap state.
   */

  class HeapMonitor {

    protected:
      RtcSecondInterruptFeature *_rtc;
      TextOutputStream  *_os;
      uint32_t _frequency;
      uint32_t _currentTick;

    protected:
    #if defined(STM32PLUS_F4)
      void onTick(uint8_t extiNumber);
    #elif defined(STM32PLUS_F1)
      void onTick(void);
    #else
      #error Unsupported MCU
    #endif

    public:
      HeapMonitor();
      ~HeapMonitor();

      void start(RtcSecondInterruptFeature& rtc,OutputStream& os,uint32_t frequency);
      void stop();
  };


  /**
   * Constructor
   */

  inline HeapMonitor::HeapMonitor()
    : _rtc(nullptr),
      _os(nullptr) {
  }


  /**
   * Destructor, stop ticking
   */

  inline HeapMonitor::~HeapMonitor() {

    stop();

    if(_os)
      delete _os;
  }


  /**
   * start ticking
   * @param rtc Rtc interrupt feature
   * @param os output stream
   * @param frequency how many seconds between statistics
   */

  inline void HeapMonitor::start(RtcSecondInterruptFeature& rtc,OutputStream& os,uint32_t frequency) {

    _rtc=&rtc;
    _os=new TextOutputStream(os);
    _frequency=frequency;
    _currentTick=0;

    #if defined(STM32PLUS_F4)
      rtc.ExtiInterruptEventSender.insertSubscriber(ExtiInterruptEventSourceSlot::bind(this,&HeapMonitor::onTick));
    #elif defined(STM32PLUS_F1)
      rtc.RtcSecondInterruptEventSender.insertSubscriber(RtcSecondInterruptEventSourceSlot::bind(this,&HeapMonitor::onTick));
    #else
      #error Unsupported MCU
    #endif
  }


  /**
   * stop ticking
   */

  inline void HeapMonitor::stop() {

    _currentTick=0;

    #if defined(STM32PLUS_F4)
      _rtc->ExtiInterruptEventSender.removeSubscriber(ExtiInterruptEventSourceSlot::bind(this,&HeapMonitor::onTick));
    #elif defined(STM32PLUS_F1)
      _rtc->RtcSecondInterruptEventSender.removeSubscriber(RtcSecondInterruptEventSourceSlot::bind(this,&HeapMonitor::onTick));
    #else
      #error Unsupported MCU
    #endif
  }


  /**
   * We ticked.
   */

#if defined(STM32PLUS_F4)

  __attribute__((noinline)) inline void HeapMonitor::onTick(uint8_t /* extiNumber */) {

    // check frequency

    if(++_currentTick<_frequency)
      return;

    // get statistics

    struct mallinfo minfo=mallinfo();

    // send to the output stream

    *_os << (uint32_t)minfo.arena << ","
         << (uint32_t)minfo.ordblks << ","
         << (uint32_t)minfo.hblks << ","
         << (uint32_t)minfo.hblkhd << ","
         << (uint32_t)minfo.uordblks << ","
         << (uint32_t)minfo.fordblks << ","
         << (uint32_t)minfo.keepcost << "\r\n";
  }

#elif defined(STM32PLUS_F1)

   __attribute__((noinline)) inline void HeapMonitor::onTick(void) {

    // check frequency

    if(++_currentTick<_frequency)
      return;

    // get statistics

    struct mallinfo minfo=mallinfo();

    // send to the output stream

    *_os << (uint32_t)minfo.arena << ","
         << (uint32_t)minfo.ordblks << ","
         << (uint32_t)minfo.hblks << ","
         << (uint32_t)minfo.hblkhd << ","
         << (uint32_t)minfo.uordblks << ","
         << (uint32_t)minfo.fordblks << ","
         << (uint32_t)minfo.keepcost << "\r\n";
  }

#else
    #error Unsupported MCU
#endif

}
