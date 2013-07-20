/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/net.h"
#include "config/net_http.h"
#include "config/display/tft.h"
#include "config/timing.h"
#include "config/usart.h"
#include "MyTcpClientConnection.h"


using namespace stm32plus;
using namespace stm32plus::net;
using namespace stm32plus::display;

extern uint32_t ErrorPixels,ErrorPixelsSize;


/**
 * This example demonstrates a cycling 'picture frame' of JPEG images
 * downloaded from the internet and displayed on the attached LCD
 * screen. The images are pre-sized to fit the QVGA screen and are
 * located in a directory on my website.
 *
 * The examples works by reading an 'index' text file from the web
 * site which tells it how many images there are and what their URLs
 * are. The example then goes into a loop retrieving each JPEG image
 * and displaying it on the LCD.
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
 * This is example is only compatible with the F4 because it
 * requires the FSMC peripheral to drive the LCD.
 *
 * Tested on devices:
 *   STM32F407VGT6
 */

class NetWebPictureFrame {

	public:

		/**
		 * Types that define the LCD that we'll be using
		 */

		typedef Fsmc16BitAccessMode<FsmcBank1NorSram1> LcdAccessMode;
		typedef ST7783_Portrait_64K<LcdAccessMode> LcdPanel;


		/**
		 * LCD member variables
		 */

		LcdAccessMode *_accessMode;
		LcdPanel *_tft;
		Font *_font;
		DefaultBacklight *_backlight;
		uint16_t _line;


		/**
		 * The RTC that the stack requires
		 */

		typedef Rtc<RtcLsiClockFeature<Rtc32kHzLsiFrequencyProvider>,RtcSecondInterruptFeature> MyRtc;
		MyRtc *_rtc;


		/**
		 * Types that define the network stack
		 */

		typedef PhysicalLayer<DP83848C> MyPhysicalLayer;
		typedef DatalinkLayer<MyPhysicalLayer,DefaultRmiiInterface,Mac> MyDatalinkLayer;
		typedef NetworkLayer<MyDatalinkLayer,DefaultIp,Arp> MyNetworkLayer;
		typedef TransportLayer<MyNetworkLayer,Udp,Tcp> MyTransportLayer;
		typedef ApplicationLayer<MyTransportLayer,DhcpClient,Dns> MyApplicationLayer;
		typedef NetworkStack<MyApplicationLayer> MyNetworkStack;


		/**
		 * The network stack object
		 */

		MyNetworkStack *_net;


		/**
		 * Program variables
		 */

		constexpr static const char *WEB_SERVER = "www.andybrown.me.uk";
		constexpr static const uint16_t WEB_SERVER_PORT = 80;
		constexpr static const char *PICTURE_INDEX_URI = "/wk/public/net_web_pframe/index.txt";

		std::slist<std::string> _pictureUriList;
		IpAddress _serverAddress;


		/**
		 * Run the picture frame
		 */

		void run() {

			// initialise the LCD and then the network

			initialiseLcd();
			initialiseNetwork();

			// do a DNS lookup for the server and read the index file

			lookupServerAddress();
			readPictureIndex();

			// give a small delay so that the user can see the startup info

			writeLine("Starting in 5 seconds");
			MillisecondTimer::delay(5000);

			// go into an infinite loop showing the pictures with a 10 second
			// delay between each one

			for(;;) {
				for(auto it=_pictureUriList.begin();it!=_pictureUriList.end();it++) {
					showPicture(*it);
					MillisecondTimer::delay(10000);
				}
			}
		}


		/**
		 * Initialise the LCD
		 */

		void initialiseLcd() {

			// reset is on PE1 and RS (D/CX) is on PE3

			GpioE<DefaultDigitalOutputFeature<1>,
						DefaultFsmcAlternateFunctionFeature<3>> pe;

			// set up the FSMC timing. these numbers (particularly the data setup time) are dependent on
			// both the FSMC bus speed and the panel timing parameters.

			Fsmc8080LcdTiming fsmcTiming(2,4);

			// set up the FSMC with RS=A19 (PE3)

			_accessMode=new LcdAccessMode(fsmcTiming,19,pe[1]);
			_tft=new LcdPanel(*_accessMode);

			// apply gamma settings

			ST7783Gamma gamma(0,0x0107,0,0x0203,0x0402,0,0x0207,0,0x0203,0x0403);
			_tft->applyGamma(gamma);

			// clear to black while the lights are out

			_tft->setBackground(ColourNames::BLACK);
			_tft->setForeground(ColourNames::WHITE);
			_tft->clearScreen();

			// fade up to 100% in 4ms steps

			_backlight=new DefaultBacklight;
			_backlight->fadeTo(100,4);

			// create a font

			_font=new Font_VOLTER__28GOLDFISH_299;
			*_tft << *_font;
			_line=0;
		}


