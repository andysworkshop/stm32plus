/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Base class for usart features
   */

  class UsartFeatureBase {
    protected:
      Usart& _usart;

    public:
      UsartFeatureBase(Usart& usart);
      operator Usart&();
  };


  /**
   * Constructor
   * @param usart
   */

  inline UsartFeatureBase::UsartFeatureBase(Usart& usart)
    : _usart(usart) {
  }


  /**
   * Cast to Usart reference
   */

  inline UsartFeatureBase::operator Usart&() {
    return _usart;
  }
}
