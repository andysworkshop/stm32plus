/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {

    /**
     * Extend the state to include a time field for when we give
     * up and clean up this connection
     */

    struct TcpClosingConnectionState : TcpConnectionState {

      /**
       * The tick time when we will expire this entry due to receiving nothing
       * from the other end (2 * msl)
       */

      uint32_t cleanupTime;


      /**
       * Construct from an existing remote state
       * @param rstate
       */

      TcpClosingConnectionState(const TcpConnectionState& rstate,uint32_t ctime) {
        *static_cast<TcpConnectionState *>(this)=rstate;
        cleanupTime=ctime;
      }


      /**
       * Default constructor
       */

      TcpClosingConnectionState() {
      }


      /**
       * Copy constructor
       * @param src where to copy from
       */

      TcpClosingConnectionState(const TcpClosingConnectionState& src) {
        TcpConnectionState::operator=(src);
        cleanupTime=src.cleanupTime;
      }


      /**
       * Check if this event matches this remote state
       * @param event The event to check
       * @return true if it matches
       */

      bool matches(TcpSegmentEvent& event) const {

        return event.ipPacket.header->ip_sourceAddress==remoteAddress &&
               localPort==event.destinationPort &&
               remotePort==event.sourcePort;
      }
    };
  }
}
