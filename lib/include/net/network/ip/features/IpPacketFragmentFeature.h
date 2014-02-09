/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {

    /**
     * Class to take a netbuffer and generate collection of other netbuffers
     * that contain the input data fragmented into parts small enough to
     * fit into the link layer's max MTU size.
     *
     * The original netbuffer's data is referenced by pointers in the fragment
     * buffers so very little additional memory is required. The original netbuffer
     * is added as a reference to the last fragmentso that any higher layer that
     * is tracking outgoing packets by waiting for a netbuffer they created
     * to be sent will function correctly.
     */

    class IpPacketFragmentFeature {

      protected:

        /**
         * Flags
         */

        enum {
          MORE_FRAGMENTS = 0x2000
        };

        /**
         * Parameters class
         */

        struct Parameters {
          bool ip_checksumOnLargeUdpPackets;      ///< true if we manually calculate checksums on large UDP packets. default is true.

          /**
           * Constructor
           */

          Parameters() {
            ip_checksumOnLargeUdpPackets=true;
          }
        };

        Parameters _params;
        uint16_t _mtu;
        uint16_t _linkHeaderSize;
        uint16_t _identification;

      private:
        uint32_t fragmentsRequired(uint16_t size);

        bool internalCreateFragments(NetBuffer *inputBuffer,
                                     NetBuffer **&outputBuffers);

        bool createFragmentsFrom(NetBuffer *inputBuffer,
                                 const void *sourceData,
                                 uint16_t sourceDataSize,
                                 NetBuffer **outputBuffers,
                                 uint16_t firstOutputBuffer,
                                 bool lastFragmentIsHere,
                                 uint16_t& offset);

      public:
        bool initialise(const Parameters& params,NetworkUtilityObjects&);
        bool startup();

        bool ip_createFragments(IpProtocol protocol,
                                 const IpAddress& myIpAddress,
                                 const IpAddress& destinationIpAddress,
                                 uint16_t mtu,
                                 uint16_t linkHeaderSize,
                                 NetBuffer *inputBuffer,
                                 NetBuffer **&outputBuffers,
                                 uint16_t& outputBufferCount);
    };


    /**
     * Initialise the feature
     */

    inline bool IpPacketFragmentFeature::initialise(const Parameters& params,NetworkUtilityObjects&) {
      _params=params;
      _identification=0;
      return true;
    }


    /**
     * startup the feature
     */

    inline bool IpPacketFragmentFeature::startup() {
      return true;
    }
  }
}
