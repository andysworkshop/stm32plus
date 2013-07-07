/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
	namespace net {

		/**
		 * Template feature-holder for features that live in the physical
		 * network layer, e.g. PHY. The physical layer is the lowest level
		 * of the layers that we support. There is nothing below it and the level
		 * above is the datalink layer.
		 */

		template<typename... Features>
		class PhysicalLayer : public Features...,
													public virtual NetworkUtilityObjects {
			public:

			  /**
				 * Base parameters collection-class for datalink features
				 */

				struct Parameters : Features::Parameters...,
														NetworkIntervalTicker::Parameters {
				};

			public:
				bool initialise(Parameters& params);
				bool startup();
		};


		/**
		 * Initialise the features
		 * @param params The parameters class to initialise the layer features
		 * @return true if it worked
		 */

		template<typename... Features>
		bool PhysicalLayer<Features...>::initialise(Parameters& params) {

			// initialise the interval ticker

			if(!NetworkIntervalTicker::initialise(params))
				return false;

			// recursively initialise the features

			return RecursiveBoolInitWithParamsAndNetworkUtilityObjects<PhysicalLayer,Features...>::tinit(this,params,*this);
		}


		/**
		 * Startup the components in this layer
		 * @return
		 */

		template<typename... Features>
		bool PhysicalLayer<Features...>::startup() {

			// startup the interval ticker

			if(!NetworkIntervalTicker::startup())
				return false;

			// recursively startup the features

			return RecursiveBoolStartup<PhysicalLayer,Features...>::tstartup(this);
		}
	}
}
