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
 * This example demonstrates how to receive UDP packets from a remote host. After obtaining an IP
 * address via DHCP this example will wait for UDP datagrams to arrive on port 12345. When a datagram
 * arrives it will print the first 10 bytes to USART #3.
 *
 * For testing I recommend the Linux NPING utility that allows you to create hand-crafted network
 * packets to send to your device. For example, here's how to send a byte sequence of 1..10 to your
 * device on 192.168.1.5 using nping:
 *
 * nping --udp 192.168.1.10 -p 12345 --data 0102030405060708090a
 *
 * The ICMP protocol is included to demonstrate how you'll get a "port unreachable" error if you try
 * to hit a port other than 12345. This feature can be individually controlled or ICMP can be left out
 * entirely.
 *
 * If there are any errors then they are output to a USART which for me is
 * USART3 57600/8/N/1
 *
 * DHCP lease renewal is not considered here - see the DHCP example for the correct procedure.
 *
 * Here's how the network stack for this example is configured:
 *
 *              +----------------------------+
 * APPLICATION: | DhcpClient                 |
 *              +------+---------------------+
 * TRANSPORT:   | Udp | Icmp                 |
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

class NetUdpReceiveTest {

  public:

    /**
     * Types that define the network stack
     */

    typedef PhysicalLayer<DP83848C> MyPhysicalLayer;
    typedef DatalinkLayer<MyPhysicalLayer,DefaultRmiiInterface,Mac> MyDatalinkLayer;
    typedef NetworkLayer<MyDatalinkLayer,DefaultIp,Arp> MyNetworkLayer;
    typedef TransportLayer<MyNetworkLayer,Udp,Icmp> MyTransportLayer;
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
    TextOutputStream *_outputStream;


    /*
     * Run the test
     */

    void run() {

      // declare an instance of the USART and the stream that we'll use to write to it

      _usart=new MyUsart(57600);
      UsartPollingOutputStream usartStream(*_usart);
      _outputStream=new TextOutputStream(usartStream);

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

      _net->NetworkErrorEventSender.insertSubscriber(NetworkErrorEventSourceSlot::bind(this,&NetUdpReceiveTest::onError));

      // Initialise the stack. This will reset the PHY, initialise the MAC
      // and attempt to create a link to our link partner. Ensure your cable
      // is plugged in when you run this or be prepared to handle the error

      if(!_net->initialise(params))
        error();

      // start the ethernet MAC Tx/Rx DMA channels
      // this will trigger the DHCP transaction

      if(!_net->startup())
        error();

      for(;;) {

        uint8_t datagramBytes[10];
        uint16_t size;

        // receive up to 10 bytes with a 24 hour timeout. If nothing
        // arrives in 60 seconds then false is returned and ERROR_PROVIDER_NET_UDP/E_TIMED_OUT
        // are to be found in the errorProvider instance. 'size' is updated with the
        // number of bytes actually received, which may be less than I asked for.

        size=10;
        if(_net->udpReceive(12345,datagramBytes,size,1000*60*60*24)) {

          // a datagram has been received, print out the first 10 bytes

          for(uint16_t i=0;i<size;i++)
            (*_outputStream) << (uint16_t) datagramBytes[i] << " ";

          (*_outputStream) << "\r\n";
        }
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

  NetUdpReceiveTest test;
  test.run();

  // not reached
  return 0;
}
