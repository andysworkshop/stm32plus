/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @brief Base class for classes that are able to provide a time.
   * The time is required for file system operations that need to create or write to files.
   */

  class TimeProvider {

    public:

      virtual ~TimeProvider() {}

      /**
       * Return the time as a Unix time_t
       * @return The current time.
       */

      virtual time_t getTime() const=0;
  };
}
