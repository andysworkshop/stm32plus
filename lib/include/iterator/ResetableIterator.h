/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

/**
 * @brief Extension of iterator to provide reset functionality.
 * @tparam T The type that we are iterating over.
 *
 * Resettng an iterator moves it back to before the first element.
 */

  template<typename T> 
  class ResetableIterator : public Iterator<T> {
    public:

      virtual ~ResetableIterator() {}

    /**
     * Reset the iterator to before the first element.
     */

      virtual void reset()=0;
  };
}
