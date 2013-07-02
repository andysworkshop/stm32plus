/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {



  /**
   * Max implementation with templates
   * @param a
   * @param b
   * @return The greater of a,b
   */

  template<typename T>
  inline const T& Max(const T& a,const T& b) {
    return (a>b) ? a : b;
  }

  /**
   * Min implementation with templates
   * @param a
   * @param b
   * @return The lesser of a,b
   */

  template<typename T>
  inline const T& Min(const T& a,const T& b) {
    return (a<b) ? a : b;
  }

  /**
   * Abs implementation with templates
   * @param a negative or positive value
   * @return A positive version of the parameter
   */

  template<typename T>
  inline const T Abs(const T a) {
    return a<0 ? -a : a;
  }
}
