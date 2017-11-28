/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {

    /**
     * Utility class to do the IP checksum algorithm
     */

    class InternetChecksum {

      protected:

        struct PseudoHeader {
          IpAddress sourceAddress;
          IpAddress destinationAddress;
          uint8_t zero;
          IpProtocol protocol;
          uint16_t length;
        };

      protected:
        static void sumit(const void *vptr,uint16_t length,uint32_t& sum);

      public:
        static void calculate(const IpAddress& sourceAddress,const IpAddress& destinationAddress,NetBuffer& nb);
    };
  }
}
