/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /*
   * Internal full-speed PHY implementation, inherits from the PHY base functionality
   */

  class UsbInternalFsPhy {

    public:
      struct Parameters {
      };

    protected:
      USB_OTG_GlobalTypeDef *_phyRegisters;

    public:
      UsbInternalFsPhy(Parameters& /* params */)
        : _phyRegisters(USB_OTG_FS) {
      }


      /**
       * Return true if the PHY supports DMA
       * @return false
       */

      bool phySupportsDma() const {
        return false;
      }


      /**
       * Get the type of interface (embedded/ULPI)
       * @return PCD_PHY_EMBEDDED
       */

      uint32_t getPhyInterface() const {
        return PCD_PHY_EMBEDDED;
      }


      /**
       * Get the interface speed
       * @return PCD_SPEED_FULL
       */

      uint32_t getPhySpeed() const {
        return PCD_SPEED_FULL;
      }
  };
}
