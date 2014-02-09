/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Disable the packet reassembler feature with stub methods
     */

    class IpDisablePacketReassemblerFeature {

      public:
        enum {
          E_FEATURE_DISABLED = 1
        };

      public:
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

        bool ip_handleFragment(const IpPacket& /* packet */,IpFragmentedPacket*& fp) {
          fp=nullptr;
          return _networkUtilityObjects->setError(ErrorProvider::ERROR_PROVIDER_NET_IP_DISABLED_PACKET_REASSEMBLER_FEATURE,E_FEATURE_DISABLED);
        }

        void ip_freePacket(IpFragmentedPacket * /* packetToFree */ ) {
        }
    };
  }
}
