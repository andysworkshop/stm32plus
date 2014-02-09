/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * The sliding transmit window. This class manages the following variables.
     *
     *          |<       sendWindow        >|
     * +-----------------------------------------
     * |        |                 |         |
     * +-----------------------------------------
     *          ^                 ^
     *     sendUnacknowleged     sendNext
     */

    struct TcpTransmitWindow {
      uint32_t sendUnacknowledged;      ///< seq num of first byte of data sent but not acked
      uint32_t sendNext;                ///< seq num of next byte of data to be sent
      uint16_t sendWindow;              ///< send window (starts at _sendUnacknowledged)
    };
  }
}
