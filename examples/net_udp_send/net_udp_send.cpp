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
 * This example demonstrates how to send UDP packets to a remote host. After obtaining an IP
 * address via DHCP this example will send three 2Kb UDP packets to a remote host every 5 seconds.
 * The target IP address is hardcoded into this example code and you can change it to fit your
 * network configuration.
 *
 * I recommend either tcpdump (linux) or wireshark (windows) for observing the incoming packets.
 * The requisite wireshark filter is "udp.dstport==12345"
 *
 * If there are any errors then they are output to a USART which for me is USART3 57600/8/N/1
 *
 * DHCP lease renewal is not considered here - see the DHCP example for the correct procedure.
 *
 * Here's how the network stack for this example is configured:
 *
 *              +----------------------------+
 * APPLICATION: | DhcpClient                |
 *              +----------------------------+
 * TRANSPORT:   | Udp                       |
 *              +-----+----------------------+
 * NETWORK      | DefaultIp | Arp           |
 *              +-----+----------------+-----+
 * DATALINK:    | DefaultRmiiInterface | Mac |
 *              +----------------------+-----+
 * PHYSICAL:    | DP83848C                   |
 *              +-----------------------------
 *
 * This example is also tested using the KSZ8051MLL in MII mode instead of the DP83848C/RMII.
 * The KSZ8051MLL test was performed on the STM32F107. The DP83848C was tested on the STM32F407.
 * To reconfigure this demo for the F107 using remapped MAC pins connected to the KSZ8051MLL change
 * the physical and datalink layers thus:
 *
 * typedef PhysicalLayer<KSZ8051MLL> MyPhysicalLayer;
 * typedef DatalinkLayer<MyPhysicalLayer,RemapMiiInterface,Mac> MyDatalinkLayer;
 *
 * Tested on devices:
 *   STM32F107VCT6
 *   STM32F407VGT6
 */

class NetUdpSendTest {

  public:

    /**
     * Types that define the network stack
     */

    typedef PhysicalLayer<KSZ8091RNA> MyPhysicalLayer;
    typedef DatalinkLayer<MyPhysicalLayer,DefaultRmiiInterface,Mac> MyDatalinkLayer;
    typedef NetworkLayer<MyDatalinkLayer,DefaultIp,Arp> MyNetworkLayer;
    typedef TransportLayer<MyNetworkLayer,Udp> MyTransportLayer;
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

      _net->NetworkErrorEventSender.insertSubscriber(NetworkErrorEventSourceSlot::bind(this,&NetUdpSendTest::onError));

      // Initialise the stack. This will reset the PHY, initialise the MAC
      // and attempt to create a link to our link partner. Ensure your cable
      // is plugged in when you run this or be prepared to handle the error

      if(!_net->initialise(params))
        error();

      // start the ethernet MAC Tx/Rx DMA channels
      // this will trigger the DHCP transaction

      if(!_net->startup())
        error();

      // This is where we will send the UDP packets. Change it to suit your requirements
      // IP multicast senders are supported so feel free to use the multicast host group
      // range if that's what you want.

      IpAddress ipAddress("192.168.1.12");

      // Set up a buffer full of a test pattern. The buffer is 50 bytes in size and as such is
      // guaranteed to easily fit within the ethernet MTU size.

      uint32_t i;
      char buffer[50];
      for(i=0;i<sizeof(buffer);i++)
        buffer[i]=0x55;

      for(;;) {

        // send 3 packets in rapid succession to port 12345 on the target host

        for(i=0;i<3;i++)
          _net->udpSend(ipAddress,12345,12345,buffer,sizeof(buffer),false,5000);

        // wait for 5 seconds

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

  NetUdpSendTest test;
  test.run();

  // not reached
  return 0;
}
