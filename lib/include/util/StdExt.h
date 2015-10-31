/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once

/**
 * This file contains useful STL/libstdc++/boost features not found in the SGI STL
 * included in this library
 */

namespace std {


  typedef char (&_is_base_of_yes)[1];
  typedef char (&_is_base_of_no)[2];

  template <typename B,typename D>
  struct _is_base_of_host {
    operator B*() const;
    operator D*();
  };

  template <typename B,typename D>
  struct is_base_of
  {
    template<typename T>
    static _is_base_of_yes check(D*, T);
    static _is_base_of_no check(B*,int);

    static const bool value=sizeof(check(_is_base_of_host<B,D>(),int()))==sizeof(_is_base_of_yes);
  };


  template<class InputIterator, class UnaryPredicate>
  bool all_of (InputIterator first, InputIterator last, UnaryPredicate pred) {
    while (first!=last) {
      if (!pred(*first)) return false;
      ++first;
    }
    return true;
  }


  template<class InputIterator, class UnaryPredicate>
  bool any_of (InputIterator first, InputIterator last, UnaryPredicate pred) {
    while (first!=last) {
      if (pred(*first)) return true;
      ++first;
    }
    return false;
  }


  template<class InputIterator, class UnaryPredicate>
  bool none_of (InputIterator first, InputIterator last, UnaryPredicate pred) {
    while (first!=last) {
      if (pred(*first)) return false;
      ++first;
    }
    return true;
  }


  template<class InputIterator, class UnaryPredicate>
  InputIterator find_if_not (InputIterator first, InputIterator last, UnaryPredicate pred) {
    while (first!=last) {
      if (!pred(*first)) return first;
      ++first;
    }
    return last;
  }
}
