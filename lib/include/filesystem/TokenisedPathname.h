/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @brief Tokenised pathname string: supports back and forward slash as separator
   */

  class TokenisedPathname : public TokenisedString {
    public:
      TokenisedPathname(const char *pathname_);
      virtual ~TokenisedPathname() {}
  };
}
