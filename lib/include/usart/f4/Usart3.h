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
  struct Usart3_Custom : UsartPeripheral<TPinPackage,PERIPHERAL_USART3>,
                         Features... {

    Usart3_Custom(const typename UsartPeripheral<TPinPackage,PERIPHERAL_USART3>::Parameters& params)
      : UsartPeripheral<TPinPackage,PERIPHERAL_USART3>(params),
        Features(static_cast<Usart&>(*this))... {
    }
  };


  /**
   * Default pin package:
   * (TX,RX,RTS,CTS,CK) = (PB10,PB11,PB14,PB13,PB12)
   */

  struct Usart3DefaultPinPackage {
    enum {
      Port_CK=GPIOB_BASE,     ///< clock port
      Port_TX=GPIOB_BASE,     ///< transmit port
      Port_RX=GPIOB_BASE,     ///< receive port
      Port_CTS=GPIOB_BASE,    ///< CTS port
      Port_RTS=GPIOB_BASE,    ///< RTS port

      Pin_TX=GPIO_Pin_10,     ///< transmit pin
      Pin_RX=GPIO_Pin_11,     ///< receive pin
      Pin_RTS=GPIO_Pin_14,    ///< RTS pin
      Pin_CTS=GPIO_Pin_13,    ///< CTS pin
      Pin_CK=GPIO_Pin_12      ///< clock pin
    };
  };


  /**
   * Convenience class to match the F1 pin for pin.
   */

  template<class... Features>
  struct Usart3 : UsartPeripheral<Usart3DefaultPinPackage,PERIPHERAL_USART3>,
                  Features... {

    /**
     * Constructor
     * @param params initialisation parameters
     */

    Usart3(const Parameters& params)
      : UsartPeripheral<Usart3DefaultPinPackage,PERIPHERAL_USART3>(params),
        Features(static_cast<Usart&>(*this))... {
    }
  };


  /**
   * Remap #1:
   * (TX,RX,RTS,CTS,CK) = (PD8,PD9,PD12,PD11,PD10)
   */

  struct Usart3Remap1PinPackage {
    enum {
      Port_TX=GPIOD_BASE,
      Port_RX=GPIOD_BASE,
      Port_RTS=GPIOD_BASE,
      Port_CTS=GPIOD_BASE,
      Port_CK=GPIOD_BASE,

      Pin_TX=GPIO_Pin_8,
      Pin_RX=GPIO_Pin_9,
      Pin_RTS=GPIO_Pin_12,
      Pin_CTS=GPIO_Pin_11,
      Pin_CK=GPIO_Pin_10
    };
  };


  /**
   * Convenience class to match the F1 pin for pin.
   */

  template<class... Features>
  struct Usart3_Remap1 : UsartPeripheral<Usart3Remap1PinPackage,PERIPHERAL_USART3>,
                         Features... {

    /**
     * Constructor
     * @param params Initialisation parameters
     */

    Usart3_Remap1(const Parameters& params)
      : UsartPeripheral<Usart3Remap1PinPackage,PERIPHERAL_USART3>(params),
        Features(static_cast<Usart&>(*this))... {
    }
  };


  /**
   * Remap #2:
   * (TX,RX,RTS,CTS,CK) = (PC10,PC11,PB14,PB13,PC12)
   */

  struct Usart3Remap2PinPackage {
    enum {
      Port_TX=GPIOC_BASE,
      Port_RX=GPIOC_BASE,
      Port_RTS=GPIOB_BASE,
      Port_CTS=GPIOB_BASE,
      Port_CK=GPIOC_BASE,

      Pin_TX=GPIO_Pin_10,
      Pin_RX=GPIO_Pin_11,
      Pin_RTS=GPIO_Pin_14,
      Pin_CTS=GPIO_Pin_13,
      Pin_CK=GPIO_Pin_12
    };
  };


  /**
   * Convenience class to match the F1 pin for pin.
   */

  template<class... Features>
  struct Usart3_Remap2 : UsartPeripheral<Usart3Remap2PinPackage,PERIPHERAL_USART3>,
                         Features... {

    /**
     * Constructor
     * @param params Initialisation parameters
     */

    Usart3_Remap2(const Parameters& params)
      : UsartPeripheral<Usart3Remap2PinPackage,PERIPHERAL_USART3>(params),
        Features(static_cast<Usart&>(*this))... {
    }
  };
}
