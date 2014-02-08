/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Stub class to disable ip packet fragmentation and prevent any linking of
     * the associated code paths.
     */

    class IpDisablePacketFragmentFeature {

      public:
        enum {
          E_FEATURE_DISABLED = 1
        };

      protected:
        struct Parameters {
        };

        NetworkUtilityObjects *_networkUtilityObjects;

      public:
        bool initialise(const Parameters&,NetworkUtilityObjects& networkUtilityObjects) {
          _networkUtilityObjects=&networkUtilityObjects;
          return true;
        }

        bool startup() {
          return true;
        }

        bool ip_createFragments(IpProtocol /* protocol */,
                                const IpAddress& /* myIpAddress */,
                                const IpAddress& /* destinationIpAddress */,
                                uint16_t /* mtu */,
                                uint16_t /* linkHeaderSize */,
                                NetBuffer * /* inputBuffer */,
                                NetBuffer **& outputBuffers,
                                uint16_t& outputBufferCount) {

        outputBuffers=nullptr;
        outputBufferCount=0;
        return _networkUtilityObjects->setError(ErrorProvider::ERROR_PROVIDER_NET_IP_DISABLED_PACKET_FRAGMENT_FEATURE,E_FEATURE_DISABLED);
      }
    };
  }
}
