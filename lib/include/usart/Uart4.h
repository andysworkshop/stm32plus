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

  template<class TPinPackage,class... Features>
  struct Uart4_Custom : UsartPeripheral<TPinPackage,PERIPHERAL_UART4>,
                        Features... {

    /**
     * Constructor
     * @param params Initialisation parameters
     */

    Uart4_Custom(const typename UsartPeripheral<TPinPackage,PERIPHERAL_UART4>::Parameters& params)
      : UsartPeripheral<TPinPackage,PERIPHERAL_UART4>(params),
        Features(static_cast<Usart&>(*this))... {
    }
  };


  /**
   * Default pin package:
   * (TX,RX) = (PC10,PC11)
   */

  struct Uart4DefaultPinPackage {
    enum {
      Port_CK=0,
      Port_TX=GPIOC_BASE,
      Port_RX=GPIOC_BASE,
      Port_CTS=0,
      Port_RTS=0,

      Pin_CK=0,
      Pin_TX=GPIO_Pin_10,
      Pin_RX=GPIO_Pin_11,
      Pin_CTS=0,
      Pin_RTS=0
    };
  };


  /**
   * Convenience class to match the F1 pin for pin.
   */

  template<class... Features>
  struct Uart4 : UsartPeripheral<Uart4DefaultPinPackage,PERIPHERAL_UART4>,
                 Features... {

    /**
     * Constructor
     * @param params Initialisation parameters
     */

    Uart4(const Parameters& params)
    : UsartPeripheral<Uart4DefaultPinPackage,PERIPHERAL_UART4>(params),
        Features(static_cast<Usart&>(*this))... {
    }
  };
}
