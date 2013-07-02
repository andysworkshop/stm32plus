/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


/**
 * @file
 * This file contains template metaprogramming helpers used elsewhere in the library
 */

namespace stm32plus {

  /**
   * Recursive initialisation of feature classes defined using variadic templates.
   * This initialiser has a parameters class and returns bool
   * Here's the forward definition
   */

  template<typename F,typename... T>
  struct RecursiveBoolInitWithParams;

  /**
   * Recursion termination condition, always return success
   */

  template<class F>
  struct RecursiveBoolInitWithParams<F> {
    static bool tinit(F *,typename F::Parameters&) {
      return true;
    }
  };


  /**
   * General recursive initialiser
   */

  template<class F,class T,class... G>
  struct RecursiveBoolInitWithParams<F,T,G...> {
    static bool tinit(F *ptr,typename F::Parameters& p) {

      if(!ptr->T::initialise(p))
        return false;

      return RecursiveBoolInitWithParams<F,G...>::tinit(ptr,p);
    }
  };


  /**
   * Recursive initialisation of feature classes defined using variadic templates.
   * This initialiser has a parameters class, an observable and returns bool
   * Here's the forward definition
   */

  template<typename F,typename... T>
  struct RecursiveBoolInitWithParamsObservable;

  /**
   * Recursion termination condition, always return success
   */

  template<class F>
  struct RecursiveBoolInitWithParamsObservable<F> {
    static bool tinit(F *,typename F::Parameters&,Observable&) {
      return true;
    }
  };


  /**
   * General recursive initialiser
   */

  template<class F,class T,class... G>
  struct RecursiveBoolInitWithParamsObservable<F,T,G...> {
    static bool tinit(F *ptr,typename F::Parameters& p,Observable& o) {

      if(!ptr->T::initialise(p,o))
        return false;

      return RecursiveBoolInitWithParamsObservable<F,G...>::tinit(ptr,p,o);
    }
  };
}