		/**
		 * Initialise the network
		 */

		void initialiseNetwork() {

			// declare the RTC that that stack requires. it's used for cache timeouts, DHCP lease expiry
			// and such like so it does not have to be calibrated for accuracy. A few seconds here or there
			// over a 24 hour period isn't going to make any difference. Start it ticking at zero which is
			// some way back in 2000 but that doesn't matter to us

			_rtc=new MyRtc;
			_rtc->setTick(0);

			// declare an instance of the network stack

			MyNetworkStack::Parameters params;
			_net=new MyNetworkStack;

			// the stack requires the RTC

			params.base_rtc=_rtc;

			// It's nice to give the DHCP client a host name because then it'll show up in DHCP
			// 'active leases' page. In a home router this is often called 'attached devices'

			params.dhcp_hostname="stm32plus";

			// spy on the DHCP announcements for IP address, subnet mask, default gateway and DNS servers
			// by subscribing to the notification events passed around the network stack. we will
			// also receive notification that the DHCP lease will be renewed via this event.

			_net->NetworkNotificationEventSender.insertSubscriber(NetworkNotificationEventSourceSlot::bind(this,&NetWebPictureFrame::onNotification));

			// subscribe to error events from the network stack

			_net->NetworkErrorEventSender.insertSubscriber(NetworkErrorEventSourceSlot::bind(this,&NetWebPictureFrame::onError));

			// Initialise the stack. This will reset the PHY, initialise the MAC
			// and attempt to create a link to our link partner. Ensure your cable
			// is plugged in when you run this or be prepared to handle the error

			writeLine("Initialising network...");

			if(!_net->initialise(params))
				errorRestart();

			// start the ethernet MAC Tx/Rx DMA channels
			// this will trigger the DHCP transaction

			writeLine("Starting network...");

			if(!_net->startup())
				errorRestart();
		}


		/**
		 * Lookup the web server address
		 */

		void lookupServerAddress() {

			writeLine("Looking up the web server address...");

			// do the DNS query

			if(!_net->dnsHostnameQuery(WEB_SERVER,_serverAddress))
				error("Failed to lookup web server address");
		}


		/**
		 * The picture index is a text file on the web server. The format of the file is one URI per line
		 * and the last line must be "#end"
		 */

		void readPictureIndex() {

			writeLine("Connecting to server...");

			// create the connection

			MyTcpClientConnection *ptr;

			if(!_net->tcpConnect<MyTcpClientConnection>(_serverAddress,WEB_SERVER_PORT,ptr))
				error("Failed to connect to web server");

			{
				// manage the connection pointer in a scoped_ptr so it's automatically deleted (and closed)
				// when it goes out of scope

				scoped_ptr<MyTcpClientConnection> conn(ptr);
				HttpClient httpClient(*conn);

				// set the parameters for the HTTP GET

				httpClient.setUri(PICTURE_INDEX_URI);
				httpClient.setHost(WEB_SERVER);
				httpClient.setVersion(HttpVersion::HTTP_1_0);

				// send it

				writeLine("Sending request for index...");

				if(!httpClient.sendRequest())
					error("Failed to send the request to the server");

				// read the response lines up to max of 200 per line

				TcpTextLineReceiver lineReceiver(200);

				writeLine("Reading index...");
				_pictureUriList.clear();

				for(;;) {

					if(!lineReceiver.add(*conn))
						error("Failed to index data");

					if(lineReceiver.ready()) {

						if(!strcasecmp(lineReceiver.str().c_str(),"#end"))
							break;

						_pictureUriList.push_front(lineReceiver.str());
						lineReceiver.reset();
					}
				}
			}

			char buf[100];
			StringUtil::modp_uitoa10(_pictureUriList.size(),buf);
			strcat(buf," images indexed");
			writeLine(buf);
		}


		/**
		 * Read and show the JPEG
		 * @param uri The URI of the JPEG
		 */

