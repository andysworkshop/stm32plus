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
 * This examples demonstrates the use of the DHCP client to
 * fetch your IP address, subnet mask, default gateway and
 * DNS servers.
 *
 * A DHCP lease is time limited. Half way through the lease time
 * you will receive a notification event informing you that the
 * DHCP client is going to automatically renew the lease. An error
 * notification will be raised if the renewal fails.
 *
 * The results and any errors are sent to USART3 (57000/8/N/1).
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
 * This example is also tested using the KSZ8051MLL in MII mode
 * instead of the DP83848C/RMII. The KSZ8051MLL test was performed
 * on the STM32F107. The DP83848C was tested on the STM32F407. To
 * reconfigure this demo for the F107 using remapped MAC pins connected
 * to the KSZ8051MLL change the physical and datalink layers thus:
 *
 * typedef PhysicalLayer<KSZ8051MLL> MyPhysicalLayer;
 * typedef DatalinkLayer<MyPhysicalLayer,RemapMiiInterface,Mac> MyDatalinkLayer;
 *
 * Tested on devices:
 *   STM32F107VCT6
 *   STM32F407VGT6
 */

class NetDhcpClientTest {

	public:

		/*
		 * Define the layers that make up the networking stack
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
			// some way back in 1970 but that doesn't matter to us

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

			// spy on the DHCP announcements for IP address, subnet mask, default gateway and DNS servers
			// by subscribing to the notification events passed around the network stack. we will
			// also receive notification that the DHCP lease will be renewed via this event.

			_net->NetworkNotificationEventSender.insertSubscriber(NetworkNotificationEventSourceSlot::bind(this,&NetDhcpClientTest::onNotification));

			// subscribe to error events from the network stack

			_net->NetworkErrorEventSender.insertSubscriber(NetworkErrorEventSourceSlot::bind(this,&NetDhcpClientTest::onError));

			// Initialise the stack. This will reset the PHY, initialise the MAC
			// and attempt to create a link to our link partner. Ensure your cable
			// is plugged in when you run this or be prepared to handle the error

			if(!_net->initialise(params))
				error();

			// start the ethernet MAC Tx/Rx DMA channels
			// this will trigger the DHCP transaction

			if(!_net->startup())
				error();

			// nothing more to do

			for(;;);
		}


		/**
		 * A notification event was received from the stack. Check for lease renewal
		 * notification and new address announcements. This could be IRQ code.
		 * @param ned The event descriptor base class
		 */

		void onNotification(NetEventDescriptor& ned) {

			char buf[30];
			uint8_t i;

			if(ned.eventType==NetEventDescriptor::NetEventType::DHCP_RENEWAL_DUE)
				*_outputStream << "DHCP lease renewal is happening";
			else if(ned.eventType==NetEventDescriptor::NetEventType::IP_ADDRESS_ANNOUNCEMENT) {
				static_cast<IpAddressAnnouncementEvent&>(ned).ipAddress.toString(buf);
				*_outputStream << "IP Address: " << buf << "\r\n";
			}
			else if(ned.eventType==NetEventDescriptor::NetEventType::SUBNET_MASK_ANNOUNCEMENT) {
				static_cast<IpSubnetMaskAnnouncementEvent&>(ned).subnetMask.toString(buf);
				*_outputStream << "Subnet mask: " << buf << "\r\n";
			}
			else if(ned.eventType==NetEventDescriptor::NetEventType::DEFAULT_GATEWAY_ANNOUNCEMENT) {
				static_cast<IpDefaultGatewayAnnouncementEvent&>(ned).defaultGateway.toString(buf);
				*_outputStream << "Default gateway: " << buf << "\r\n";
			}
			else if(ned.eventType==NetEventDescriptor::NetEventType::DNS_SERVERS_ANNOUNCEMENT) {

				IpDnsServersAnnouncementEvent& dnsevent(static_cast<IpDnsServersAnnouncementEvent&>(ned));

				for(i=0;i<3;i++) {
					if(dnsevent.ipDnsServers[i].isValid()) {
						dnsevent.ipDnsServers[i].toString(buf);
						*_outputStream << "DNS server: " << buf << "\r\n";
					}
				}
			}
		}


		/**
		 * Network error event received, report it
		 * @param ned The event descriptor base class
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

	NetDhcpClientTest test;
	test.run();

	// not reached
	return 0;
}
