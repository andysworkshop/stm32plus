/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Event signature for the interval callback
     */

    DECLARE_EVENT_SIGNATURE(NetworkIntervalTick,void (uint32_t& interval));


    /**
     * Tick data passed to the callback
     */

    struct NetworkIntervalTickData {

      uint32_t& interval;                 ///< tick interval - can be modified by the caller
      uint32_t timeNow;                   ///< the current time

      NetworkIntervalTickData(uint32_t& i,uint32_t now)
        : interval(i),
          timeNow(now) {
      }
    };


    /**
     * Many of the caches and algorithms within the stack have timeouts or other
     * course-grained thresholds. This class provides the ability to subscribe to an
     * event that will call you after N seconds. The finest granularity is 1 second.
     */

    class NetworkIntervalTicker {

      public:
        typedef wink::slot<void (NetworkIntervalTickData&)> TickIntervalSlotType;

        /**
         * Parameters class
         */

        struct Parameters {
          RtcSecondInterruptFeature *base_rtc;            ///< Cannot be nullptr, an RTC is required
        };

      protected:


        /**
         * Subscriber information structure.
         */

        struct SubscriberInfo {
          uint32_t nextCall;
          uint32_t interval;
          TickIntervalSlotType delegate;
        };

        RtcBase *_rtc;
        RtcSecondInterruptFeature *_rtcInterruptFeature;
        std::slist<SubscriberInfo> _subscribers;
        bool _ready;

      protected:
        void onTickF4(uint8_t extiNumber);      ///< The raw per-second ticker
        void onTick();

      public:
        bool initialise(Parameters& params);
        bool startup();

        void subscribeIntervalTicks(uint32_t interval,const TickIntervalSlotType& delegate);
        void updateIntervalTickSubscription(uint32_t interval,const TickIntervalSlotType& delegate);

        const RtcBase& getRtc() const;
        RtcSecondInterruptFeature& getRtcSecondInterruptFeature() const;
    };


    /**
     * Initialise the class
     * @param rtcInterruptFeature The RTC's interrupt feature
     * @return true if it worked
     */

    inline bool NetworkIntervalTicker::initialise(Parameters& params) {

      // not started up

      _ready=false;

      // remember parameters

      _rtcInterruptFeature=params.base_rtc;
      _rtc=&static_cast<RtcBase&>(*_rtcInterruptFeature);

      // subscribe to the interrupts. this is a bit different between the F1 and F4 because
      // the RTC on those devices is so different.

#if defined(STM32PLUS_F4)
      _rtcInterruptFeature->ExtiInterruptEventSender.insertSubscriber(ExtiInterruptEventSourceSlot::bind(this,&NetworkIntervalTicker::onTickF4));
#elif defined(STM32PLUS_F1_CL_E)
      _rtcInterruptFeature->RtcSecondInterruptEventSender.insertSubscriber(RtcSecondInterruptEventSourceSlot::bind(this,&NetworkIntervalTicker::onTick));
#else
      #error Unsupported MCU
#endif

      return true;
    }


    /**
     * Startup the class
     * @return true if it worked
     */

    inline bool NetworkIntervalTicker::startup() {

      // start the second interrupt

      _ready=true;
      _rtcInterruptFeature->enableSecondInterrupt();

      return true;
    }


    /**
     * Get the RTC base class
     * @return The RTC base
     */

    inline const RtcBase& NetworkIntervalTicker::getRtc() const {
      return *_rtc;
    }


    /**
     * Get the RTC second interrupt feature class
     * @return The feature class
     */

    inline RtcSecondInterruptFeature& NetworkIntervalTicker::getRtcSecondInterruptFeature() const {
      return *_rtcInterruptFeature;
    }


    /**
     * Subscribe to ticks by interval. The first callback will be after the first interval period and then at
     * subsequent interval periods ad-infinitum unless modified by the caller. Must not be called from IRQ code
     * because the list structure is modified here. Should only be called during module initialise() so that the
     * list remains stable during operation.
     * @param interval The interval in seconds. An interval of zero creates this callback initially disabled.
     * @param delegate The delegate to call back on
     */

    inline void NetworkIntervalTicker::subscribeIntervalTicks(uint32_t interval,const TickIntervalSlotType& delegate) {

      SubscriberInfo si;

      // set up the subscriber info and add to the list

      si.delegate=delegate;
      si.interval=interval;

      if(interval)
        si.nextCall=interval+_rtc->getTick();
      else
        si.nextCall=0;

      _subscribers.push_front(si);
    }


    /**
     * Update an existing subscription with a new next-tick interval.
     * @param interval The new interval, which may be zero to disable ticking.
     * @param delegate The delegate to call back (this is used as the comparator when searching for the existing subscription)
     */

    inline void NetworkIntervalTicker::updateIntervalTickSubscription(uint32_t interval,const TickIntervalSlotType& delegate) {

      for(auto it=_subscribers.begin();it!=_subscribers.end();it++) {

        if(it->delegate==delegate) {

          // found it, update values

          it->interval=interval;

          if(interval)
            it->nextCall=interval+_rtc->getTick();

          return;
        }
      }
    }


    /**
     * The raw per-second ticker from the RTC. This is IRQ code.
     */

    inline void NetworkIntervalTicker::onTickF4(uint8_t /* extiNumber */) {
      onTick();
    }

    inline void NetworkIntervalTicker::onTick() {

      uint32_t now;

      // return immediately if startup() has not yet happened

      if(!_ready)
        return;

      // get the current time

      now=_rtc->getTick();

      for(auto it=_subscribers.begin();it!=_subscribers.end();it++) {

        // need to call?

        if(it->interval && now>=it->nextCall) {

          // call it

          NetworkIntervalTickData nitd(it->interval,now);

          it->delegate(nitd);

          // this is when it'll tick next

          if(it->interval)
            it->nextCall=now+it->interval;
        }
      }
    }
  }
}
