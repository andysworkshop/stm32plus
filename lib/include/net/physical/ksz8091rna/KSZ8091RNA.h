/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {

    /**
     * Class for the KSZ8091RNA PHY. Contains additional register definitions
     * that describe the additional features of this PHY. This is an RMII PHY.
     */

    class KSZ8091RNA : public PhyBase {

      public:

        /**
         * Custom registers for this PHY
         */

        enum {
          AFE_CONTROL_1                   = 0x11,   //!< AFE_CONTROL_1
          AFE_CONTROL_4                   = 0x13,   //!< AFE_CONTROL_4
          RXER_COUNTER                    = 0x15,   //!< RXER_COUNTER
          OPERATION_MODE_STRAP_OVERRIDE   = 0x16,   //!< OPERATION_MODE_STRAP_OVERRIDE
          OPERATION_MODE_STRAP_STATUS     = 0x17,   //!< OPERATION_MODE_STRAP_STATUS
          EXPANDED_CONTROL                = 0x18,   //!< EXPANDED_CONTROL
          INTERRUPT_CONTROL_STATUS        = 0x1b,   //!< INTERRUPT_CONTROL_STATUS
          LINKMD_CABLE_DIAGNOSTIC         = 0x1d,   //!< LINKMD_CABLE_DIAGNOSTIC
          PHY_CONTROL_1                   = 0x1e,   //!< PHY_CONTROL_1
          PHY_CONTROL_2                   = 0x1f    //!< PHY_CONTROL_2
        };

        /**
         * Interrupt bits
         */

        enum {
          INTERRUPT_JABBER                   = 0x80,  //!< INTERRUPT_JABBER
          INTERRUPT_RECEIVE_ERROR            = 0x40,  //!< INTERRUPT_RECEIVE_ERROR
          INTERRUPT_PAGE_RECEIVED            = 0x20,  //!< INTERRUPT_PAGE_RECEIVED
          INTERRUPT_PARALLEL_DETECT_FAULT    = 0x10,  //!< INTERRUPT_PARALLEL_DETECT_FAULT
          INTERRUPT_LINK_PARTNER_ACKNOWLEDGE = 0x8,   //!< INTERRUPT_LINK_PARTNER_ACKNOWLEDGE
          INTERRUPT_LINK_DOWN                = 0x4,   //!< INTERRUPT_LINK_DOWN
          INTERRUPT_REMOTE_FAULT             = 0x2,   //!< INTERRUPT_REMOTE_FAULT
          INTERRUPT_LINK_UP                  = 0x1,   //!< INTERRUPT_LINK_UP
          INTERRUPT_ALL                      = 0xff   //!< INTERRUPT_ALL
        };


        /**
         * Configuration parameters for the KSZ80511MLL
         */

        struct Parameters : virtual PhyBase::Parameters {

          /**
           * Constructor: set defaults
           */

          Parameters() {
            phy_resetDelay      = 100;      // wait 100ms after reset is asserted
            phy_speedStatusBit  = 0x0002;   // link speed bit in status word
            phy_duplexStatusBit = 0x0004;   // full duplex bit in status word
          }
        };

        bool phyEnableInterrupts(uint8_t interruptMask) const;
        bool phyDisableInterrupts(uint8_t interruptMask) const;
        bool phyClearPendingInterrupts() const;

        bool phyIs100M(bool& is100) const;
        bool phyIsFullDuplex(bool& isFull) const;

        // hard reset is supported through the PhyHardReset feature class

        void phyHardReset(GpioPinRef& pin) const;

        bool initialise(Parameters& params,NetworkUtilityObjects& netutils);
        bool startup();
    };


    /**
     * Initialise
     * @param params
     * @return
     */

    inline bool KSZ8091RNA::initialise(Parameters& params,NetworkUtilityObjects& netutils) {
      return PhyBase::initialise(params,netutils);
    }


    /**
     * Startup
     * @return
     */

    inline bool KSZ8091RNA::startup() {
      return PhyBase::startup();
    }


    /**
     * Check if we are linked at 100Mb/s
     * @param is100 true if we are at 100Mb/s
     * @return true if it worked
     */

    inline bool KSZ8091RNA::phyIs100M(bool& is100) const {

      uint16_t value;

      if(!phyReadRegister(PHY_CONTROL_1,value))
        return false;

      value&=7;
      is100=value==2 || value==6;     // 010 or 110

      return true;
    }


    /**
     * Check if we are linked at full duplex
     * @param is100 true if we are at 100Mb/s
     * @return true if it worked
     */

    inline bool KSZ8091RNA::phyIsFullDuplex(bool& isFull) const {

      uint16_t value;

      if(!phyReadRegister(PHY_CONTROL_1,value))
        return false;

      value&=7;
      isFull=value==5 || value==6;

      return true;
    }


    /**
     * Enable interrupts to be received on the Observable interface. This function merges
     * the bits in interrupt mask into the register
     * @param interruptMask The interrupts to enable (INTERRUPT_*)
     * @return true if it worked
     */

    inline bool KSZ8091RNA::phyEnableInterrupts(uint8_t interruptMask) const {
      return phySetRegisterBits(INTERRUPT_CONTROL_STATUS,static_cast<uint16_t>(interruptMask) << 8);
    }


    /**
     * Disable interrupts to be received on the subscription callback. This function masks out
     * the bits in interrupt mask into the register
     * @param interruptMask The interrupts to enable (INTERRUPT_*)
     * @return true if it worked
     */

    inline bool KSZ8091RNA::phyDisableInterrupts(uint8_t interruptMask) const {
      return phyClearRegisterBits(INTERRUPT_CONTROL_STATUS,static_cast<uint16_t>(interruptMask) << 8);
    }


    /**
     * Clear pending interrupt flags. On the KSZ8091RNA reading the INTERRUPT_CONTROL_STATUS register
     * clears the pending interrupt flags
     * @return true if it worked
     */

    inline bool KSZ8091RNA::phyClearPendingInterrupts() const {
      uint16_t value;
      return phyReadRegister(INTERRUPT_CONTROL_STATUS,value);
    }


    /**
     * Hard-reset the KSZ8091RNA by pulling the RESET pin low for 20ms. The datasheet
     * indicates that 10ms is the minimum between stable power and rising reset so
     * we're playing safe.
     * @param pin
     */

    inline void KSZ8091RNA::phyHardReset(GpioPinRef& pin) const {
      pin.reset();
      MillisecondTimer::delay(20);
      pin.set();
    }
  }
}
