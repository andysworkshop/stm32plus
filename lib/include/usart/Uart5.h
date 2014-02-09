/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Generic class for any pin package
   */

  template<class TPinPackage,class...Features>
  struct Uart5_Custom : UsartPeripheral<TPinPackage,PERIPHERAL_UART5>,
                        Features... {

    /**
     * Constructor
     * @param params Initialisation parameters
     */

    Uart5_Custom(const typename UsartPeripheral<TPinPackage,PERIPHERAL_UART5>::Parameters& params)
      : UsartPeripheral<TPinPackage,PERIPHERAL_UART5>(params),
        Features(static_cast<Usart&>(*this))... {
    }
  };


  /**
   * Default pin package:
   * (TX,RX,RTS,CTS,CK) = (PA9,PA10,PA12,PA11,PA8)
   */

  struct Uart5DefaultPinPackage {
    enum {
      Port_CK=0,
      Port_TX=GPIOC_BASE,
      Port_RX=GPIOD_BASE,
      Port_CTS=0,
      Port_RTS=0,

      Pin_CK=0,
      Pin_TX=GPIO_Pin_12,
      Pin_RX=GPIO_Pin_2,
      Pin_CTS=0,
      Pin_RTS=0
    };
  };


  /**
   * Convenience class to match the F1 pin for pin.
   */

  template<class... Features>
  struct Uart5 : UsartPeripheral<Uart5DefaultPinPackage,PERIPHERAL_UART5>,
                 Features... {

    /**
     * Constructor
     * @param params Initialisation parameters
     */

    Uart5(const Parameters& params)
    : UsartPeripheral<Uart5DefaultPinPackage,PERIPHERAL_UART5>(params),
        Features(static_cast<Usart&>(*this))... {
    }
  };
}
