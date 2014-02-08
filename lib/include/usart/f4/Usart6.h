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
  struct Usart6_Custom : UsartPeripheral<TPinPackage,PERIPHERAL_USART6>,
                         Features... {

    Usart6_Custom(const typename UsartPeripheral<TPinPackage,PERIPHERAL_USART6>::Parameters& params)
      : UsartPeripheral<TPinPackage,PERIPHERAL_USART6>(params),
        Features(static_cast<Usart&>(*this))... {
    }
  };


  /*
   * Default pin package:
   * (TX,RX,RTS,CTS,CK) = (PC6,PC7,PG8,PG13,PC8)
   */

  struct Usart6DefaultPinPackage {
    enum {
      Port_TX=GPIOC_BASE,
      Port_RX=GPIOC_BASE,
      Port_RTS=GPIOG_BASE,
      Port_CTS=GPIOC_BASE,
      Port_CK=GPIOC_BASE,

      Pin_TX=GPIO_Pin_6,
      Pin_RX=GPIO_Pin_7,
      Pin_RTS=GPIO_Pin_8,
      Pin_CTS=GPIO_Pin_13,
      Pin_CK=GPIO_Pin_8
    };
  };


  /**
   * Convenience class to match the F1 pin for pin.
   */

  template<class... Features>
  struct Usart6 : UsartPeripheral<Usart6DefaultPinPackage,PERIPHERAL_USART6>,
                  Features... {

    Usart6(const Parameters& params)
      : UsartPeripheral<Usart6DefaultPinPackage,PERIPHERAL_USART6>(params),
        Features(static_cast<Usart&>(*this))... {
    }
  };


  /*
   * Remap #1:
   * (TX,RX,RTS,CTS,CK) = (PG7,PG9,PG12,PG15,PC8)
   */

  struct Usart6Remap1PinPackage {
    enum {
      Port_TX=GPIOG_BASE,
      Port_RX=GPIOG_BASE,
      Port_RTS=GPIOG_BASE,
      Port_CTS=GPIOG_BASE,
      Port_CK=GPIOC_BASE,

      Pin_TX=GPIO_Pin_7,
      Pin_RX=GPIO_Pin_9,
      Pin_RTS=GPIO_Pin_12,
      Pin_CTS=GPIO_Pin_15,
      Pin_CK=GPIO_Pin_8
    };
  };


  /**
   * Convenience class to match the F1 pin for pin.
   */

  template<class... Features>
  struct Usart6_Remap1 : UsartPeripheral<Usart6Remap1PinPackage,PERIPHERAL_USART6>,
                         Features... {

    Usart6_Remap1(const Parameters& params)
      : UsartPeripheral<Usart6Remap1PinPackage,PERIPHERAL_USART6>(params),
        Features(static_cast<Usart&>(*this))... {
    }
  };
}
