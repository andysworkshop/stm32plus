/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"

#if defined(STM32PLUS_F4_HAS_MAC) || defined(STM32PLUS_F1_CL_E)

#include "config/net.h"


namespace stm32plus {
  namespace net {


    /**
     * Constructor when creating a connection from an incoming connection to a TcpServer. This is IRQ code.
     * @param networkUtilityObjects utility objects
     * @param tcpEvents TCP events
     * @param segmentEvent The event that triggered the server
     * @param segmentSizeLimit The MSS limit for this server
     * @param additionalHeaderSize Number of bytes required for lower layer headers
     */

    bool TcpConnection::initialise(NetworkUtilityObjects& networkUtilityObjects,
                                   TcpEvents& tcpEvents,
                                   TcpSegmentEvent& segmentEvent,
                                   uint16_t segmentSizeLimit,
                                   uint16_t additionalHeaderSize) {

      const TcpOptionMaximumSegmentSize *mss;

      // remember parameters

      _networkUtilityObjects=&networkUtilityObjects;
      _tcpEvents=&tcpEvents;
      _segmentSizeLimit=segmentSizeLimit;
      _additionalHeaderSize=additionalHeaderSize;
      _lastZeroWindowPollTime=0;

      // create the receive buffer

      _receiveBuffer=new TcpReceiveBuffer(_params.tcp_receiveBufferSize);

      // set up the class

      initialise(segmentEvent.ipPacket.header->ip_sourceAddress,
                 segmentEvent.sourcePort,
                 segmentEvent.destinationPort);

      // pull out the state variables from the remote side

      _state.rxWindow.receiveNext=NetUtil::ntohl(segmentEvent.tcpHeader.tcp_sequenceNumber);
      _state.txWindow.sendWindow=NetUtil::ntohs(segmentEvent.tcpHeader.tcp_windowSize);

      // find the MSS option

      if((mss=segmentEvent.tcpHeader.findOption<TcpOptionMaximumSegmentSize>())==nullptr)
        _remoteMss=536;                 // default from the RFC
      else
        _remoteMss=NetUtil::ntohs(mss->tcp_optionMss);

      // this is an incoming client connection to our server. we need to send a SYN-ACK

      _state.localPortIsEphemeral=false;
      _state.changeState(*_networkUtilityObjects,TcpState::SYN_RCVD);

      return sendSynAck();
    }


    /**
     * Constructor when creating a client for an outgoing connection to a server.
     * @param networkUtilityObjects utility objects
     * @param tcpEvents TCP events
     * @param segmentSizeLimit The MSS limit for this server
     * @param additionalHeaderSize Number of bytes required for lower layer headers
     */

    bool TcpConnection::initialise(NetworkUtilityObjects& networkUtilityObjects,
                                   TcpEvents& tcpEvents,
                                   const IpAddress& remoteAddress,
                                   uint16_t localPort,
                                   uint16_t remotePort,
                                   uint16_t segmentSizeLimit,
                                   uint16_t additionalHeaderSize) {

      // remember parameters

      _networkUtilityObjects=&networkUtilityObjects;
      _tcpEvents=&tcpEvents;
      _segmentSizeLimit=segmentSizeLimit;
      _additionalHeaderSize=additionalHeaderSize;
      _lastZeroWindowPollTime=0;

      // create the receive buffer

      _receiveBuffer=new TcpReceiveBuffer(_params.tcp_receiveBufferSize);

      // set up the class

      initialise(remoteAddress,remotePort,localPort);

      // no state variables from the remote side yet

      _state.rxWindow.receiveNext=0;
      _state.txWindow.sendWindow=0;

      // this is an incoming client connection to our server. we need to send a SYN-ACK

      _state.localPortIsEphemeral=true;
      _state.changeState(*_networkUtilityObjects,TcpState::SYN_SENT);

      return sendSyn();
    }


    /**
     * Destructor
     */

