/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Options found in the TCP header. These values are adjusted for network byte order
     * and are designed to be set into and compared against TCP segment header options
     */

    enum class TcpHeaderFlags : uint16_t {
      FIN = 0x0100,               // closing
      SYN = 0x0200,               // syncing
      RST = 0x0400,               // aborting
      PSH = 0x0800,               // push immediate
      ACK = 0x1000,               // acknowledge included
      URG = 0x2000,               // urgent
      ECE = 0x4000,               // ECN echo
      CWR = 0x8000                // congestion window reduced
    };


    /**
     * Global | operator for flag combining
     * @param lhs flag 1
     * @param rhs flag 2
     * @return result
     */

    inline TcpHeaderFlags operator|(TcpHeaderFlags lhs,TcpHeaderFlags rhs) {
      return static_cast<TcpHeaderFlags>(
          static_cast<uint16_t>(lhs) | static_cast<uint16_t>(rhs)
        );
    }


    /**
     * Global & operator for flag selection
     * @param lhs flag 1
     * @param rhs flag 2
     * @return The selection
     */

    inline TcpHeaderFlags operator&(TcpHeaderFlags lhs,TcpHeaderFlags rhs) {
      return static_cast<TcpHeaderFlags>(
          static_cast<uint16_t>(lhs) & static_cast<uint16_t>(rhs)
        );
    }


    /**
     * Global & operator for flag selection
     * @param lhs flag 1
     * @param rhs flag 2
     * @return The selection
     */

    inline uint16_t operator&(uint16_t lhs,TcpHeaderFlags rhs) {
      return lhs & static_cast<uint16_t>(rhs);
    }


    /**
     * Global equality operator
     * @param lhs flag1
     * @param rhs comparison
     * @return true if equal
     */

    inline bool operator==(TcpHeaderFlags lhs,uint16_t rhs) {
      return static_cast<uint16_t>(lhs)==rhs;
    }


    /**
     * Global inequality operator
     * @param lhs flag1
     * @param rhs comparison
     * @return true if not-equal
     */

    inline bool operator!=(TcpHeaderFlags lhs,uint16_t rhs) {
      return static_cast<uint16_t>(lhs)!=rhs;
    }
  }
}
