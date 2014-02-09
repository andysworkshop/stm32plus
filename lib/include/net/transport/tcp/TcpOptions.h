/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {

    /**
     * All the options we care about
     */

    enum class TcpOptionKind : uint8_t {
      END_OF_OPTIONS       = 0x00,                      ///< no more options in the header
      NOP                  = 0x01,                      ///< padding option
      MAXIMUM_SEGMENT_SIZE = 0x02                       ///< MSS
    };


    /**
     * Global operator == for comparison against uint8_t
     * @param lhs
     * @param rhs
     * @return
     */

    inline bool operator==(uint8_t lhs,TcpOptionKind rhs) {
      return lhs==static_cast<uint8_t>(rhs);
    }


    /**
     * Base structure for the header options
     */

    struct TcpOption {

      TcpOptionKind tcp_optionKind;

      void initialise(TcpOptionKind optionKind) {
        tcp_optionKind=optionKind;
      }
    } __attribute__((packed));


    /**
     * Base structure for a header option with length
     */

    struct TcpVariableLengthOption : TcpOption {

      uint8_t tcp_optionLength;

      void initialise(TcpOptionKind optionKind,uint8_t length) {
        TcpOption::initialise(optionKind);
        tcp_optionLength=length;
      }
    } __attribute__((packed));


    /**
     * End of options
     */

    struct TcpOptionEndOfOptions : TcpOption {
      void initialise() {
        TcpOption::initialise(TcpOptionKind::END_OF_OPTIONS);
      }
    } __attribute__((packed));


    /**
     * no-operation
     */

    struct TcpOptionNop : TcpOption {
      void initialise() {
        TcpOption::initialise(TcpOptionKind::NOP);
      }
    } __attribute__((packed));


    /**
     * MSS
     */

    struct TcpOptionMaximumSegmentSize : TcpVariableLengthOption {

      uint16_t tcp_optionMss;

      void initialise(uint16_t mss) {
        TcpVariableLengthOption::initialise(TcpOptionKind::MAXIMUM_SEGMENT_SIZE,4);
        tcp_optionMss=NetUtil::htons(mss);
      }

      constexpr static uint16_t getSize() {
        return 4;
      }

      constexpr static TcpOptionKind getOptionKind() {
        return TcpOptionKind::MAXIMUM_SEGMENT_SIZE;
      }
    } __attribute__((packed));
  }
}
