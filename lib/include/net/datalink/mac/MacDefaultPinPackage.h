/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * MacDefaultPinPackage favours the lower ports and matches the STM32F107 pin-for-pin.
     */

    struct MacDefaultPinPackage {

      enum {
        Mac_Peripheral = PeripheralName::PERIPHERAL_MAC
      };

      // MII ports/pins

      typedef gpio::PC3 MII_TXCLK;
      typedef gpio::PB12 MII_TXD0;
      typedef gpio::PB13 MII_TXD1;
      typedef gpio::PC2 MII_TXD2;
      typedef gpio::PB8 MII_TXD3;
      typedef gpio::PB11 MII_TXEN;
      typedef gpio::PA1 MII_RXCLK;
      typedef gpio::PC4 MII_RXD0;
      typedef gpio::PC5 MII_RXD1;
      typedef gpio::PB0 MII_RXD2;
      typedef gpio::PB1 MII_RXD3;
      typedef gpio::PB10 MII_RXER;
      typedef gpio::PA7 MII_RXDV;
      typedef gpio::PA0 MII_CRS;
      typedef gpio::PA3 MII_COL;

      // RMII ports/pins

      typedef gpio::PB12 RMII_TXD0;
      typedef gpio::PB13 RMII_TXD1;
      typedef gpio::PB11 RMII_TXEN;
      typedef gpio::PC4 RMII_RXD0;
      typedef gpio::PC5 RMII_RXD1;
      typedef gpio::PA7 RMII_CRSDV;
      typedef gpio::PA1 RMII_REFCLK;

      // PHY ports/pins

      typedef gpio::PC1 MDC;
      typedef gpio::PA2 MDIO;
    };
  }
}
