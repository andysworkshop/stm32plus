/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

namespace stm32plus {
  namespace net {

    /**
     * Class for the LAN8710a PHY. Contains additional register definitions
     * that describe the additional features of this PHY. This is an MII/RMII PHY.
     */

    class LAN8710A : public PhyBase {

      public:

        /**
         * Custom registers for this PHY
         */

        enum {
          PHY_BasicControl=0,
          PHY_BasicStatus=1,
          PHY_Identifier1=2,
          PHY_Identifier2=3,
          PHY_AutoNegotiationAdvertisement=4,
          PHY_AutoNegotiationLinkPartnerAbility=5,
          PHY_AutoNegotiationExpansion=6,
          PHY_ModeControlStatus=17,
          PHY_SpecialModes=18,
          PHY_SymbolErrorCounter=26,
          PHY_ControlStatusIndication=27,
          PHY_InterruptSource=29,
          PHY_InterruptMask=30,
          PHY_SpecialControlStatus=31
        };


        /**
         * Interrupt bits
         */

        enum {
          INTERRUPT_ENERGYON=0x80,
          INTERRUPT_AUTO_NEGOTIATION=0x40,
          INTERRUPT_REMOTE_FAULT=0x20,
          INTERRUPT_LINK_DOWN=0x10,
          INTERRUPT_AUTO_NEGOTIATION_LP_ACK=0x8,
          INTERRUPT_PARALLEL_FAULT=0x4,
          INTERRUPT_AUTO_NEGOTIATION_PAGE_RX=0x2,
          INTERRUPT_ALL=0xf7
        };


        /**
         * Configuration parameters for the LAN8710A
         */

        struct Parameters : virtual PhyBase::Parameters {

            /**
             * Constructor: set defaults
             */

            Parameters() {

              phy_resetDelay=2;           // wait 2ms after reset is asserted
              phy_speedStatusBit=0x0002;  // link speed bit in status word
              phy_duplexStatusBit=0x0004; // full duplex bit in status word
            }
        };

        bool phyEnableInterrupts(uint8_t interruptMask) const;
        bool phyDisableInterrupts(uint8_t interruptMask) const;

        bool phyIs100M(bool& is100) const;
        bool phyIsFullDuplex(bool& isFull) const;
        bool phyClearPendingInterrupts() const;
        bool phyGetPendingInterrupts(uint16_t& interrupts) const;

        // hard reset is supported through the PhyHardReset feature class

        void phyHardReset(GpioPinRef& pin) const;

        bool initialise(Parameters& params,NetworkUtilityObjects& netutils);
        bool startup();
    };


    /**
     * Initialise the PHY class
     * @param params The parameters class
     * @param netutils The network utilities
     * @return true if it worked
     */

    inline bool LAN8710A::initialise(Parameters& params,NetworkUtilityObjects& netutils) {
      return PhyBase::initialise(params,netutils);
    }


    /**
     * Startup
     * @return true if it worked
     */

    inline bool LAN8710A::startup() {
      return PhyBase::startup();
    }


    /**
     * Check if we are linked at 100Mb/s
     * @param is100 true if we are at 100Mb/s
     * @return true if it worked
     */

    inline bool LAN8710A::phyIs100M(bool& is100) const {

      uint16_t value;

      if(!phyReadRegister(PHY_BasicStatus,value))
        return false;

      is100=(value & 0xE600)!=0;    // bits 15,14,13,10,9
      return true;
    }


    /**
     * Check if we are linked at full duplex
     * @param is100 true if we are at 100Mb/s
     * @return true if it worked
     */

    inline bool LAN8710A::phyIsFullDuplex(bool& isFull) const {

      uint16_t value;

      if(!phyReadRegister(PHY_BasicStatus,value))
        return false;

      isFull=(value & 0x5400)!=0;   // bits 14,12,10
      return true;
    }


    /**
     * Clear pending interrupt flags. On the LAN8710A reading the MISR register
     * clears the pending interrupt flags
     * @return true if it worked
     */

    inline bool LAN8710A::phyClearPendingInterrupts() const {
      uint16_t value;
      return phyReadRegister(PHY_InterruptSource,value);
    }


    /**
     * Read the pending interrupts register to determine which ones have fired
     * @param[out] interrupts The interrupt bitmask. Can be AND'ed with INTERRUPT_ values
     * @return true if it worked
     */

    inline bool LAN8710A::phyGetPendingInterrupts(uint16_t& interrupts) const {
      return phyReadRegister(PHY_InterruptSource,interrupts);
    }


    /**
     * Enable interrupts to be received on the Observable interface. This function merges
     * the bits in interrupt mask into the register
     * @param interruptMask The interrupts to enable (INTERRUPT_*)
     * @return true if it worked
     */

    inline bool LAN8710A::phyEnableInterrupts(uint8_t interruptMask) const {
      return phySetRegisterBits(PHY_InterruptMask,interruptMask);
    }


    /**
     * Disable interrupts to be received on the Observable interface. This function masks out
     * the bits in interrupt mask into the register
     * @param interruptMask The interrupts to enable (INTERRUPT_*)
     * @return true if it worked
     */

    inline bool LAN8710A::phyDisableInterrupts(uint8_t interruptMask) const {
      return phyClearRegisterBits(PHY_InterruptMask,interruptMask);
    }


    /**
     * Hard-reset the PHY by pulling the RESET pin low for 5ms. The datasheet
     * indicates that 1us is the minimum between stable power and rising reset so
     * we're playing safe.
     * @param pin
     */

    inline void LAN8710A::phyHardReset(GpioPinRef& pin) const {
      pin.reset();
      MillisecondTimer::delay(5);
      pin.set();
    }
  }
}
