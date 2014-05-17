/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Encapsulation of the ADC channel types for the more advanced MCUs. Used
   * by the analog watchdog functionality.
   */

  enum class AdcChannelType {
    Regular,
    Injected,
    RegularAndInjected
  };
}
