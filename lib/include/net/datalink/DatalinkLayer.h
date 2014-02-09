/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Template feature-holder for features that live in the datalink
     * network layer, e.g. MAC. The layer below this is the physical layer
     * and the layer above is the network layer.
     */

    template<class TPhysicalLayer,template<class> class... Features>
    class DatalinkLayer : public virtual TPhysicalLayer,
                          public Features<TPhysicalLayer>... {

      public:

        /**
         * Base parameters collection-class for datalink features
         */

        struct Parameters : TPhysicalLayer::Parameters,
                            Features<TPhysicalLayer>::Parameters... {
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

    template<class TPhysicalLayer,template<class> class... Features>
    bool DatalinkLayer<TPhysicalLayer,Features...>::initialise(Parameters& params) {
      return TPhysicalLayer::initialise(params) &&
             RecursiveBoolInitWithParams<DatalinkLayer,Features<TPhysicalLayer>...>::tinit(this,params);
    }


    /**
     * Startup the components in this layer
     * @return
     */

    template<class TPhysicalLayer,template<class> class... Features>
    bool DatalinkLayer<TPhysicalLayer,Features...>::startup() {
      return TPhysicalLayer::startup() &&
             RecursiveBoolStartup<DatalinkLayer,Features<TPhysicalLayer>...>::tstartup(this);
    }
  }
}
