/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Simple structure to hold the remote state of a connection: this allows it to be
     * easily transferred between a connection and the closing handler when the connection
     * goes away
     */

    struct TcpConnectionState {

      uint16_t additionalHeaderSize;              ///< how much extra space needed for IP and link layer headers
      IpAddress remoteAddress;                    ///< our peer's address
      uint16_t localPort;                         ///< our local port
      uint16_t remotePort;                        ///< peer remote port
      bool localPortIsEphemeral;                  ///< true if this is a client (local ports are ephemeral)
      volatile TcpState state;                    ///< the state of the connection
      volatile TcpTransmitWindow txWindow;        ///< the transmit window state
      volatile TcpReceiveWindow rxWindow;         ///< the receive window state
      volatile bool pendingDataAck;               ///< true if there's a pending data ACK

      /*
       * Constructor
       */

      TcpConnectionState()
        : state(TcpState::NONE) {
      }

      /**
       * = operator. need this because of the volatile members. used when move the state to the closed
       * manager
       * @param src to copy from
       * @return self-reference
       */

      TcpConnectionState& operator=(const TcpConnectionState& src) {

        additionalHeaderSize=src.additionalHeaderSize;
        remoteAddress=src.remoteAddress;
        localPort=src.localPort;
        localPortIsEphemeral=src.localPortIsEphemeral;
        remotePort=src.remotePort;
        state=src.state;
        pendingDataAck=src.pendingDataAck;

        // OK to cast off the volatile qualifier because by the time this operator is used
        // these variables are no longer volatile

        const_cast<TcpTransmitWindow&>(txWindow)=const_cast<TcpTransmitWindow&>(src.txWindow);
        const_cast<TcpReceiveWindow&>(rxWindow)=const_cast<TcpReceiveWindow&>(src.rxWindow);

        return *this;
      }


      /**
       * (re)send the current ACK
       * @param netutils The network utils
       * @param windowSize the current receive window size
       * @return true if it was sent
       */

      bool sendAck(NetworkUtilityObjects& netutils,uint16_t windowSize) {
        return sendHeaderOnly(netutils,TcpHeaderFlags::ACK,windowSize);
      }


      /**
       * Send a FIN and ACK
       * @param netutils The network utils
       * @param windowSize the current receive window size
       * @return true if it was sent
       */

      bool sendFinAck(NetworkUtilityObjects& netutils,uint16_t windowSize) {
        return sendHeaderOnly(netutils,TcpHeaderFlags::FIN | TcpHeaderFlags::ACK,windowSize);
      }


      /**
       * Send a reset (RST) segment to the other end. After a connection has been reset it is considered
       * aborted and closed. No further operations are possible.
       * @param netutils The network utils
       * @param windowSize the current receive window size
       * @return true if it was sent
       */

      bool sendRstAck(NetworkUtilityObjects& netutils,uint16_t windowSize) {
        changeState(netutils,TcpState::CLOSED);
        return sendHeaderOnly(netutils,TcpHeaderFlags::RST | TcpHeaderFlags::ACK,windowSize);
      }


      /**
       * Send header-with-flags
       * @param netutils The network utils
       * @param windowSize the current receive window size
       * @param flags The flags to set in the header
       * @return true if it was sent
       */

      bool sendHeaderOnly(NetworkUtilityObjects& netutils,
                          TcpHeaderFlags flags,
                          uint16_t windowSize) {

        // create a NetBuffer to hold the RST segment

        NetBuffer *nb=new NetBuffer(additionalHeaderSize+TcpHeader::getNoOptionsHeaderSize(),0);

        // construct the header

        TcpHeader *header=reinterpret_cast<TcpHeader *>(nb->moveWritePointerBack(TcpHeader::getNoOptionsHeaderSize()));

        header->initialise(localPort,
                           remotePort,
                           txWindow.sendNext,           // where we are sending from
                           (flags & TcpHeaderFlags::ACK)==0 ? 0 : rxWindow.receiveNext, //  ack up to receiveNext
                           windowSize,                  // data space available
                           flags);

        // ask the IP layer to send the packet

        IpTransmitRequestEvent iptre(
            nb,
            remoteAddress,
            IpProtocol::TCP);

        netutils.NetworkSendEventSender.raiseEvent(iptre);
        return iptre.succeeded;
      }


      /**
       * Change the state of this connection and notify subscribers
       * @param netutils The network utility objects
       * @param newState The new state
       */

      void changeState(NetworkUtilityObjects& netutils,TcpState newState) {

        TcpState oldState;

        oldState=state;
        state=newState;

        netutils.NetworkNotificationEventSender.raiseEvent(
            TcpConnectionStateChangedEvent(remoteAddress,remotePort,oldState,newState)
          );
      }
    };
  }
}