    TcpConnection::~TcpConnection() {

      // unsubscribe from notification events

      _networkUtilityObjects->NetworkNotificationEventSender.removeSubscriber(NetworkNotificationEventSourceSlot::bind(this,&TcpConnection::onNotification));

      // unsubscribe from receive events

      _tcpEvents->TcpReceiveEventSender.removeSubscriber(TcpReceiveEventSourceSlot::bind(this,&TcpConnection::onReceive));

      // notify that we've been released. depending on our state, the connection may be moved into the
      // closing handler

      _networkUtilityObjects->NetworkNotificationEventSender.raiseEvent(TcpConnectionReleasedEvent(*this));

      // delete the receive buffer

      delete _receiveBuffer;
    }


    /**
     * Initialise the class members and subscriptions
     * @param remoteAddress IP address of the remote end
     * @param remotePort The remote port
     * @param localPort The local port
     */

    void TcpConnection::initialise(const IpAddress& remoteAddress,uint16_t remotePort,uint16_t localPort) {

      _state.changeState(*_networkUtilityObjects,TcpState::CLOSED);
      _state.additionalHeaderSize=_additionalHeaderSize;

      _state.localPort=localPort;
      _state.remotePort=remotePort;
      _state.remoteAddress=remoteAddress;

      // the receive window is not currently closed

      _receiveWindowIsClosed=false;

      // set the last active time to now

      _lastActiveTime=MillisecondTimer::millis();

      // generate a random initial sequence number. Yes this is wrong according to the host requirements RFC but we're
      // not in a position to maintain a continuously incrementing 4us timer. OK to cast off the volatile here.

      _networkUtilityObjects->nextRandom(const_cast<uint32_t&>(_state.txWindow.sendNext));
      _state.txWindow.sendNext&=0x7FFFFFFF;

      _state.txWindow.sendUnacknowledged=_state.txWindow.sendNext;
      _state.rxWindow.receiveWindow=_receiveBuffer->availableToWrite();

      // subscribe to notification events

      _networkUtilityObjects->NetworkNotificationEventSender.insertSubscriber(NetworkNotificationEventSourceSlot::bind(this,&TcpConnection::onNotification));

      // subscribe to segment receive events

      _tcpEvents->TcpReceiveEventSender.insertSubscriber(TcpReceiveEventSourceSlot::bind(this,&TcpConnection::onReceive));
    }


    /**
     * Callback from our subscription to notification events
     * @param ned The event descriptor
     */

    void TcpConnection::onNotification(NetEventDescriptor& ned) {

      if(ned.eventType==NetEventDescriptor::NetEventType::TCP_FIND_CONNECTION)
        handleFindConnectionEvent(static_cast<TcpFindConnectionNotificationEvent&>(ned));
    }


    /**
     * Segment received event. This is IRQ code.
     * @param event The event
     */

    void TcpConnection::onReceive(TcpSegmentEvent& event) {

      // must be for this connection

      if(event.ipPacket.header->ip_sourceAddress!=_state.remoteAddress ||
         _state.localPort!=event.destinationPort ||
         _state.remotePort!=event.sourcePort)
        return;

      // it's for us, so it's considered handled even if we drop it

      event.handled=true;

      // see what we've been sent

      if(event.tcpHeader.hasRst())
        handleIncomingRst();
      else {

        // check for SYN-ACK

        if(event.tcpHeader.hasSyn() && event.tcpHeader.hasAck())
          handleIncomingSynAck(event.tcpHeader);
        else {

          if(event.tcpHeader.hasAck())
            handleIncomingAck(event.tcpHeader,event.payloadLength!=0);

          if(event.payloadLength>0)
            handleIncomingData(event);

          if(event.tcpHeader.hasFin())
            handleIncomingFin(event);
        }

        // store the latest sender window size - it can change on any incoming segment

        _state.txWindow.sendWindow=NetUtil::ntohs(event.tcpHeader.tcp_windowSize);
      }
    }


