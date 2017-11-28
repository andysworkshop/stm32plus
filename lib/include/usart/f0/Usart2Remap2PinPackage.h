/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F0
#error This class can only be used with the STM32F0 series
#endif


namespace stm32plus {

  /**
   * Remap #2:
   * (TX,RX,RTS,CTS,CK) = (PA14,PA15,PA1,PA0,PA4)
   */

  struct Usart2Remap2PinPackage {
    enum {
      Port_TX=GPIOA_BASE,
      Port_RX=GPIOA_BASE,
      Port_RTS=GPIOA_BASE,
      Port_CTS=GPIOA_BASE,
      Port_CK=GPIOA_BASE,

      Pin_TX=GPIO_Pin_14,       // note, clashes with SWCLK
      Pin_RX=GPIO_Pin_15,
      Pin_RTS=GPIO_Pin_1,
      Pin_CTS=GPIO_Pin_0,
      Pin_CK=GPIO_Pin_4
    };
  };


  /**
   * Convenience class to make use of Usart2Remap2PinPackage
   */

  template<class... Features>
  struct Usart2_Remap2 : UsartPeripheral<Usart2Remap2PinPackage,PERIPHERAL_USART2>,
                         Features... {

    /**
     * Constructor
     * @param params Initialisation parameters
     */

    Usart2_Remap2(const Parameters& params)
      : UsartPeripheral<Usart2Remap2PinPackage,PERIPHERAL_USART2>(params),
        Features(static_cast<Usart&>(*this))... {
    }
  };
}
