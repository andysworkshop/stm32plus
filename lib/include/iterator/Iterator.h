/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

/**
 * @brief Iterator base class.
 * @tparam T The type that we are iterating over.
 *
 * Iterators are created pointing into nothing before the first item. A call
 * to next() is required to move to the first item and on through the items until next() returns
 * false. An error may also cause next() to return false. The error provider should be checked to
 * differentiate between end-of-sequence and fail.
 */

  template<typename T> class Iterator {
    public:

      /**
       * Error codes
       */

      enum {
        /// Iteration has reached the end.
        E_END_OF_ENTRIES=1
      };

      /**
       * Virtual destructor. Does nothing.
       */

      virtual ~Iterator() {}

      /**
       * Move to the next item in the sequence, or on to the first item in the sequence if this is the
       * first call to next().
       * @return false if there is an error or the end of sequence is reached. The difference between an error
       * and the end of a sequence can be determined by inspecting the error provider.
       */

      virtual bool next()=0;

      /**
       * Get the current item pointed at by the iterator.
       */

      virtual T current()=0;
  };
}