    /**
     * Handle a FIN (remote close) coming from the other side.
     * Change the state to CLOSE_WAIT and send a notification.
     *
     * This is IRQ code
     */

    void TcpConnection::handleIncomingFin(TcpSegmentEvent& event) {

      uint32_t rxnext;

      // the sequence number must be in order. if the segments have got out of order on
      // the network then there could be data to come before this FIN

      rxnext=NetUtil::ntohl(event.tcpHeader.tcp_sequenceNumber);
      if(rxnext!=_state.rxWindow.receiveNext)
        return;

      // change our state

      _state.changeState(*_networkUtilityObjects,TcpState::CLOSE_WAIT);

      // ACK the FIN so the connection is now half-closed

      _state.rxWindow.receiveNext++;
      _state.sendAck(*_networkUtilityObjects,sillyWindowAvoidance());

      // notify

      TcpConnectionClosedEventSender.raiseEvent(TcpConnectionClosedEvent(*this));
    }


    /**
     * Handle some incoming data from the remote end. This is IRQ code.
     * @param event The segment event
     */

    void TcpConnection::handleIncomingData(const TcpSegmentEvent& event) {

      uint32_t rxnext;

      // we've become active

      _lastActiveTime=MillisecondTimer::millis();

      // we can only handle sequential data. if the sequence number on the incoming packet is
      // not what we expect then we drop the segment because an earlier segment has either got
      // lost or been overtaken on the network. the sender will have to resend.

      rxnext=NetUtil::ntohl(event.tcpHeader.tcp_sequenceNumber);

      if(rxnext==_state.rxWindow.receiveNext) {

        // the data size cannot be greater than the write space available in the buffer. If it
        // is then the sender is most likely probing a zero window that we have advertised.

        if(event.payloadLength<=_receiveBuffer->availableToWrite()) {

          // write the data into the buffer

          _receiveBuffer->write(event.payload,event.payloadLength);

          // update our variables

          _state.rxWindow.receiveNext+=event.payloadLength;
          _state.rxWindow.receiveWindow=_receiveBuffer->availableToWrite();
        }
      }

      // ack the current state

      _state.sendAck(*_networkUtilityObjects,sillyWindowAvoidance());

      // notify if there is some data to read

      if(_receiveBuffer->availableToRead()>0)
        TcpConnectionDataReadyEventSender.raiseEvent(TcpConnectionDataReadyEvent(*this));
    }


    /**
     * Handle an incoming ACK. We can handle any ACK that moves sendUnacknowledged forward. We are trusting
     * the remote not to ACK data that it hasn't received.
     * This is IRQ code.
     * @param header The TCP header
     * @param true if this segment contains data
     */

    void TcpConnection::handleIncomingAck(const TcpHeader& header,bool hasData) {

      uint32_t newSuna;

      // if the current state is SYN_RCVD then we can move to established

      if(_state.state==TcpState::SYN_RCVD)
        _state.changeState(*_networkUtilityObjects,TcpState::ESTABLISHED);

      // the gotcha here is to cater for 32-bit overflow while checking that the new s.una
      // is greater than the old which is necessary to avoid winding back the window by
      // accident when segments arrive out of order. We arbitrarily decide that a distance
      // of 2^31 between the pointers is sufficient to indicate a wrap.

      newSuna=NetUtil::ntohl(header.tcp_ackNumber);
      if((newSuna>_state.txWindow.sendUnacknowledged || _state.txWindow.sendUnacknowledged-newSuna>0x80000000))
        _state.txWindow.sendUnacknowledged=newSuna;
      else {

        // if the ACK has no data and did not move the window then re-ack our current state
        // possibly opening our window

        if(!hasData)
          _state.sendAck(*_networkUtilityObjects,sillyWindowAvoidance());
      }
    }