		void showPicture(const std::string& uri) {

			// fade down the backlight to 10%, 4ms per step

			_backlight->fadeTo(10,4);

			// clear the screen

			_tft->setBackground(ColourNames::BLACK);
			_tft->clearScreen();

			// we're using a custom TCP client connection that processes incoming data on the receive IRQ
			// so that we avoid advertising a zero receive window back to the server with the performance
			// hit that we would take

			MyTcpClientConnection *conn;

			if(!_net->tcpConnect<MyTcpClientConnection>(_serverAddress,WEB_SERVER_PORT,conn))
				error("Failed to connect to web server");

			// manage the connection pointer in a scoped_ptr so it's automatically deleted (and closed)
			// when it goes out of scope

			HttpClient httpClient(*conn);

			// set the parameters for the HTTP GET

			httpClient.setUri(uri);
			httpClient.setHost(WEB_SERVER);
			httpClient.setVersion(HttpVersion::HTTP_1_0);				// connection to close after we get the image

			if(!httpClient.sendRequest()) {
				delete conn;
				error("Failed to send the request to the server");
			}

			// use a read-ahead input stream wrapped around a TCP input stream
			// with a read-ahead buffer of 256 bytes

			TcpInputStream tcis(*conn);

			// if the JPEG will fit then display it centered on screen, otherwise ignore it

			Size size;
			JpegDecoder<LcdPanel> jpeg;

			if(!jpeg.beginDecode(tcis,size)) {
				delete conn;
				error("Failed to decode JPEG image");
			}

			if(size.Height<=_tft->getHeight() && size.Width<=_tft->getWidth()) {

				// it fits, stream it in

				Point pt;

				pt.X=(_tft->getWidth()-size.Width)/2;
				pt.Y=(_tft->getHeight()-size.Height)/2;

				jpeg.endDecode(pt,*_tft);
			}

			delete conn;

			// fade up the backklight to 100%, 4ms per step

			_backlight->fadeTo(100,4);
		}


		/**
		 * A notification event was received from the stack. Check for lease renewal
		 * notification and new address announcements. This could be IRQ code.
		 * @param ned The event descriptor base class
		 */

		void onNotification(NetEventDescriptor& ned) {

			char buf[100];
			uint8_t i;

			if(ned.eventType==NetEventDescriptor::NetEventType::IP_ADDRESS_ANNOUNCEMENT) {
				strcpy(buf,"IP address: ");
				static_cast<IpAddressAnnouncementEvent&>(ned).ipAddress.toString(buf+12);
				writeLine(buf);
			}
			else if(ned.eventType==NetEventDescriptor::NetEventType::SUBNET_MASK_ANNOUNCEMENT) {
				strcpy(buf,"Subnet mask: ");
				static_cast<IpSubnetMaskAnnouncementEvent&>(ned).subnetMask.toString(buf+13);
				writeLine(buf);
			}
			else if(ned.eventType==NetEventDescriptor::NetEventType::DEFAULT_GATEWAY_ANNOUNCEMENT) {
				strcpy(buf,"Default gateway: ");
				static_cast<IpDefaultGatewayAnnouncementEvent&>(ned).defaultGateway.toString(buf+17);
				writeLine(buf);
			}
			else if(ned.eventType==NetEventDescriptor::NetEventType::DNS_SERVERS_ANNOUNCEMENT) {

				IpDnsServersAnnouncementEvent& dnsevent(static_cast<IpDnsServersAnnouncementEvent&>(ned));

				for(i=0;i<3;i++) {
					if(dnsevent.ipDnsServers[i].isValid()) {
						strcpy(buf,"DNS server: ");
						dnsevent.ipDnsServers[i].toString(buf+12);
						writeLine(buf);
					}
				}
			}
		}


		/**
		 * Write a status line to the LCD
		 * @param line The status line
		 */

		void writeLine(const std::string& line) {
			*_tft << Point(0,_line*10) << line.c_str();
			_line++;
		}


		/**
		 * Network error event received, report it and restart after a delay
		 * @param ned
		 */

		void onError(NetEventDescriptor& ned) {

			NetworkErrorEvent& errorEvent(static_cast<NetworkErrorEvent&>(ned));

			std::string str("Network error: ");
			char buf[20];

			StringUtil::modp_uitoa10(errorEvent.provider,buf);
			strcat(buf,"/");
			str+=buf;

			StringUtil::modp_uitoa10(errorEvent.code,buf);
			strcat(buf,"/");
			str+=buf;

			StringUtil::modp_uitoa10(errorEvent.cause,buf);
			str+=buf;

			_tft->setBackground(ColourNames::BLACK);
			_tft->setForeground(ColourNames::WHITE);

			_tft->clearRectangle(
					Rectangle(0,
										_tft->getHeight()-_font->getHeight(),
										_tft->getWidth(),
										_font->getHeight()));

			*_tft << Point(0,_tft->getHeight()-_font->getHeight()) << str.c_str();
		}


		/**
		 * Display the error code, then pause and reset
		 * @param str
		 */

		void error(const std::string& str) {

			_tft->setBackground(ColourNames::WHITE);
			_tft->setForeground(ColourNames::BLACK);
			_tft->clearScreen();

			LinearBufferInputOutputStream compressedData((uint8_t *)&ErrorPixels,(uint32_t)&ErrorPixelsSize);
			_tft->drawJpeg(Rectangle(95,135,50,50),compressedData);

			*_tft << Point(2,2) << str.c_str();

			errorRestart();
		}


		/**
		 * Pause then restart the MCU
		 */

		void errorRestart() {
			MillisecondTimer::delay(10000);
			NVIC_SystemReset();
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

	NetWebPictureFrame test;
	test.run();

	// not reached
	return 0;
}
