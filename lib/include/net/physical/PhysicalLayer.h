/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {

    /**
     * Template feature-holder for features that live in the physical network layer, e.g. PHY.
     * The physical layer is the lowest level of the layers that we support. There is nothing
     * below it and the level above is the datalink layer. A PHY class implementation is required
     * and all the features are implemented as templates parameterised with the PHY type and receiving
     * a reference to the PHY as a parameter to initialise()
     */

    template<class TPhy,template <class> class... Features>
    class PhysicalLayer : public TPhy,
                          public Features<TPhy>...,
                          public virtual NetworkUtilityObjects {
      public:

        /**
         * Base parameters collection-class for physical layer features
         */

        struct Parameters : TPhy::Parameters,
                            Features<TPhy>::Parameters...,
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

    template<class TPhy,template <class> class... Features>
    bool PhysicalLayer<TPhy,Features...>::initialise(Parameters& params) {

      // initialise the interval ticker

      if(!NetworkIntervalTicker::initialise(params))
        return false;

      // initialise the PHY

      if(!TPhy::initialise(params,*this))
        return false;

      // recursively initialise the features

      return RecursivePhysicalLayerInit<PhysicalLayer,TPhy,Features<TPhy>...>::tinit(this,params,*this,*this);
    }


    /**
     * Startup the components in this layer
     * @return
     */

    template<class TPhy,template <class> class... Features>
    bool PhysicalLayer<TPhy,Features...>::startup() {

      // startup the interval ticker

      if(!NetworkIntervalTicker::startup())
        return false;

      // startup the PHY

      if(!TPhy::startup())
        return false;

      // recursively startup the features

      return RecursiveBoolStartup<PhysicalLayer,TPhy,Features<TPhy>...>::tstartup(this);
    }
  }
}