    /**
     * Handle an incoming SYN-ACK.
     * This is IRQ code.
     * @param header The TCP header
     */

    void TcpConnection::handleIncomingSynAck(const TcpHeader& header) {

      const TcpOptionMaximumSegmentSize *mss;

      // the only legal state is SYN_SENT

      if(_state.state!=TcpState::SYN_SENT)
        return;

      // that SYN cost us a sequence number

      _state.txWindow.sendNext++;

      // pull out the state variables from the remote side

      _state.rxWindow.receiveNext=NetUtil::ntohl(header.tcp_sequenceNumber)+1;
      _state.txWindow.sendWindow=NetUtil::ntohs(header.tcp_windowSize);

      // find the MSS option

      if((mss=header.findOption<TcpOptionMaximumSegmentSize>())==nullptr)
        _remoteMss=536;                 // default from the RFC
      else
        _remoteMss=NetUtil::ntohs(mss->tcp_optionMss);

      // we're established, as far as we know

      _state.changeState(*_networkUtilityObjects,TcpState::ESTABLISHED);

      // ACK their SYN-ACK

      _state.sendAck(*_networkUtilityObjects,sillyWindowAvoidance());
    }


    /**
     * Send a SYN segment to the server. This segment has no data. It contains the SYN flag plus our receive buffer
     * size and the MSS option.
     * @return true if it was sent
     */

    bool TcpConnection::sendSyn() {

      // create a NetBuffer to hold the SYN segment

      NetBuffer *nb=new NetBuffer(_additionalHeaderSize+TcpHeader::getNoOptionsHeaderSize(),4);

      // set up MSS (maximum segment size) option

      TcpOptionMaximumSegmentSize *mssOption=reinterpret_cast<TcpOptionMaximumSegmentSize *>(nb->moveWritePointerBack(4));
      mssOption->initialise(_segmentSizeLimit);

      // construct the header

      TcpHeader *header=reinterpret_cast<TcpHeader *>(nb->moveWritePointerBack(TcpHeader::getNoOptionsHeaderSize()));

      // we're acking the SYN, which costs the sender 1 sequence number

      header->initialise(_state.localPort,                          // ports
                         _state.remotePort,
                         _state.txWindow.sendNext,                  // initial sequence number
                         0,                                         // nothing to ACK
                         getReceiveBufferSpaceAvailable(),          // data space available
                         TcpHeaderFlags::SYN);

      // this header is larger than the minimum

      header->setSize(TcpHeader::getNoOptionsHeaderSize()+TcpOptionMaximumSegmentSize::getSize());

      // ask the IP layer to send the packet

      IpTransmitRequestEvent iptre(
          nb,
          _state.remoteAddress,
          IpProtocol::TCP);

      _networkUtilityObjects->NetworkSendEventSender.raiseEvent(iptre);
      return iptre.succeeded;
    }


    /**
     * Send a SYN-ACK segment back to our client. This segment has no data. It contains the SYN
     * and ACK flags plus our receive buffer size and the MSS option.
     * @return true if it worked
     */

