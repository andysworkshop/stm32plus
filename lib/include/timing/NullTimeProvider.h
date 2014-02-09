/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @brief null time provider, always returns zero
   */

  class NullTimeProvider : public TimeProvider {

    public:
      /**
       * Virtual destructor
       */

      virtual ~NullTimeProvider() {}

      /**
       * Get the current time. For this Null provider it is always zero.
       * @return zero.
       */

      virtual time_t getTime() const override {
        return static_cast<time_t> (0);
      }
  };
}
