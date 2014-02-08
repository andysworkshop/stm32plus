/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Template feature-holder for features that live in the application
     * layer of the stack, e.g. DNS, HTTP, FTP, DHCP protocols. The next level
     * down is the transport layer and there is no layer above this. The features
     * must themselves be templated on, and virtually inherit from TTransportLayer
     * so that the stack is correctly formed.
     */

    template<class TTransportLayer,template<class> class... Features>
    class ApplicationLayer : public virtual TTransportLayer,
                             public Features<TTransportLayer>... {

      public:

        /**
         * Base parameters collection-class for application features
         */

        struct Parameters : TTransportLayer::Parameters,
                            Features<TTransportLayer>::Parameters... {
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

    template<class TTransportLayer,template<class> class... Features>
    bool ApplicationLayer<TTransportLayer,Features...>::initialise(Parameters& params) {
      return TTransportLayer::initialise(params) &&
             RecursiveBoolInitWithParams<ApplicationLayer,Features<TTransportLayer>...>::tinit(this,params);
    }


    /**
     * Startup the components in this layer
     * @return
     */

    template<class TTransportLayer,template<class> class... Features>
    bool ApplicationLayer<TTransportLayer,Features...>::startup() {
      return TTransportLayer::startup() &&
             RecursiveBoolStartup<ApplicationLayer,Features<TTransportLayer>...>::tstartup(this);
    }
  }
}