    bool TcpConnection::sendSynAck() {

      // create a NetBuffer to hold the SYN-ACK segment. we're dealing with an incoming
      // SYN segment from an IRQ

      NetBuffer *nb=new NetBuffer(_additionalHeaderSize+TcpHeader::getNoOptionsHeaderSize(),4);

      // set up MSS (maximum segment size) option

      TcpOptionMaximumSegmentSize *mssOption=reinterpret_cast<TcpOptionMaximumSegmentSize *>(nb->moveWritePointerBack(4));
      mssOption->initialise(_segmentSizeLimit);

      // construct the header

      TcpHeader *header=reinterpret_cast<TcpHeader *>(nb->moveWritePointerBack(TcpHeader::getNoOptionsHeaderSize()));

      // we're acking the SYN, which costs the sender 1 sequence number

      _state.rxWindow.receiveNext++;

      header->initialise(_state.localPort,                          // ports
                         _state.remotePort,
                         _state.txWindow.sendNext,                  // initial sequence number
                         _state.rxWindow.receiveNext,               // ack the SYN (the SYN consumes a sequence number)
                         getReceiveBufferSpaceAvailable(),          // data space available
                         TcpHeaderFlags::SYN | TcpHeaderFlags::ACK);

      // this header is larger than the minimum

      header->setSize(TcpHeader::getNoOptionsHeaderSize()+TcpOptionMaximumSegmentSize::getSize());

      // increment our sequence number

      _state.txWindow.sendNext++;

      // ask the IP layer to send the packet

      IpTransmitRequestEvent iptre(
            nb,
            _state.remoteAddress,
            IpProtocol::TCP);

      _networkUtilityObjects->NetworkSendEventSender.raiseEvent(iptre);
      return iptre.succeeded;
    }


    /**
     * Send a batch of data to the remote client with an optional timeout. If the timeout is zero
     * then this is effectively a blocking call that will not return until success or a network
     * error occurs.
     *
     * Data is sent in segments to the other side. The size of each segment is bounded by the lower
     * of our MTU and the last known receive window of the recipient. actuallySent is updated to
     * hold the amount of data acknowledged by the other end when this function returns.
     *
     * If tcp_nagleAvoidance is true (the default) then this method tries to send at least two
     * packets per call to force the remote to ACK immediately. If only one packet were to go out
     * per call then we may have to wait up to 200ms for the remote end's Nagle algorithm timer
     * to expire and send us our ACK.
     *
     * The timeout, if non zero, is applied in full to each segment that we send. It means
     * that we will wait for that many milliseconds after we send a segment to receiving an ACK
     * before we give up. There will be an interplay between this timeout and the tcp_sendRetry*
     * configuration parameters.
     *
     * @param data The buffer of data to transmit
     * @param datasize How many bytes of data to transmit
     * @param[out] actuallySent How many bytes we sent and have been acknowledged, updated on success or failure.
     * @param[in] timeoutMillis How long to wait for any blocking state to release, or zero (the default) to wait forever.
     * @return true if all the data was sent, false if there was an error. Even if there is an error it is still possible for data to have been sent. Always check actuallySent to see how much data was sent.
     */

