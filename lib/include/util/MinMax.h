/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


#error The use of Min, Max and Abs from this file is now deprecated. Please migrate to std::min, std::max and std::abs. This file will be deleted in a future release.


  /**
   * Max implementation with templates
   * @param a
   * @param b
   * @return The greater of a,b
   */

  template<typename T>
  constexpr inline const T& Max(const T& a,const T& b) {
    return (a>b) ? a : b;
  }

  /**
   * Min implementation with templates
   * @param a
   * @param b
   * @return The lesser of a,b
   */

  template<typename T>
  constexpr inline const T& Min(const T& a,const T& b) {
    return (a<b) ? a : b;
  }

  /**
   * Abs implementation with templates
   * @param a negative or positive value
   * @return A positive version of the parameter
   */

  template<typename T>
  constexpr inline const T Abs(const T a) {
    return a<0 ? -a : a;
  }
}
