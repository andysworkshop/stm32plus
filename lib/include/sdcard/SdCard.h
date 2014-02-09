/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Template feature collection class for the sd-card peripheral.
   */

  template<class... Features>
  struct SdCard : SdCardBase,
                  Features... {

    SdCard() :
      Features(static_cast<SdCardBase&>(*this))... {
    }
  };
}