    bool TcpConnection::send(const void *data,uint32_t datasize,uint32_t& actuallySent,uint32_t timeoutMillis) {

      uint32_t bufpos,expectsuna,batchpos,batchbufpos,now,resendtimeout,startwait;
      uint16_t batchwin,batchsendcap;
      TcpHeaderFlags headerFlags;

      actuallySent=0;
      now=MillisecondTimer::millis();

      // we've become active

      _lastActiveTime=now;

      // if the state is SYNC_RCVD then we wait for it to move on

      if(_state.state==TcpState::SYN_RCVD)
        if(!waitForStateChange(TcpState::SYN_RCVD,timeoutMillis))
          return false;

      // the new state must be ESTABLISHED

      if(_state.state!=TcpState::ESTABLISHED)
        return _networkUtilityObjects->setError(ErrorProvider::ERROR_PROVIDER_NET_TCP_CONNECTION,E_INVALID_STATE);

      // we need to handle the zero window case. don't try to send if the window is at zero
      // for a defined interval.

      if(_state.txWindow.sendWindow==0) {

        if(_lastZeroWindowPollTime==0) {
          _lastZeroWindowPollTime=now;      // we can try now, this is the first time we know it to be zero
          _zeroWindowPollDelay=_params.tcp_initialResendDelay;
        }
        else {
          if(MillisecondTimer::hasTimedOut(_lastZeroWindowPollTime,_zeroWindowPollDelay))
            return true;                    // this is not an error, zero bytes were sent because the window is closed
          else {
            _lastZeroWindowPollTime=now;    // we can try again now
            _zeroWindowPollDelay=std::min(_params.tcp_maxResendDelay,_zeroWindowPollDelay*2);
          }
        }
      }
      else
        _lastZeroWindowPollTime=0;          // non-zero window, cancel the poll time

      // bufpos is the index into the user's data of the current batch being sent
      // batchwin is current sender receive window

      bufpos=0;
      batchwin=_state.txWindow.sendWindow;
      resendtimeout=_params.tcp_initialResendDelay;

      // if the data would be sent in one go and nagle avoidance is enabled then force the send
      // to be 2 packets so that the recipient will generate an ACK immediately.

      if(datasize<=batchwin && _params.tcp_nagleAvoidance && datasize>1)
        batchsendcap=(datasize/2)+1;
      else
        batchsendcap=UINT16_MAX;

      // set up the header flags

      headerFlags=TcpHeaderFlags::ACK;
      if(_params.tcp_push)
        headerFlags=headerFlags | TcpHeaderFlags::PSH;

      // keep going while there is data and the connection is open

      while(datasize>0 && !isLocalEndClosed()) {

        uint16_t batchsize,batchremaining;
        bool resend;

        // a batch is how much we push out without waiting for ACKs. We always try to send 1 byte even when the sender
        // window is closed, this effectively polls the sender for window updates if they've been advertising
        // a zero window to us.

        batchsize=std::max(1UL,std::min(datasize,static_cast<uint32_t>(batchwin)));
        batchremaining=batchsize;
        batchpos=_state.txWindow.sendNext;
        batchbufpos=bufpos;

        // expectsuna is the ACK number that we'll be expecting when this batch is done

        expectsuna=_state.txWindow.sendNext+batchsize;

        // keep transmitting segments for this batch or until the local end is closed

        while(batchremaining>0 && !isLocalEndClosed()) {

          uint16_t tosend;

          // send up to the remote MSS in one segment

          tosend=std::min(std::min(batchsendcap,batchremaining),_remoteMss);

          // send this segment if it's not been ACK'd already (can happen if this is a resend)

          if(batchpos+tosend>=_state.txWindow.sendUnacknowledged) {

            // create a netbuffer for the user data - only the header space is alloc'd. the user data
            // is transmitted in-place.

            NetBuffer *nb=new NetBuffer(_additionalHeaderSize+TcpHeader::getNoOptionsHeaderSize(),
                                        0,
                                        reinterpret_cast<const uint8_t *>(data)+batchbufpos,
                                        tosend);

            // create the header

            TcpHeader *header=reinterpret_cast<TcpHeader *>(nb->moveWritePointerBack(TcpHeader::getNoOptionsHeaderSize()));

            header->initialise(_state.localPort,
                               _state.remotePort,
                                batchpos,                       // where we are sending from
                                _state.rxWindow.receiveNext,    //  ack up to receiveNext
                                _state.rxWindow.receiveWindow,  // current window size
                                headerFlags);                   // always ACK

            // ask the IP layer to send the packet

            IpTransmitRequestEvent iptre(
                  nb,
                  _state.remoteAddress,
                  IpProtocol::TCP);

            _networkUtilityObjects->NetworkSendEventSender.raiseEvent(iptre);
            if(!iptre.succeeded)
              return false;
          }

          // update the sequence number (batchpos) and the user buffer position (batchbufpos)

          batchpos+=tosend;
          batchbufpos+=tosend;
          batchremaining-=tosend;
        }

        // reset the resend flag

        resend=false;
        startwait=MillisecondTimer::millis();

        // wait for the ACKs on that last batch to come back

        while(expectsuna!=_state.txWindow.sendUnacknowledged && !isLocalEndClosed()) {

          // check for user timeout, measured from the beginning of the call

          if(timeoutMillis && MillisecondTimer::hasTimedOut(now,timeoutMillis))
            return _networkUtilityObjects->setError(ErrorProvider::ERROR_PROVIDER_NET_TCP_CONNECTION,E_TIMED_OUT);

          // check for resend timeout for this batch

          if(MillisecondTimer::hasTimedOut(startwait,resendtimeout)) {
            resend=true;
            resendtimeout=std::min(_params.tcp_maxResendDelay,resendtimeout*2);
            break;
          }
        }

        // if we're not about to go into a resend of this batch then update the batch position
        // for the next run

        if(!resend) {
          resendtimeout=_params.tcp_initialResendDelay;
          bufpos=batchbufpos;
          _state.txWindow.sendNext=batchpos;      // it's very important that sendNext and actuallySent move in sync
          actuallySent+=batchsize;
          datasize-=batchsize;
          batchwin=_state.txWindow.sendWindow;
        }
      }

      // if we bailed because the state was changed then indicate that to the caller

      if(isLocalEndClosed())
        return _networkUtilityObjects->setError(ErrorProvider::ERROR_PROVIDER_NET_TCP_CONNECTION,E_CONNECTION_RESET);

      return true;
    }


