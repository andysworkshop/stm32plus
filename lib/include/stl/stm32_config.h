/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once

#include <cstdio>
#include <cstdlib>
#include <string/StringUtil.h>


namespace stm32plus {

// default alloc-ahead for vectors. quoting from the SGI docs:
//
//   "It is crucial that the amount of growth is proportional to the current capacity(),
//    rather than a fixed constant: in the former case inserting a series of elements
//    into a vector is a linear time operation, and in the latter case it is quadratic."
//
// If this advice pertains to you, then uncomment the first line and comment out the second.
// The default here on the stm32 is to assume that memory is scarce.

//  template<typename T> size_t Stm32VectorAllocAhead(size_t oldSize_) { return 2*oldSize_; }
  template<typename T> size_t Stm32VectorAllocAhead(size_t oldSize_) { return 20+oldSize_; }
//  template<> size_t Stm32VectorAllocAhead<char>(size_t oldSize_) { return 20+oldSize_; }     // sample specialization for char

// minimum buffer size allocated ahead by a deque

  inline size_t Stm32DequeBufferSize() { return 20; }

// alloc-ahead additional memory increment for strings. The default SGI implementation will add
// the old size, doubling memory each time. We don't have memory to burn, so add 20 types each time

  template<typename T> size_t Stm32StringAllocAheadIncrement(size_t) { return 20; }
}
