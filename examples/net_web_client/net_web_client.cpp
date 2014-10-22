/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/net.h"
#include "config/net_http.h"
#include "config/timing.h"
#include "config/usart.h"


using namespace stm32plus;
using namespace stm32plus::net;


/**
 * This example shows how to use the HttpClientConnection to retrieve an HTTP resource. In this example
 * we will connect to http://www.st.com and ask for the root document. We will write the response to
 * the USART.
 *
 * I use USART3, remap-2 with settings 57000/8/N/1.
 *
 *              +----------------------------+
 * APPLICATION: | DhcpClient | Dns           |
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

class NetHttpClientTest {

  public:

    /**
     * Types that define the network stack
     */

    typedef PhysicalLayer<DP83848C> MyPhysicalLayer;
    typedef DatalinkLayer<MyPhysicalLayer,DefaultRmiiInterface,Mac> MyDatalinkLayer;
    typedef NetworkLayer<MyDatalinkLayer,DefaultIp,Arp> MyNetworkLayer;
    typedef TransportLayer<MyNetworkLayer,Udp,Tcp> MyTransportLayer;
    typedef ApplicationLayer<MyTransportLayer,DhcpClient,Dns> MyApplicationLayer;
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

      _net->NetworkErrorEventSender.insertSubscriber(NetworkErrorEventSourceSlot::bind(this,&NetHttpClientTest::onError));

      // Initialise the stack. This will reset the PHY, initialise the MAC
      // and attempt to create a link to our link partner. Ensure your cable
      // is plugged in when you run this or be prepared to handle the error

      if(!_net->initialise(params))
        error();

      // start the ethernet MAC Tx/Rx DMA channels
      // this will trigger the DHCP transaction

      if(!_net->startup())
        error();

      // look up the IP address for www.st.com

      *_outputStream << "Looking up the DNS address for www.st.com\r\n";

      IpAddress address;
      if(!_net->dnsHostnameQuery("www.st.com",address)) {
        *_outputStream << "Failed to look up www.st.com\r\n";
        error();
      }

      *_outputStream << "Connecting to www.st.com:80\r\n";

      // create the connection

      TcpClientConnection *ptr;

      if(!_net->tcpConnect<TcpClientConnection>(address,80,ptr)) {
        *_outputStream << "Failed to connect to www.st.com\r\n";
        error();
      }

      {
        // manage the connection pointer in a scoped_ptr so it's automatically deleted (and closed)
        // when it goes out of scope

        scoped_ptr<TcpClientConnection> conn(ptr);
        HttpClient httpClient(*conn);

        // set the parameters for the HTTP GET

        httpClient.setUri("/");             // get the root document
        httpClient.setHost("www.st.com");   // host header is mandatory for HTTP/1.1

        // send it

        *_outputStream << "Sending request to server\r\n";

        if(!httpClient.sendRequest()) {
          *_outputStream << "Failed to send the request to the server\r\n";
          error();
        }

        // must have a content-length for this test call

        int32_t contentLength;

        if((contentLength=httpClient.getResponseContentLength())==-1) {
          *_outputStream << "Server did not send a Content-Length header\r\n";
          error();
        }

        // read back the response in 100 byte chunks with a 60 second timeout

        uint8_t buffer[100];
        uint32_t actuallyRead;

        *_outputStream << "Reading response body from the server\r\n";

        while(contentLength) {

          // read a chunk

          if(!conn->receive(buffer,Min(contentLength,100L),actuallyRead,60000)) {
            *_outputStream << "Timed out waiting for data from the server\r\n";
            error();
          }

          if(actuallyRead==0) {
            *_outputStream << "The remote end has closed the connection\r\n";
            error();
          }

          // push out to the USART

          _outputStream->write(buffer,actuallyRead);

          // decrease amount remaining

          contentLength-=actuallyRead;
        }

        *_outputStream << "Finished reading response body\r\n";
      }

      // finished, reset the board to try again

      for(;;);
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

  NetHttpClientTest test;
  test.run();

  // not reached
  return 0;
}
