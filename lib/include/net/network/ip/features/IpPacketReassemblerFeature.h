/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Handler for fragmented IPv4 packets. Fragmented packets are held in a linked list until either
     * the full packet arrives or one of the parameterised limits is hit. Management of the incoming
     * packets is done according to the algorithm described in RFC815 with the 'hole' descriptors
     * existing inside the holes themselves.
     */

    class IpPacketReassemblerFeature {

      public:

        /**
         * Error codes
         */

        enum {
          E_TOO_MANY_FRAGMENTED_PACKETS = 1,
          E_PACKET_TOO_BIG,
          E_MEMORY_LIMIT_EXCEEDED,
          E_OUT_OF_MEMORY
        };

        struct Parameters {

          uint16_t ip_maxPacketLength;                    //<! max length of any packet. default is 2048 bytes
          uint16_t ip_maxInProgressFragmentedPackets;   //<! max incoming packets that can be fragmented and incompletely assembled in memory. The default is 2.
          uint32_t ip_maxFragmentedPacketMemoryUsage;   //<! similar to ip_maxInProgressFragmentedPackets, but restricts overall memory usage of the fragment reassembler. The default is 4096.
          uint8_t ip_fragmentExpirySeconds;             //<! seconds after which partially reassembled packets are dropped (default is 15)
          uint8_t ip_fragmentExpiryIntervalCheckSeconds;  //<! how often to wake up and check for expired fragments

          /**
           * Constructor, set the default parameters
           */

          Parameters() {
            ip_maxPacketLength=2048;
            ip_maxInProgressFragmentedPackets=2;
            ip_maxFragmentedPacketMemoryUsage=4096;
            ip_fragmentExpirySeconds=15;
            ip_fragmentExpiryIntervalCheckSeconds=23;
          }
        };

      private:
        Parameters _params;
        std::slist<IpFragmentedPacket *> _frags;
        uint32_t _totalFragmentSize;
        uint32_t _inFlightPacketCount;
        NetworkUtilityObjects *_utilityObjects;

      private:
        bool internalHandleFragment(const IpPacket& packet,IpFragmentedPacket*& fp);
        IpFragmentedPacket *findFragment(const IpFragmentedPacket::PacketId& pid) const;
        bool createNewFragment(IpFragmentedPacket::PacketId pid,IpFragmentedPacket *&fp);
        void expireOldEntries(NetworkIntervalTickData& nitd);

      public:
        ~IpPacketReassemblerFeature();

        bool initialise(const Parameters& params,NetworkUtilityObjects& utilityObjects);
        bool startup();

        bool ip_handleFragment(const IpPacket& packet,IpFragmentedPacket*& fp);
        void ip_freePacket(IpFragmentedPacket *packetToFree);
    };
  }
}
