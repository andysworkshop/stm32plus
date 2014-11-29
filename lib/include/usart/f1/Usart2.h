/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F1
#error This class can only be used with the STM32F1 series
#endif


namespace stm32plus {

  /**
   * Generic class for any pin package
   */

  template<class TPinPackage,class... Features>
  struct Usart2_Custom : UsartPeripheral<TPinPackage,PERIPHERAL_USART2>,
                         Features... {

    /**
     * Constructor
     * @param params Initialisation parameters
     */

    Usart2_Custom(const typename UsartPeripheral<TPinPackage,PERIPHERAL_USART2>::Parameters& params)
      : UsartPeripheral<TPinPackage,PERIPHERAL_USART2>(params),
        Features(static_cast<Usart&>(*this))... {
    }
  };


  /**
   * Default pin package:
   * (TX,RX,RTS,CTS,CK) = (PA2,PA3,PA1,PA0,PA4)
   */

  struct Usart2DefaultPinPackage {
    enum {
      Port_CK=GPIOA_BASE,
      Port_TX=GPIOA_BASE,
      Port_RX=GPIOA_BASE,
      Port_CTS=GPIOA_BASE,
      Port_RTS=GPIOA_BASE,

      Pin_TX=GPIO_Pin_2,
      Pin_RX=GPIO_Pin_3,
      Pin_RTS=GPIO_Pin_1,
      Pin_CTS=GPIO_Pin_0,
      Pin_CK=GPIO_Pin_4
    };
  };


  /**
   * Convenience class to match the F1 pin for pin.
   */

  template<class... Features>
  struct Usart2 : UsartPeripheral<Usart2DefaultPinPackage,PERIPHERAL_USART2>,
                  Features... {

    Usart2(const Parameters& params)
      : UsartPeripheral<Usart2DefaultPinPackage,PERIPHERAL_USART2>(params),
        Features(static_cast<Usart&>(*this))... {
    }
  };


  /**
   * Remap #1:
   * (TX,RX,RTS,CTS,CK) = (PB6,PB7,PA12,PA11,PA8)
   */

  struct Usart2Remap1PinPackage {
    enum {
      Port_TX=GPIOD_BASE,
      Port_RX=GPIOD_BASE,
      Port_RTS=GPIOD_BASE,
      Port_CTS=GPIOD_BASE,
      Port_CK=GPIOD_BASE,

      Pin_TX=GPIO_Pin_5,
      Pin_RX=GPIO_Pin_6,
      Pin_RTS=GPIO_Pin_4,
      Pin_CTS=GPIO_Pin_3,
      Pin_CK=GPIO_Pin_7
    };
  };


  /**
   * Convenience class to match the F1 pin for pin.
   */

  template<class... Features>
  struct Usart2_Remap1 : UsartPeripheral<Usart2Remap1PinPackage,PERIPHERAL_USART2>,
                         Features... {

    /**
     * Constructor
     * @param params Initialisation parameters
     */

    Usart2_Remap1(const Parameters& params)
      : UsartPeripheral<Usart2Remap1PinPackage,PERIPHERAL_USART2>(params),
        Features(static_cast<Usart&>(*this))... {

      GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
    }
  };
}
