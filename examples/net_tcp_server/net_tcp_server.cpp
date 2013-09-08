/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/net.h"
#include "config/timing.h"
#include "config/usart.h"


using namespace stm32plus;
using namespace stm32plus::net;


/**
 * This example demonstrates a TCP 'echo' server. Telnet to this server and type lines of
 * text at it to see them echo'd back. Maximum 100 characters per line, please. Multiple
 * simultaneous connections are supported up to the configured maximum per server. The
 * 'TcpConnectionArray' class takes care of the connection multiplexing for you. All you need
 * to do to run a server is subclass TcpConnection and implement the handleRead(), handleWrite()
 * and handleClose() methods as demonstrated below.
 *
 *              +----------------------------+
 * APPLICATION: | DhcpClient                 |
 *              +------+---------------------+
 * TRANSPORT:   | Tcp | Udp                  |
 *              +-----++---------------------+
 * NETWORK      | DefaultIp | Arp            |
 *              +-----+----------------+-----+
 * DATALINK:    | DefaultRmiiInterface | Mac |
 *              +----------------------+-----+
 * PHYSICAL:    | DP83848C                   |
 *              +----------------------------+
 *
 * Tested on devices:
 *   STM32F107VCT6
 *   STM32F407VGT6
 */

class NetTcpServerTest {

  public:

    /**
     * Types that define the network stack
     */

    typedef PhysicalLayer<DP83848C> MyPhysicalLayer;
    typedef DatalinkLayer<MyPhysicalLayer,DefaultRmiiInterface,Mac> MyDatalinkLayer;
    typedef NetworkLayer<MyDatalinkLayer,DefaultIp,Arp> MyNetworkLayer;
    typedef TransportLayer<MyNetworkLayer,Udp,Tcp> MyTransportLayer;
    typedef ApplicationLayer<MyTransportLayer,DhcpClient> MyApplicationLayer;
    typedef NetworkStack<MyApplicationLayer> MyNetworkStack;


    /**
     * Our connection class. The TCP server is templated to return instances of this class when
     * remote clients connect. Each instance of this class independently handles an incoming
     * connection. This model allows you to easily service multiple connections without having
     * to worry about doing the multiplexing yourself. When you're done with a connection just
     * delete it and the event system will take care of all necessary close and cleanup.
     *
     * This example server will read lines of text from the connection and echo them back as you
     * type them. The idea is that you telnet to the server and type lines into it.
     */

    class MyTcpConnection : public TcpConnection {

      protected:

        uint32_t pos;
        uint32_t length;
        bool reading;
        char buffer[101];

      public:

        /**
         * Default constructor. Make sure to call up to base class constructor
         * with the parameters class instance as a reference.
         */

        MyTcpConnection(const Parameters& params)
          : TcpConnection(params) {

          pos=0;
          reading=true;
        }


        /**
         * Read data from the remote end up to a linefeed
         * @return true
         */

        bool handleRead() {

          uint32_t actuallyRead;

          // must be reading state

          if(!reading)
            return true;

          // read up to a \n or we run out of data

          for(;;) {

            if(!receive(&buffer[pos],1,actuallyRead,1) || actuallyRead!=1)
              return true;

            if(buffer[pos++]=='\n') {
              reading=false;
              length=pos;
              pos=0;
              return true;
            }
          }
        }


        /**
         * Echo back the data from the remote end
         * @return true
         */

        bool handleWrite() {

          uint32_t actuallySent;

          // must be writing

          if(reading)
            return true;

          // try to send all remaining

          if(!send(buffer+pos,length,actuallySent,0))
            return true;

          // update counters

          pos+=actuallySent;
          length-=actuallySent;

          // if all gone then flip back to reading

          if(length==0) {
            pos=0;
            reading=true;
          }

          return true;
        }


        /**
         * Callback for when the connection is detected to be closed. We will self-destruct which will
         * cause the connection-released event to be fired and the connection state will be transferred
         * back to the network stack where the TCP closing sequence is managed. The connection itself
         * will be automatically removed from the connections array.
         * @return true - no need to abort the connection round robin.
         */

