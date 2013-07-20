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
 * This examples demonstrates the use of the Link Local IP client
 * to automatically select an unused IP address from the "link local"
 * class B network: 169.254/16. Link-local addresses can be used
 * in a scenario where a DHCP server is not available, such as when
 * a number of computers are directly connected to each other.
 *
 * If an address collision is detected while the stack is running then
 * a new address will be automatically generated and broadcast to the
 * stack modules. You can simulate this condition by using the linux
 * NPING utility to hand-craft a colliding ARP packet. For example, if
 * you have been assigned 169.254.232.52:
 *
 * $ nping 169.254.232.52 --arp --arp-type arp-request --arp-sender-mac \
 *   00:0a:34:30:fe:ae --arp-sender-ip 169.254.232.52 --arp-target-mac \
 *   00:00:00:00:00:00 --arp-target-ip 169.254.232.52
 *
 * You can then use tcpdump or wireshark to observe this module searching
 * for a new address.
 *
 * The results and any errors are sent to USART3 (57000/8/N/1).
 *
 * Here's how the network stack for this example is configured:
 *
 *              +----------------------------+
 * APPLICATION: | LinkLocalIp                |
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

class NetLlipClientTest {

	public:

		/*
		 * Define the layers that make up the networking stack
		 */

		typedef PhysicalLayer<DP83848C> MyPhysicalLayer;
		typedef DatalinkLayer<MyPhysicalLayer,DefaultRmiiInterface,Mac> MyDatalinkLayer;
		typedef NetworkLayer<MyDatalinkLayer,DefaultIp,Arp> MyNetworkLayer;
		typedef TransportLayer<MyNetworkLayer,Udp> MyTransportLayer;
		typedef ApplicationLayer<MyTransportLayer,LinkLocalIp> MyApplicationLayer;
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

			// spy on the LLIP announcements for IP address, subnet mask, default gateway and DNS servers
			// by subscribing to the notification events passed around the network stack. we will
			// also receive notification of any LLIP clashes here.

			_net->NetworkNotificationEventSender.insertSubscriber(NetworkNotificationEventSourceSlot::bind(this,&NetLlipClientTest::onNotification));

			// subscribe to error events from the network stack

			_net->NetworkErrorEventSender.insertSubscriber(NetworkErrorEventSourceSlot::bind(this,&NetLlipClientTest::onError));

			// Initialise the stack. This will reset the PHY, initialise the MAC
			// and attempt to create a link to our link partner. Ensure your cable
			// is plugged in when you run this or be prepared to handle the error

			if(!_net->initialise(params))
				error();

			// start the ethernet MAC Tx/Rx DMA channels
			// this will trigger the LLIP transaction

			if(!_net->startup())
				error();

			// finished

			for(;;) {
			}
		}


		/**
		 * A notification event was received from the stack. Check for lease renewal
		 * notification and new address announcements. This could be IRQ code.
		 * @param ned The event descriptor base class
		 */

		void onNotification(NetEventDescriptor& ned) {

			char buf[30];

			if(ned.eventType==NetEventDescriptor::NetEventType::IP_ADDRESS_ANNOUNCEMENT) {
				static_cast<IpAddressAnnouncementEvent&>(ned).ipAddress.toString(buf);
				*_outputStream << "IP Address: " << buf << "\r\n";
			}
			else if(ned.eventType==NetEventDescriptor::NetEventType::SUBNET_MASK_ANNOUNCEMENT) {
				static_cast<IpSubnetMaskAnnouncementEvent&>(ned).subnetMask.toString(buf);
				*_outputStream << "Subnet mask: " << buf << "\r\n";
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

	NetLlipClientTest test;
	test.run();

	// not reached
	return 0;
}
