/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Template feature-holder for features that live in the transport
     * layer of the stack, e.g. the TCP and UDP protocols. The next level
     * down is the network layer and the next level up is the application
     * layer (we omit session and presentation layers). The features
     * must themselves be templated on, and virtually inherit from TNetworkLayer
     * so that the stack is correctly formed.
     */

    template<class TNetworkLayer,template<class> class... Features>
    class TransportLayer : public virtual TNetworkLayer,
                           public Features<TNetworkLayer>... {

      public:

        /**
         * Base parameters collection-class for transport features
         */

        struct Parameters : TNetworkLayer::Parameters,
                            Features<TNetworkLayer>::Parameters... {
        };

      public:
        bool initialise(Parameters& params);
        bool startup();
    };


    /**
     * Initialise the features after initialising upwards first
     * @param params The parameters class to initialise the layer features
     * @return true if it worked
     */

    template<class TNetworkLayer,template<class> class... Features>
    bool TransportLayer<TNetworkLayer,Features...>::initialise(Parameters& params) {
      return TNetworkLayer::initialise(params) &&
             RecursiveBoolInitWithParams<TransportLayer,Features<TNetworkLayer>...>::tinit(this,params);
    }


    /**
     * Startup the components in this layer
     * @return
     */

    template<class TNetworkLayer,template<class> class... Features>
    bool TransportLayer<TNetworkLayer,Features...>::startup() {
      return TNetworkLayer::startup() &&
             RecursiveBoolStartup<TransportLayer,Features<TNetworkLayer>...>::tstartup(this);
    }
  }
}
