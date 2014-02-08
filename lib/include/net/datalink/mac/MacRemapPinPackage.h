/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * MacDefaultPinPackage favours the lower ports and matches the STM32F107 pin-for-pin in remap mode.
     *   RXDV, RXD0..3 are moved to PD8..12
     */

    struct MacRemapPinPackage {

      enum {
        Mac_Peripheral = PeripheralName::PERIPHERAL_MAC_REMAP,

        // MII ports

        Port_MII_TXCLK = GPIOC_BASE,
        Port_MII_TXD0 = GPIOB_BASE,
        Port_MII_TXD1 = GPIOB_BASE,
        Port_MII_TXD2 = GPIOC_BASE,
        Port_MII_TXD3 = GPIOB_BASE,
        Port_MII_TXEN = GPIOB_BASE,
        Port_MII_RXCLK = GPIOA_BASE,
        Port_MII_RXD0 = GPIOD_BASE,
        Port_MII_RXD1 = GPIOD_BASE,
        Port_MII_RXD2 = GPIOD_BASE,
        Port_MII_RXD3 = GPIOD_BASE,
        Port_MII_RXER = GPIOB_BASE,
        Port_MII_RXDV = GPIOD_BASE,
        Port_MII_CRS = GPIOA_BASE,
        Port_MII_COL = GPIOA_BASE,

        // RMII ports

        Port_RMII_TXD0 = GPIOB_BASE,
        Port_RMII_TXD1 = GPIOB_BASE,
        Port_RMII_TXEN = GPIOB_BASE,
        Port_RMII_RXD0 = GPIOD_BASE,
        Port_RMII_RXD1 = GPIOD_BASE,
        Port_RMII_CRSDV = GPIOD_BASE,
        Port_RMII_REFCLK = GPIOA_BASE,

        // PHY ports

        Port_MDC = GPIOC_BASE,
        Port_MDIO = GPIOA_BASE,

        // MII pins

        Pin_MII_TXCLK = GPIO_Pin_3,
        Pin_MII_TXD0 = GPIO_Pin_12,
        Pin_MII_TXD1 = GPIO_Pin_13,
        Pin_MII_TXD2 = GPIO_Pin_2,
        Pin_MII_TXD3 = GPIO_Pin_8,
        Pin_MII_TXEN = GPIO_Pin_11,
        Pin_MII_RXCLK = GPIO_Pin_1,
        Pin_MII_RXD0 = GPIO_Pin_9,
        Pin_MII_RXD1 = GPIO_Pin_10,
        Pin_MII_RXD2 = GPIO_Pin_11,
        Pin_MII_RXD3 = GPIO_Pin_12,
        Pin_MII_RXER = GPIO_Pin_10,
        Pin_MII_RXDV = GPIO_Pin_8,
        Pin_MII_CRS = GPIO_Pin_0,
        Pin_MII_COL = GPIO_Pin_3,

        // RMII pins

        Pin_RMII_TXD0 = GPIO_Pin_12,
        Pin_RMII_TXD1 = GPIO_Pin_13,
        Pin_RMII_TXEN = GPIO_Pin_11,
        Pin_RMII_RXD0 = GPIO_Pin_9,
        Pin_RMII_RXD1 = GPIO_Pin_10,
        Pin_RMII_CRSDV = GPIO_Pin_8,
        Pin_RMII_REFCLK = GPIO_Pin_1,

        // PHY pins

        Pin_MDC = GPIO_Pin_1,
        Pin_MDIO = GPIO_Pin_2
      };
    };
  }
}
