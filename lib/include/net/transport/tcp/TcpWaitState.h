/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

namespace stm32plus {
  namespace net {


    /**
     * Possible connection states that you can ask a connection array
     * to wait for.
     */

    enum class TcpWaitState : uint8_t {
      NONE     = 0,         ///< nothing (used in comparisons)
      READ     = 0x1,       ///< ready to read (rx buffer has data)
      WRITE    = 0x2,       ///< ready to tx (ESTABLISHED and remote window>0)
      CLOSED   = 0x4,       ///< closed or reset (either end)
      CALLBACK = 0x8        ///< implement handleCallback() to get a round-robin callback regardless of connection state
    };


    /**
     * Global & operator
     */

    inline TcpWaitState operator&(TcpWaitState rhs,TcpWaitState lhs) {
      return static_cast<TcpWaitState>(static_cast<uint8_t>(rhs) & static_cast<uint8_t>(lhs));
    }


    /**
     * Global | operator
     */

    inline TcpWaitState operator|(TcpWaitState rhs,TcpWaitState lhs) {
      return static_cast<TcpWaitState>(static_cast<uint8_t>(rhs) | static_cast<uint8_t>(lhs));
    }
  }
}
