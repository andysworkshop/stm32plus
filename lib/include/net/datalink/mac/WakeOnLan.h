/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
	namespace net {

		/**
		 * Class to implement Wake-on-LAN (WOL) support using the magic frame feature
		 * of the MAC.
		 * @tparam TPhysicalLayer The name of the type implementing the physical layer
		 */

		template<class TPhysicalLayer>
		class WakeOnLan : public virtual TPhysicalLayer {

			public:
				struct Parameters {
				};

			public:
				bool initialise(const Parameters& params);
				bool startup();

				bool wakeOnLanSleep();
		};


		/**
		 * Initialise the class - nothing to do
		 * @return true;
		 */

		template<class TPhysicalLayer>
		inline bool WakeOnLan<TPhysicalLayer>::initialise(const Parameters&) {
			return true;
		}


		/**
		 * startup the class - nothing to do
		 * @return true
		 */

		template<class TPhysicalLayer>
		inline bool WakeOnLan<TPhysicalLayer>::startup() {
			return true;
		}


		/**
		 * Put the MAC to sleep. This follows the procedure documented in RM0090.
		 * @return true
		 */

		template<class TPhysicalLayer>
		inline bool WakeOnLan<TPhysicalLayer>::wakeOnLanSleep() {

			// 1. Disable the transmit DMA

			ETH_DMATransmissionCmd(DISABLE);

			// wait for any already-transmitting frame to complete

			return true;
		}
	}
}
