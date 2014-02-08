/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Template feature-holder for features that live in the network
     * layer of the stack, e.g. the IP and ICMP protocols. The next level down is the
     * data link layer and the next level up is the transport layer. The features
     * must themselves be templated on, and virtually inherit from TDatalinkLayer so
     * that the stack is correctly formed.
     */

    template<class TDatalinkLayer,template<class> class... Features>
    class NetworkLayer : public virtual TDatalinkLayer,
                         public Features<TDatalinkLayer>... {

      public:

        /**
         * Base parameters collection-class for network features
         */

        struct Parameters : TDatalinkLayer::Parameters,
                            Features<TDatalinkLayer>::Parameters... {
        };


      public:
        bool startup();
        bool initialise(Parameters& params);
    };


    /**
     * Initialise the features after initialising upwards first
     * @param params The parameters class to initialise the layer features
     * @return true if it worked
     */

    template<class TDatalinkLayer,template<class> class... Features>
    bool NetworkLayer<TDatalinkLayer,Features...>::initialise(Parameters& params) {
      return TDatalinkLayer::initialise(params) &&
             RecursiveBoolInitWithParams<NetworkLayer,Features<TDatalinkLayer>...>::tinit(this,params);
    }


    /**
     * Startup the components in this layer
     * @return
     */

    template<class TDatalinkLayer,template<class> class... Features>
    bool NetworkLayer<TDatalinkLayer,Features...>::startup() {
      return TDatalinkLayer::startup() &&
             RecursiveBoolStartup<NetworkLayer,Features<TDatalinkLayer>...>::tstartup(this);
    }
  }
}
