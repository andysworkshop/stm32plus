/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Base class for anything that needs to use the shared port ranges. This class manages
     * handing out Ephemeral ports. Ephemeral ports are in the 61440-65535 (4K) range. This is significantly
     * less than the 16K ports you get with a typical PC stack but in this MCU we do not have the resources
     * to handle 16K simultaneous ports.
     *
     * This class requires 512 bytes of SRAM for the port bitmap.
     *
     * You can customise the above numbers using the ip_ephemeralPortCount variable in the parameters class.
     */

    class IpPorts {

      public:

        enum  {
          PORT_DNS_REQUEST   = 53,
          PORT_DHCP_REQUEST  = 67,
          PORT_DHCP_RESPONSE = 68
        };

        /**
         * Error codes
         */

        enum {
          E_OUT_OF_MEMORY = 1,
          E_ALL_PORTS_IN_USE
        };


        /**
         * Parameters to the class
         */

        struct Parameters {

          uint16_t ip_firstEphemeralPort;         ///< The first ephemeral port
          uint16_t ip_lastEphemeralPort;          ///< The last ephemeral port.

          Parameters() {
            ip_firstEphemeralPort=49152;
            ip_lastEphemeralPort=65535;
          }
        };


      private:
        Parameters _params;

        std::vector<uint16_t> _ephemeralPortsInUse;
        std::vector<uint16_t> _definedPortsInUse;
        uint16_t _nextEphemeralPort;

      protected:
        void incrementNextEphemeralPort();

      public:
        bool initialise(const Parameters& params,NetworkUtilityObjects& networkUtilityObjects);

        bool ip_acquireEphemeralPort(uint16_t& portNumber);
        bool ip_releaseEphemeralPort(uint16_t portNumber);
        bool ip_isEphemeralPortInUse(uint16_t portNumber) const;

        bool ip_acquireDefinedPort(uint16_t portNumber);
        bool ip_releaseDefinedPort(uint16_t portNumber);
        bool ip_isDefinedPortInUse(uint16_t portNumber) const;
    };


    /**
     * Initialise the class
     * @return true if it worked
     */

    inline bool IpPorts::initialise(const Parameters& params,NetworkUtilityObjects& networkUtilityObjects) {

      uint32_t rnd;

      // store params

      _params=params;

      // start the ephemeral port issuer at a random place

      networkUtilityObjects.nextRandom(rnd);
      _nextEphemeralPort=params.ip_firstEphemeralPort+rnd % (params.ip_lastEphemeralPort-params.ip_firstEphemeralPort);

      return true;
    }


    /**
     * Increment the next ephemeral port
     */

    inline void IpPorts::incrementNextEphemeralPort() {

      if(_nextEphemeralPort==_params.ip_lastEphemeralPort)
        _nextEphemeralPort=_params.ip_firstEphemeralPort;
      else
        _nextEphemeralPort++;
    }


    /**
     * Acquire a free ephemeral port
     * @param[out] portNumber The new port number
     * @return true if it worked
     */

    inline bool IpPorts::ip_acquireEphemeralPort(uint16_t& portNumber) {

      uint16_t end;

      // got to stop if we come back here

      end=_nextEphemeralPort;

      // try to find a port. this is not an efficient search method if the number of ephemeral
      // ports is high. speed is traded for space.

      while(ip_isEphemeralPortInUse(_nextEphemeralPort)) {

        incrementNextEphemeralPort();

        if(end==_nextEphemeralPort)
          return errorProvider.set(ErrorProvider::ERROR_PROVIDER_NET_IPPORTS,E_ALL_PORTS_IN_USE);
      }

      // found one

      portNumber=_nextEphemeralPort;
      _ephemeralPortsInUse.push_back(portNumber);
      incrementNextEphemeralPort();

      return true;
    }


    /**
     * Release an ephemeral port
     * @param portNumber The port to release
     * @return true if it was released
     */

    inline bool IpPorts::ip_releaseEphemeralPort(uint16_t portNumber) {

      std::vector<uint16_t>::iterator it;

      if((it=std::find(_ephemeralPortsInUse.begin(),_ephemeralPortsInUse.end(),portNumber))!=_ephemeralPortsInUse.end()) {
        _ephemeralPortsInUse.erase(it);
        return true;
      }

      return false;
    }


    /**
     * Check if the given ephemeral port is in use
     * @param portNumber the port to check
     * @return true if in use
     */

    inline bool IpPorts::ip_isEphemeralPortInUse(uint16_t portNumber) const {
      return std::find(_ephemeralPortsInUse.begin(),_ephemeralPortsInUse.end(),portNumber)!=_ephemeralPortsInUse.end();
    }


    /**
     * Acquire a defined port
     * @param portNumber The port number to try to acquire
     * @return true if it worked
     */

    inline bool IpPorts::ip_acquireDefinedPort(uint16_t portNumber) {

      if(ip_isDefinedPortInUse(portNumber))
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_NET_IPPORTS,E_ALL_PORTS_IN_USE);

      _definedPortsInUse.push_back(portNumber);
      return true;
    }


    /**
     * Release a defined port
     * @param portNumber The port to release
     * @return true if it was released
     */

    inline bool IpPorts::ip_releaseDefinedPort(uint16_t portNumber) {

      std::vector<uint16_t>::iterator it;

      if((it=std::find(_definedPortsInUse.begin(),_definedPortsInUse.end(),portNumber))!=_definedPortsInUse.end()) {
        _definedPortsInUse.erase(it);
        return true;
      }
      return false;
    }


    /**
     * Check if the given defined port is in use
     * @param portNumber the port to check
     * @return true if in use
     */

    inline bool IpPorts::ip_isDefinedPortInUse(uint16_t portNumber) const {
      return std::find(_definedPortsInUse.begin(),_definedPortsInUse.end(),portNumber)!=_definedPortsInUse.end();
    }
  }
}
