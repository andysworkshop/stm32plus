/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/net_http.h"
#include "config/timing.h"
#include "config/sdcard.h"
#include "config/filesystem.h"
#include "MyHttpConnection.h"


using namespace stm32plus;
using namespace stm32plus::net;


/**
 * This demo brings together a number of the stm32plus components, namely the network stack, the RTC,
 * the SD card and the FAT16/32 filesystem to build a simple web server that listens on port 80.
 *
 * Files are served starting from the root directory on the SD card. HTTP GET is the only action
 * supported. A number of content-type mappings are supported and may be extended by amending
 * MyHttpConnection.h accordingly. The client URI must match a physical file on the card. e.g.
 * http://yourserver/foo/bar.html expects to find a file called /foo/bar.html on the SD card. The
 * server supports HTTP/1.1 persistent connections.
 *
 *              +----------------------------+
 * APPLICATION: | DhcpClient                 |
 *              +------+---------------------+
 * TRANSPORT:   | Tcp | Udp | Icmp           |
 *              +-----++---------------------+
 * NETWORK      | DefaultIp | Arp            |
 *              +-----+----------------+-----+
 * DATALINK:    | DefaultRmiiInterface | Mac |
 *              +----------------------+-----+
 * PHYSICAL:    | DP83848C                   |
 *              +----------------------------+
 *
 * This example is only compatible with the F4 because it requires the SDIO peripheral to be able
 * to talk to the SD card.
 *
 * An example website is included in the 'www' subdirectory of this card. To use, copy 'www' to the
 * root directory of your SD card, run this example and then retrieve it from your web browser at
 * "http://<your-dhcp-ip>/www/index.html"
 *
 * Tested on devices:
 *   STM32F407VGT6
 */

class NetHttpServerTest {

  public:

    /**
     * Types that define the network stack
     */

    typedef PhysicalLayer<DP83848C> MyPhysicalLayer;
    typedef DatalinkLayer<MyPhysicalLayer,DefaultRmiiInterface,Mac> MyDatalinkLayer;
    typedef NetworkLayer<MyDatalinkLayer,DefaultIp,Arp> MyNetworkLayer;
    typedef TransportLayer<MyNetworkLayer,Icmp,Udp,Tcp> MyTransportLayer;
    typedef ApplicationLayer<MyTransportLayer,DhcpClient> MyApplicationLayer;
    typedef NetworkStack<MyApplicationLayer> MyNetworkStack;


    /*
     * The network stack, sdio and filesystem objects that we'll need for this demo
     */

    MyNetworkStack *_net;
    SdioDmaSdCard *_sdcard;
    FileSystem *_fs;
    RtcTimeProvider *_timeProvider;


    /*
     * Run the test
     */

    void run() {

      // declare the RTC that that stack requires. it's used for cache timeouts, DHCP lease expiry
      // and such like so it does not have to be calibrated for accuracy. A few seconds here or there
      // over a 24 hour period isn't going to make any difference. Start it ticking at zero which is
      // some way back in 2000 but that doesn't matter to us

      Rtc<RtcLsiClockFeature<Rtc32kHzLsiFrequencyProvider>,RtcSecondInterruptFeature> rtc;
      rtc.setTick(0);

      // create the RTC time provider for the file system. if writes are made to the card then
      // this provider will be used to timestamp them.

      _timeProvider=new RtcTimeProvider(rtc);

      // declare the SD card and check for error. the card must be inserted at this point

      _sdcard=new SdioDmaSdCard;

      if(errorProvider.hasError())
        error();

      // initialise a file system for the card. FAT16 and FAT32 are supported. the card must
      // already be formatted.

      if(!FileSystem::getInstance(*_sdcard,*_timeProvider,_fs))
        error();

      // declare an instance of the network stack

      MyNetworkStack::Parameters params;
      _net=new MyNetworkStack;

      // the stack requires the RTC

      params.base_rtc=&rtc;

      // It's nice to give the DHCP client a host name because then it'll show up in DHCP
      // 'active leases' page. In a home router this is often called 'attached devices'

      params.dhcp_hostname="stm32plus";

      // increase the number of connections per server

      params.tcp_maxConnectionsPerServer=10;

      // Initialise the stack. This will reset the PHY, initialise the MAC
      // and attempt to create a link to our link partner. Ensure your cable
      // is plugged in when you run this or be prepared to handle the error

      if(!_net->initialise(params))
        error();

      // start the ethernet MAC Tx/Rx DMA channels
      // this will trigger the DHCP transaction

      if(!_net->startup())
        error();

      // create an HTTP server on port 80 (our HTTP operates over TCP (the most common case))
      // Here we take advantage of the second template parameter to the TcpServer template to
      // pass in a user-defined type to the constructor of MyHttpConnection. We use it to pass
      // in a pointer to the filesystem object that holds the web documents.

      TcpServer<MyHttpConnection,FileSystem> *httpServer;

      if(!_net->tcpCreateServer(80,httpServer,_fs))
        error();

      // create an array to hold the active connections and configure it to
      // automatically receive connections as they arrive. It will also
      // automatically remove connections as they are closed.

      TcpConnectionArray<MyHttpConnection> connections(*httpServer);

      // now all the plumbing is in place, open up the server to start
      // accepting connection requests

      httpServer->start();

      // loop forever servicing connections via their handleXXX() methods

      connections.wait(TcpWaitState::WRITE | TcpWaitState::READ | TcpWaitState::CLOSED,0);
    }


    void error() {
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

  NetHttpServerTest test;
  test.run();

  // not reached
  return 0;
}