    /**
     * Receive some data from the remote client. If the timeout is zero then this is a blocking call that will
     * not return until success, the other end closes, or a network error occurs. actuallyReceived will be filled
     * in with the actual amount of data received.
     *
     * If the other end closes then 'true' is returned and actuallyReceived may be less than you asked for. Call
     * isRemoteEndClosed() to test if the other end has closed.
     *
     * This is not IRQ safe.
     *
     * @param data Where to receive the data
     * @param dataSize The amount of data to receive
     * @param actuallyReceived The amount of data actually received
     * @param timeoutMillis The total time limit to wait for all blocking calls to complete, or zero to always block.
     * @return true if there was no error.
     */

    bool TcpConnection::receive(void *data,uint32_t dataSize,uint32_t& actuallyReceived,uint32_t timeoutMillis) {

      uint32_t now,received;
      uint8_t *ptr;

      actuallyReceived=0;

      // get current time

      if(timeoutMillis)
        now=MillisecondTimer::millis();
      else
        now=0;      // keep the compiler quiet

      // continue until finished

      ptr=reinterpret_cast<uint8_t *>(data);

      while(dataSize>0) {

        if(_receiveBuffer->availableToRead()) {

          // copy in as much as we can

          received=std::min(dataSize,_receiveBuffer->availableToRead());
          _receiveBuffer->read(ptr,received);

          // update counters

          dataSize-=received;
          actuallyReceived+=received;
          ptr+=received;

          _state.rxWindow.receiveWindow=_receiveBuffer->availableToWrite();

          // reset timeout base

          if(timeoutMillis)
            now=MillisecondTimer::millis();
        }
        else {

          // there is nothing to read. has the remote end closed the connection? if so then nothing
          // more is ever going to arrive. this is not an error condition.

          if(isRemoteEndClosed())
            break;
          else {

            // no data arrived and the remote end is still open. check for timeout.

            if(timeoutMillis && MillisecondTimer::hasTimedOut(now,timeoutMillis)) {

              // if some data was received then this is not an error

              if(actuallyReceived)
                break;

              // no data arrived and we timed out. this is an error

              return _networkUtilityObjects->setError(ErrorProvider::ERROR_PROVIDER_NET_TCP_CONNECTION,E_TIMED_OUT);
            }
          }
        }
      }

      // if we've got some data then we can check if a currently-closed receive window can be opened

      if(actuallyReceived) {

        // this must be done with IRQs suspended

        IrqSuspend suspender;

        // if the receive window is closed and we can now open it then do so

        if(_receiveWindowIsClosed && receiveWindowCanBeOpened()) {
          _receiveWindowIsClosed=false;
          _state.sendAck(*_networkUtilityObjects,sillyWindowAvoidance());
        }
      }

      // finished

      return true;
    }
  }
}


#endif
