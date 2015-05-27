/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/net.h"
#include "config/timing.h"
#include "config/usart.h"


using namespace stm32plus;
using namespace stm32plus::net;


/**
 * This example demonstrates a TCP 'echo' client. It will attempt to connect to a server on a remote
 * computer and send it a line of text. The server will read that line of text and then send it back
 * in reverse. An example server, written in perl, is included in this example code directory.
 * It can be run directly on Linux or Linux-alike systems such as cygwin or msys on Windows. The
 * reversed string will be sent to the USART.
 *
 * I use Usart3, remap-2 with settings 57000/8/N/1.
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

class NetTcpClientTest {

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

      _net->NetworkErrorEventSender.insertSubscriber(NetworkErrorEventSourceSlot::bind(this,&NetTcpClientTest::onError));

      // Initialise the stack. This will reset the PHY, initialise the MAC
      // and attempt to create a link to our link partner. Ensure your cable
      // is plugged in when you run this or be prepared to handle the error

      if(!_net->initialise(params))
        error();

      // start the ethernet MAC Tx/Rx DMA channels
      // this will trigger the DHCP transaction

      if(!_net->startup())
        error();

      *_outputStream << "The TCP client is ready to run\r\n";

      TcpClientConnection *ptr;
      uint32_t actuallyReceived,actuallySent;
      char buffer[100];

      for(;;) {

        // connect to the remote end using a random local ephemeral port

        if(_net->tcpConnect<TcpClientConnection>("192.168.1.3",12345,ptr)) {

          // manage the connection pointer in a scoped_ptr so it's automatically deleted (and closed)
          // when it goes out of scope

          scoped_ptr<TcpClientConnection> conn(ptr);

          // send 11 bytes of text and a newline to the other end (blocking). We have to copy the data to
          // a sram buffer before sending because the STM32 Ethernet DMA peripheral cannot transmit from
          // flash memory. (string constants are compiled into flash).

          strcpy(buffer,"Hello World\n");

          if(conn->send(buffer,12,actuallySent) && actuallySent==12) {

            // receive the response line from the other end (blocking)

            if(conn->receive(buffer,100,actuallyReceived) && actuallyReceived>0) {

              // write the response to the USART stream

              _outputStream->write(buffer,actuallyReceived);
            }
          }
        }
        else
          *_outputStream << "Timed out while trying to connect, trying again...\r\n";

        // pause for 5 seconds to avoid flooding the network before doing it again

        MillisecondTimer::delay(5000);
      }
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

  NetTcpClientTest test;
  test.run();

  // not reached
  return 0;
}
