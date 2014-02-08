/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @brief A string broken down into components.
   *
   * Access to the components should always start at zero and
   * stop at getNumTokens(). Internally a range of tokens is managed through setRange();
   */

  class TokenisedString {

    protected:
      char **_tokens;
      int _numTokens;
      int _first;
      int _last;

    public:
      TokenisedString(const char *original_,const char *separators_);
      virtual ~TokenisedString();

      int getNumTokens() const;

      const char *operator[](int pos_) const;
      const char *last() const;

      void setRange(int first_,int last_);
      void resetRange();
  };
}
