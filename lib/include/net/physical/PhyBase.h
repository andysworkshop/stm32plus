/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {

    /**
     * Base class for all PHY features.
     */

    class PhyBase {

      public:

        /**
         * Error codes
         */

        enum {
          E_PHY_WAIT_TIMEOUT = 1  //!< timed out waiting for a status change
        };


        /**
         * Standardised PHY registers
         */

        enum {
          BASIC_MODE_CONFIGURATION        = 0x00,   //!< BASIC_MODE_CONFIGURATION
          STATUS_WORD                     = 0x01,   //!< STATUS_WORD
          PHY_IDENTIFICATION_1            = 0x02,   //!< PHY_IDENTIFICATION_1
          PHY_IDENTIFICATION_2            = 0x03,   //!< PHY_IDENTIFICATION_2
          ABILITY_ADVERTISEMENT           = 0x04,   //!< ABILITY_ADVERTISEMENT
          LINK_PARTNER_ABILITY            = 0x05,   //!< LINK_PARTNER_ABILITY
          AUTO_NEGOTIATION_EXPANSION      = 0x06,   //!< AUTO_NEGOTIATION_EXPANSION
          AUTO_NEGOTIATION_NEXT_PAGE      = 0x07,   //!< AUTO_NEGOTIATION_NEXT_PAGE
          LINK_PARTNER_NEXT_PAGE_ABILITY  = 0x08    //!< LINK_PARTNER_NEXT_PAGE_ABILITY
        };


        /**
         * Standardised PHY register bits
         */

        enum {
          RESET_BIT                     = 0x8000,
          AUTO_NEGOTIATION_BIT          = 0x1000,
          LINKED_STATUS_BIT             = 0x4,
          AUTO_NEGOTIATION_COMPLETE_BIT = 0x20,
          LOOPBACK_BIT                  = 0x4000,
          SPEED_100M_BIT                = 0x2000,
          FULL_DUPLEX_BIT               = 0x100
        };


        /**
         * Configuration parameters for any PHY
         */

        struct Parameters {

          uint16_t phy_address;                 //!< station address
          uint16_t phy_readTimeout;             //!< read timeout in ms
          uint16_t phy_writeTimeout;            //!< write timeout in ms
          uint16_t phy_resetDelay;              //!< time to delay after reset in ms
          uint16_t phy_linkTimeout;             //!< time to wait for a link in ms
          uint16_t phy_autoNegotiationTimeout;  //!< time to wait for auto negotiation in ms
          uint16_t phy_duplexStatusBit;         //!< bit mask for full duplex
          uint16_t phy_speedStatusBit;          //!< bit mask for 10M speed
          uint16_t phy_postConfigurationDelay;  //!< time to wait after manual config is complete


          /**
           * Constructor: set defaults
           */

          Parameters() {
            phy_address=1;                    // station address #1
            phy_readTimeout=5000;             // 5s read timeout
            phy_writeTimeout=5000;            // 5s write timeout
            phy_linkTimeout=5000;             // 5s link timeout
            phy_autoNegotiationTimeout=5000;  // 5s auto-negotiation timeout
            phy_postConfigurationDelay=10;    // 10ms delay after configuration is set
          }
        };

      protected:
        Parameters _params;
        NetworkUtilityObjects *_utilityObjects;

      public:
        bool initialise(const Parameters& params,NetworkUtilityObjects& netutils);
        bool startup();

        const Parameters& getParameters() const;

        bool phyReadRegister(uint16_t regNumber,uint16_t& value) const;
        bool phyWriteRegister(uint16_t regNumber,uint16_t value) const;

        bool phyWaitForStatusRegisterBit(uint16_t bit,uint16_t timeout) const;
        bool phySetRegisterBits(uint16_t regNumber,uint16_t bits) const;
        bool phyClearRegisterBits(uint16_t regNumber,uint16_t bits) const;

        bool phySoftReset() const;
        void phyPostConfigurationDelay() const;

        bool phyWaitForLink() const;
        bool phyEnableAutoNegotiation() const;
        bool phyWaitForAutoNegotiationComplete() const;

        bool phyIs100M(uint16_t statusWord) const;
        bool phyIsFullDuplex(uint16_t statusWord) const;
        bool phyIsLinked(bool& islinked) const;
        bool phyEnableLoopback() const;
        bool phyDisableLoopback() const;
        bool phySet100M() const;
        bool phySet10M() const;
        bool phySetFullDuplex() const;
        bool phySetHalfDuplex() const;
    };


    /**
     * Initialise. save configuration values.
     * @return true always
     */

    inline bool PhyBase::initialise(const Parameters& params,NetworkUtilityObjects& netutils) {

      // save configuration values

      _params=params;
      _utilityObjects=&netutils;

      return true;
    }


    /**
     * Startup
     */

    inline bool PhyBase::startup() {
      return true;
    }


    /**
     * Get the PHY configuration parameters
     * @return the parameters class
     */

    inline const typename PhyBase::Parameters& PhyBase::getParameters() const {
      return _params;
    }


    /**
     * Reset the PHY
     * @return true if it worked
     */

    inline bool PhyBase::phySoftReset() const {

      // write the reset bit

      if(!phyWriteRegister(BASIC_MODE_CONFIGURATION,RESET_BIT))
        return false;

      // delay until it's done

      MillisecondTimer::delay(_params.phy_resetDelay);
      return true;
    }


    /**
     * Read a PHY register. Raise an event for the MAC to actually do it
     * @param regNumber The register to read
     * @param value The value read back
     * @return true if it worked
     */

    inline bool PhyBase::phyReadRegister(uint16_t regNumber,uint16_t& value) const {

      PhyReadRequestEvent event(_params.phy_address,regNumber,_params.phy_readTimeout);

      _utilityObjects->NetworkNotificationEventSender.raiseEvent(event);
      value=event.result;

      return event.succeeded;
    }


    /**
     * Write a PHY register. Raise an event for the MAC to actually do it
     * @param regNumber The register to write
     * @param value The value to write
     * @return true if it worked
     */

    inline bool PhyBase::phyWriteRegister(uint16_t regNumber,uint16_t value) const {

      PhyWriteRequestEvent event(_params.phy_address,regNumber,value,_params.phy_readTimeout);

      _utilityObjects->NetworkNotificationEventSender.raiseEvent(event);
      return event.succeeded;
    }


    /**
     * wait for the link status bit to be set
     * @param timeoutMillis how long to wait until we give up
     * @return true if it worked, false on error or timeout
     */

    inline bool PhyBase::phyWaitForLink() const {
      return phyWaitForStatusRegisterBit(LINKED_STATUS_BIT,_params.phy_linkTimeout);
    }


    /**
     * Wait for a status register bit to be set
     * @param timeout How long to wait in millis
     * @return true if it worked
     */

    __attribute__((noinline)) inline bool PhyBase::phyWaitForStatusRegisterBit(uint16_t bit,uint16_t timeout) const {

      uint16_t value;
      uint32_t start;

      // clear the timer

      start=MillisecondTimer::millis();

      do {

        // check for timeout

        if(MillisecondTimer::hasTimedOut(start,timeout))
          return errorProvider.set(ErrorProvider::ERROR_PROVIDER_NET_PHY,E_PHY_WAIT_TIMEOUT);

        // read the status register

        if(!phyReadRegister(STATUS_WORD,value))
          return false;

        // continue while not set

      } while((value & bit)==0);

      return true;
    }


    /**
     * Set 1 or more bits in a register
     * @param regNumber the register to write to
     * @param bits the bitmask of bits to set
     * @return true if it worked
     */

    __attribute__((noinline)) inline bool PhyBase::phySetRegisterBits(uint16_t regNumber,uint16_t bits) const {

      uint16_t value;

      // read old value

      if(!phyReadRegister(regNumber,value))
        return false;

      // merge in bits and write back

      value|=bits;
      return phyWriteRegister(regNumber,value);
    }


    /**
     * Clear one or more bits in a register
     * @param regNumber the register to write to
     * @param bits the bitmask of bits to reset
     * @return true if it worked
     */

    __attribute__((noinline)) inline bool PhyBase::phyClearRegisterBits(uint16_t regNumber,uint16_t bits) const {

      uint16_t value;

      // read old value

      if(!phyReadRegister(regNumber,value))
        return false;

      // mask off bits and write back

      value&=~bits;
      return phyWriteRegister(regNumber,value);
    }


    /**
     * Wait for the configured number of milliseconds after manual configuration
     * of speed and duplex mode.
     */

    inline void PhyBase::phyPostConfigurationDelay() const {
      MillisecondTimer::delay(_params.phy_postConfigurationDelay);
    }


    /**
     * Enable the auto-negotiation feature
     * @return true if it worked
     */

    inline bool PhyBase::phyEnableAutoNegotiation() const {
      return phyWriteRegister(BASIC_MODE_CONFIGURATION,AUTO_NEGOTIATION_BIT);
    }


    /**
     * Wait for auto-negotiation to be complete
     * @return true if it did not timeout and there was no error
     */

    inline bool PhyBase::phyWaitForAutoNegotiationComplete() const {
      return phyWaitForStatusRegisterBit(AUTO_NEGOTIATION_COMPLETE_BIT,_params.phy_autoNegotiationTimeout);
    }


    /**
     * return true if the status word indicates a 100M link
     * @param statusWord The status word
     * @return true if it's 100Mb, false if it's 10Mb
     */

    inline bool PhyBase::phyIs100M(uint16_t statusWord) const {
      return (statusWord & _params.phy_speedStatusBit)==0;
    }


    /**
     * return true if this is a full duplex link
     * @param statusWord The status word
     * @return true if it's full duplex, false if half duplex
     */

    inline bool PhyBase::phyIsFullDuplex(uint16_t statusWord) const {
      return (statusWord & _params.phy_duplexStatusBit)!=0;
    }


    /**
     * Enable the PHY loopback mode
     * @return true if it succeeds
     */

    inline bool PhyBase::phyEnableLoopback() const {
      return phySetRegisterBits(BASIC_MODE_CONFIGURATION,LOOPBACK_BIT);
    }


    /**
     * Disable the PHY loopback mode
     * @return true if it succeeds
     */

    inline bool PhyBase::phyDisableLoopback() const {
      return phyClearRegisterBits(BASIC_MODE_CONFIGURATION,LOOPBACK_BIT);
    }


    /**
     * Set the link speed to 100Mb/s
     * @return true if it worked
     */

    inline bool PhyBase::phySet100M() const {
      return phySetRegisterBits(BASIC_MODE_CONFIGURATION,SPEED_100M_BIT);
    }


    /**
     * Set the link speed to 10Mb/s
     * @return true if it worked
     */

    inline bool PhyBase::phySet10M() const {
      return phyClearRegisterBits(BASIC_MODE_CONFIGURATION,SPEED_100M_BIT);
    }


    /**
     * Set the duplex mode to FULL
     * @return true if it worked
     */

    inline bool PhyBase::phySetFullDuplex() const {
      return phySetRegisterBits(BASIC_MODE_CONFIGURATION,FULL_DUPLEX_BIT);
    }


    /**
     * Set the duplex mode to HALF
     * @return true if it worked
     */

    inline bool PhyBase::phySetHalfDuplex() const {
      return phyClearRegisterBits(BASIC_MODE_CONFIGURATION,FULL_DUPLEX_BIT);
    }


    /**
     * Check if the PHY is linked
     * @param[out] islinked true if linked
     * @return true if it worked
     */

    inline bool PhyBase::phyIsLinked(bool& islinked) const {

      uint16_t value;

      if(!phyReadRegister(STATUS_WORD,value))
        return false;

      islinked=(value & LINKED_STATUS_BIT)!=0;
      return true;
    }
  }
}
