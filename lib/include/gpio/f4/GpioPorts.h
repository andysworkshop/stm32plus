/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


// ensure the MCU series is correct
#ifndef STM32PLUS_F4
#error This class can only be used with the STM32F4 series
#endif


namespace stm32plus {

  /*
   * Convenience typedefs to declare ports by letter
   */

  template<class... Features> using GpioA=GpioPort<GPIOA_BASE,Features...>;
  template<class... Features> using GpioB=GpioPort<GPIOB_BASE,Features...>;
  template<class... Features> using GpioC=GpioPort<GPIOC_BASE,Features...>;
  template<class... Features> using GpioD=GpioPort<GPIOD_BASE,Features...>;
  template<class... Features> using GpioE=GpioPort<GPIOE_BASE,Features...>;
  template<class... Features> using GpioH=GpioPort<GPIOH_BASE,Features...>;

#if defined(STM32PLUS_F4_HAS_GPIOF_G_I)
  template<class... Features> using GpioF=GpioPort<GPIOF_BASE,Features...>;
  template<class... Features> using GpioG=GpioPort<GPIOG_BASE,Features...>;
  template<class... Features> using GpioI=GpioPort<GPIOI_BASE,Features...>;
#endif

#if defined(STM32PLUS_F4_HAS_GPIOJ_K)
  template<class... Features> using GpioJ=GpioPort<GPIOJ_BASE,Features...>;
  template<class... Features> using GpioK=GpioPort<GPIOK_BASE,Features...>;
#endif
}
