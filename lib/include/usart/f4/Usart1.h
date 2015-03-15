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

  /**
   * Generic class for any pin package
   */

  template<class TPinPackage,class... Features>
  struct Usart1_Custom : UsartPeripheral<TPinPackage,PERIPHERAL_USART1>,
                         Features... {

    /**
     * Constructor
     * @param params Initialisation parameters
     */

    Usart1_Custom(const typename UsartPeripheral<TPinPackage,PERIPHERAL_USART1>::Parameters& params)
      : UsartPeripheral<TPinPackage,PERIPHERAL_USART1>(params),
        Features(static_cast<Usart&>(*this))... {
    }
  };


  /**
   * Default pin package:
   * (TX,RX,RTS,CTS,CK) = (PA9,PA10,PA12,PA11,PA8)
   */

  struct Usart1DefaultPinPackage {
    enum {
      Port_CK=GPIOA_BASE,
      Port_TX=GPIOA_BASE,
      Port_RX=GPIOA_BASE,
      Port_CTS=GPIOA_BASE,
      Port_RTS=GPIOA_BASE,

      Pin_CK=GPIO_Pin_8,
      Pin_TX=GPIO_Pin_9,
      Pin_RX=GPIO_Pin_10,
      Pin_CTS=GPIO_Pin_11,
      Pin_RTS=GPIO_Pin_12
    };
  };


  /**
   * Convenience class to match the F1 pin for pin.
   */

  template<class... Features>
  struct Usart1 : UsartPeripheral<Usart1DefaultPinPackage,PERIPHERAL_USART1>,
                  Features... {

    /**
     * Constructor
     * @param params Initialisation parameters
     */

    Usart1(const Parameters& params)
      : UsartPeripheral<Usart1DefaultPinPackage,PERIPHERAL_USART1>(params),
        Features(static_cast<Usart&>(*this))... {
    }
  };


  /**
   * Remap #1:
   * (TX,RX,RTS,CTS,CK) = (PB6,PB7,PA12,PA11,PA8)
   */

  struct Usart1Remap1PinPackage {
    enum {
      Port_TX=GPIOB_BASE,
      Port_RX=GPIOB_BASE,
      Port_RTS=GPIOA_BASE,
      Port_CTS=GPIOA_BASE,
      Port_CK=GPIOA_BASE,

      Pin_TX=GPIO_Pin_6,
      Pin_RX=GPIO_Pin_7,
      Pin_RTS=GPIO_Pin_12,
      Pin_CTS=GPIO_Pin_11,
      Pin_CK=GPIO_Pin_8
    };
  };


  /**
   * Convenience class to match the F1 pin for pin.
   */

  template<class... Features>
  struct Usart1_Remap1 : UsartPeripheral<Usart1Remap1PinPackage,PERIPHERAL_USART1>,
                         Features... {

    /**
     * Constructor
     * @param params Initialisation parameters
     */

    Usart1_Remap1(const Parameters& params)
      : UsartPeripheral<Usart1Remap1PinPackage,PERIPHERAL_USART1>(params),
        Features(static_cast<Usart&>(*this))... {
    }
  };
}