        bool handleClosed() {
          delete this;
          return true;
        }


        /**
         * Round-robin opportunity to do some processing. We're not interested.
         * @return true
         */

        bool handleCallback() {
          return true;
        }
    };


    /*
     * The network stack object
     */

    MyNetworkStack *_net;


    /*
     * Declare the USART that we'll use. On my dev board USART3 is mapped to PC10,11
     * and we have that defined as remap #2
     */

    typedef Usart3_Remap2<> MyUsart;
    MyUsart *_usart;
    UsartPollingOutputStream *_outputStream;


    /*
     * Run the test
     */

    void run() {

      // declare an instance of the USART and the stream that we'll use to write to it

      _usart=new MyUsart(57600);
      _outputStream=new UsartPollingOutputStream(*_usart);

      // declare the RTC that that stack requires. it's used for cache timeouts, DHCP lease expiry
      // and such like so it does not have to be calibrated for accuracy. A few seconds here or there
      // over a 24 hour period isn't going to make any difference. Start it ticking at zero which is
      // some way back in 2000 but that doesn't matter to us

      Rtc<RtcLsiClockFeature<Rtc32kHzLsiFrequencyProvider>,RtcSecondInterruptFeature> rtc;
      rtc.setTick(0);

      // declare an instance of the network stack

      MyNetworkStack::Parameters params;
      _net=new MyNetworkStack;

      // the stack requires the RTC

      params.base_rtc=&rtc;

      // It's nice to give the DHCP client a host name because then it'll show up in DHCP
      // 'active leases' page. In a home router this is often called 'attached devices'

      params.dhcp_hostname="stm32plus";

      // subscribe to error events from the network stack

      _net->NetworkErrorEventSender.insertSubscriber(NetworkErrorEventSourceSlot::bind(this,&NetTcpServerTest::onError));

      // Initialise the stack. This will reset the PHY, initialise the MAC
      // and attempt to create a link to our link partner. Ensure your cable
      // is plugged in when you run this or be prepared to handle the error

      if(!_net->initialise(params))
        error();

      // start the ethernet MAC Tx/Rx DMA channels
      // this will trigger the DHCP transaction

      if(!_net->startup())
        error();

      // create a TCP server on port 12345

      TcpServer<MyTcpConnection> *tcpServer;

      if(!_net->tcpCreateServer(12345,tcpServer))
        error();

      // create an array to hold the active connections and configure it to
      // automatically receive connections as they arrive. It will also
      // automatically remove connections as they are closed.

      TcpConnectionArray<MyTcpConnection> connections(*tcpServer);

      // now all the plumbing is in place, open up the server to start
      // accepting connection requests

      tcpServer->start();
      *_outputStream << "TCP server started\r\n";

      // loop forever servicing connections via their handleXXX() methods

      connections.wait(TcpWaitState::READ | TcpWaitState::WRITE | TcpWaitState::CLOSED,0);
    }


    /**
     * Network error event received, report it
     * @param ned
     */

    void onError(NetEventDescriptor& ned) {

      NetworkErrorEvent& errorEvent(static_cast<NetworkErrorEvent&>(ned));

      char buf[20];

      *_outputStream << "Error (provider/code/cause) ";

      StringUtil::modp_uitoa10(errorEvent.provider,buf);
      *_outputStream << buf;

      StringUtil::modp_uitoa10(errorEvent.code,buf);
      *_outputStream << "/" << buf;

      StringUtil::modp_uitoa10(errorEvent.cause,buf);
      *_outputStream << "/" << buf << "\r\n";
    }


    void error() {
      *_outputStream << "Aborted execution due to an unexpected error\r\n";
      for(;;);
    }
};


/*
 * Main entry point
 */

int main() {

  // interrupts
  Nvic::initialise();

  // set up SysTick at 1ms resolution
  MillisecondTimer::initialise();

  NetTcpServerTest test;
  test.run();

  // not reached
  return 0;
}
