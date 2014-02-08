/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * The sliding receive window. This class manages the following variables.
     *
     *          |<      receiveWindow       >   |
     * +---------------------------------------------
     * |        |                               |
     * +---------------------------------------------
     *          ^
     *     receiveNext
     */

    struct TcpReceiveWindow {
      uint32_t receiveNext;           ///< seq num of next byte expected to arrive
      uint16_t receiveWindow;         ///< the number of bytes that we are currently prepared to receive
    };
  }
}
