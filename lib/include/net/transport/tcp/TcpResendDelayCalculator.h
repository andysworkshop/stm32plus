/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * State management for the resend algorithm. This implements the algorithm in RFC2988 as
     * best we can here. Round-trip times are used to calculate an adaptive value that defines
     * how long to wait before a packet is considered lost and should be retransmitted for the
     * first time.
     */

    class TcpResendDelayCalculator {

      protected:
        uint16_t _initialDelay;
        uint16_t _maxDelay;

        uint32_t _srtt;
        uint32_t _rttvar;
        uint32_t _timerStart;
        bool _first;

      public:
        bool initialise(uint16_t initialDelay,uint16_t maxDelay);

        void startTimer();
        void stopTimer();

        uint16_t getResendDelay() const;
    };


    /**
     * Initialise the class
     * @param initialDelay The first delay seconds
     * @param maxDelay The maximum delay seconds
     * @return true
     */

    inline bool TcpResendDelayCalculator::initialise(uint16_t initialDelay,uint16_t maxDelay) {
      _initialDelay=initialDelay;
      _maxDelay=maxDelay;
      return true;
    }


    /**
     * Start the RTT timer
     */

    inline void TcpResendDelayCalculator::startTimer() {
      _timerStart=MillisecondTimer::millis();
    }


    /**
     * Stop the RTT timer and calculate the state variables
     */

    inline void TcpResendDelayCalculator::stopTimer() {

      uint32_t r;

      r=MillisecondTimer::difference(_timerStart);

      if(_first) {
        _srtt=r;
        _rttvar=r/2;
        _first=false;
      }
      else {
        _rttvar=((3*_rttvar)+std::abs((int32_t)_srtt-(int32_t)r))/4;
        _srtt=((7*_srtt)+r)/8;
      }
    }


    /**
     * Get the current resend delay, calculated from the state variables
     * @return the current resend delay
     */

    inline uint16_t TcpResendDelayCalculator::getResendDelay() const {

      if(_first)
        return _initialDelay;

      return std::min((uint32_t)_maxDelay,(_srtt+std::max(1ul,4*_rttvar))/1000);
    }
  }
}
