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
 * address via DHCP this example will wait for UDP datagrams to arrive on port 12345. When a
 * datagram arrives it will print the first 10 bytes to USART #3. The reception is done
 * asynchronously via a subscription to an event provided by the network stack's UDP module.
 *
 * For testing I recommend the Linux NPING utility that allows you to create hand-crafted
 * network packets to send to your device. For example, here's how to send a byte sequence of
 * 1..10 to your device on 192.168.1.5 using nping:
 *
 * nping --udp 192.168.1.5 -p 12345 --data 0102030405060708090a
 *
 * If there are any errors then they are output to a USART which for me is
 * USART3 57600/8/N/1
 *
 * DHCP lease renewal is not considered here - see the DHCP example for the
 * correct procedure.
 *
 * Here's how the network stack for this example is configured:
 *
 *              +----------------------------+
 * APPLICATION: | DhcpClient                 |
 *              +----------------------------+
 * TRANSPORT:   | Udp                        |
 *              +-----+----------------------+
 * NETWORK      | DefaultIp | Arp            |
 *              +-----+----------------+-----+
 * DATALINK:    | DefaultRmiiInterface | Mac |
 *              +----------------------+-----+
 * PHYSICAL:    | DP83848C                   |
 *              +-----------------------------
 *
 * Tested on devices:
 *   STM32F107VCT6
 *   STM32F407VGT6
 */

class NetUdpReceiveAsyncTest {

  public:

    /**
     * Types that define the network stack
     */

    typedef PhysicalLayer<DP83848C> MyPhysicalLayer;
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
    TextOutputStream *_outputStream;


    /*
     * State variables to manage the incoming datagram
     */

    volatile bool _datagramArrived;
    volatile uint16_t _datagramDataSize;
    volatile uint8_t _datagramData[10];
    volatile IpAddress _remoteAddress;


    /*
     * Run the test
     */

    void run() {

      // nothing arrived yet

      _datagramArrived=false;

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

      _net->NetworkErrorEventSender.insertSubscriber(NetworkErrorEventSourceSlot::bind(this,&NetUdpReceiveAsyncTest::onError));

      // subscribe to incoming datagrams from the UDP module

      _net->UdpReceiveEventSender.insertSubscriber(UdpReceiveEventSourceSlot::bind(this,&NetUdpReceiveAsyncTest::onReceive));

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

        char buffer[20];

        if(_datagramArrived) {

          // a datagram has been received, print the address of the sender

          const_cast<IpAddress&>(_remoteAddress).toString(buffer);

          *(_outputStream) << "From: " << buffer << ": ";

          // now print out the first 10 bytes

          for(uint16_t i=0;i<_datagramDataSize;i++)
            (*_outputStream) << (uint16_t) _datagramData[i] << " ";

          (*_outputStream) << "\r\n";

          // ready for another

          _datagramArrived=false;
        }
      }
    }


    /**
     * A datagram has arrived. Since we are going to see every datagram sent
     * to this host we'll quickly check that the port is ours (12345) and if
     * it is then we'll copy out some data and set the flag that indicates
     * we have data. This is IRQ code so no messing around, do what you have
     * to do and get out.
     * @param event The UDP datagram event
     */

    void onReceive(UdpDatagramEvent& event) {

      // the datagram structure in the event it straight off the network so
      // the multibyte numbers are all in big-endian order

      if(event.udpDatagram.udp_destinationPort!=NetUtil::htons(12345))
        return;

      // it's for us, set the handled flag - if this is not done then an ICMP port
      // unreachable message may be sent back to the sender (depending on your configuration)

      event.handled=true;

      // get the remote address of the sender

      _remoteAddress=event.ipPacket.header->ip_sourceAddress;

      // the datagram length includes the UDP network header (8 bytes)

      _datagramDataSize=NetUtil::ntohs(event.udpDatagram.udp_length)-UdpDatagram::getHeaderSize();

      // cut it down to a max of 10 bytes

      _datagramDataSize=_datagramDataSize<=10 ? _datagramDataSize : 10;

      // copy out the data and notify it's here

      memcpy((void *)_datagramData,event.udpDatagram.udp_data,_datagramDataSize);
      _datagramArrived=true;
    }


    /**
     * Network error event received, report it
     * @param ned The error event
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

  NetUdpReceiveAsyncTest test;
  test.run();

  // not reached
  return 0;
}
