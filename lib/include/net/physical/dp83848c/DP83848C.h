/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {

    /**
     * Class for the Texas Instruments DP83848C PHY. Contains additional
     * register definitions that describe the additional features of this PHY.
     */

    class DP83848C : public PhyBase {

      public:

        /**
         * Custom registers for this PHY
         */

        enum {
          PHY_STATUS                      = 0x10,   //!< PHY_STATUS
          INTERRUPT_CONTROL               = 0x11,   //!< INTERRUPT_CONTROL
          INTERRUPT_STATUS                = 0x12,   //!< INTERRUPT_STATUS
          FALSE_CARRIER_SENSE_COUNTER     = 0x14,   //!< FALSE_CARRIER_SENSE_COUNTER
          RXER_COUNTER                    = 0x15,   //!< RXER_COUNTER
          PCS_CONFIG_STATUS_100           = 0x16,   //!< PCS_CONFIG_STATUS_100
          RMII_AND_BYPASS                 = 0x17,   //!< RMII_AND_BYPASS
          LED_DIRECT_CONTROL              = 0x18,   //!< LED_DIRECT_CONTROL
          PHY_CONTROL                     = 0x19,   //!< PHY_CONTROL
          STATUS_CONTROL_10BASET          = 0x1a,   //!< STATUS_CONTROL_10BASET
          CD_TEST_BIST                    = 0x1b,   //!< CD_TEST_BIST
          ENERGY_DETECT_CONTROL           = 0x1d    //!< ENERGY_DETECT_CONTROL
        };

        /**
         * Interrupt bits
         */

        enum {
          INTERRUPT_ENERGY_DETECT             = 0x40, //!< INTERRUPT_ENERGY_DETECT
          INTERRUPT_LINK_STATUS_CHANGE        = 0x20, //!< INTERRUPT_LINK_STATUS_CHANGE
          INTERRUPT_SPEED_STATUS_CHANGE       = 0x10, //!< INTERRUPT_SPEED_STATUS_CHANGE
          INTERRUPT_DUPLEX_STATUS_CHANGE      = 0x8,  //!< INTERRUPT_DUPLEX_STATUS_CHANGE
          INTERRUPT_AUTO_NEGOTIATION_COMPLETE = 0x4,  //!< INTERRUPT_AUTO_NEGOTIATION_COMPLETE
          INTERRUPT_FCC_HALF_FULL             = 0x2,  //!< INTERRUPT_FCC_HALF_FULL
          INTERRUPT_RXER_HALF_FULL            = 0x1,  //!< INTERRUPT_RXER_HALF_FULL
          INTERRUPT_ALL                       = 0x7f  //!< INTERRUPT_ALL
        };


        /**
         * Configuration parameters for the DP83848C
         */

        struct Parameters : virtual PhyBase::Parameters {

          /**
           * Constructor: set defaults
           */

          Parameters() {
            phy_resetDelay      = 2;        // wait 2ms after reset is asserted
            phy_speedStatusBit  = 0x0002;   // link speed bit in status word
            phy_duplexStatusBit = 0x0004;   // full duplex bit in status word
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

    inline bool DP83848C::initialise(Parameters& params,NetworkUtilityObjects& netutils) {
      return PhyBase::initialise(params,netutils);
    }


    /**
     * Startup
     * @return true if it worked
     */

    inline bool DP83848C::startup() {
      return PhyBase::startup();
    }


    /**
     * Check if we are linked at 100Mb/s
     * @param is100 true if we are at 100Mb/s
     * @return true if it worked
     */

    inline bool DP83848C::phyIs100M(bool& is100) const {

      uint16_t value;

      if(!phyReadRegister(PHY_STATUS,value))
        return false;

      is100=(value & 0x0002)==0;
      return true;
    }


    /**
     * Check if we are linked at full duplex
     * @param is100 true if we are at 100Mb/s
     * @return true if it worked
     */

    inline bool DP83848C::phyIsFullDuplex(bool& isFull) const {

      uint16_t value;

      if(!phyReadRegister(PHY_STATUS,value))
        return false;

      isFull=(value & 0x0004)!=0;
      return true;
    }


    /**
     * Clear pending interrupt flags. On the DP83848C reading the MISR register
     * clears the pending interrupt flags
     * @return true if it worked
     */

    inline bool DP83848C::phyClearPendingInterrupts() const {
      uint16_t value;
      return phyReadRegister(INTERRUPT_STATUS,value);
    }


    /**
     * Read the pending interrupts register to determine which ones have fired
     * @param[out] interrupts The interrupt bitmask. Can be AND'ed with INTERRUPT_ values
     * @return true if it worked
     */

    inline bool DP83848C::phyGetPendingInterrupts(uint16_t& interrupts) const {

      if(!phyReadRegister(INTERRUPT_STATUS,interrupts))
        return false;

      // pending bits are in the upper byte, shift them down so that the INTERRUPT_* masks apply

      interrupts>>=8;
      return true;
    }


    /**
     * Enable interrupts to be received on the Observable interface. This function merges
     * the bits in interrupt mask into the register
     * @param interruptMask The interrupts to enable (INTERRUPT_*)
     * @return true if it worked
     */

    inline bool DP83848C::phyEnableInterrupts(uint8_t interruptMask) const {
      return
          phySetRegisterBits(INTERRUPT_CONTROL,0x3) &&        // INT(pin-7) + INTEN
          phySetRegisterBits(INTERRUPT_STATUS,interruptMask);
    }


    /**
     * Disable interrupts to be received on the Observable interface. This function masks out
     * the bits in interrupt mask into the register
     * @param interruptMask The interrupts to enable (INTERRUPT_*)
     * @return true if it worked
     */

    inline bool DP83848C::phyDisableInterrupts(uint8_t interruptMask) const {
      return phyClearRegisterBits(INTERRUPT_STATUS,interruptMask);
    }


    /**
     * Hard-reset the PHY by pulling the RESET pin low for 5ms. The datasheet
     * indicates that 1us is the minimum between stable power and rising reset so
     * we're playing safe.
     * @param pin
     */

    inline void DP83848C::phyHardReset(GpioPinRef& pin) const {
      pin.reset();
      MillisecondTimer::delay(5);
      pin.set();
    }
  }
}
