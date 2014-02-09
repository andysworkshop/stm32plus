/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {

    /**
     * Convenience template to allow a network stack to be declared with a sensibly named
     * root object (NetworkStack) instead of the top layer (Application)
     */

    template<class TApplicationLayer>
    class NetworkStack : public TApplicationLayer {

      public:

        /*
         * We have no parameters of our own
         */

        typedef typename TApplicationLayer::Parameters Parameters;

      public:
        bool initialise(Parameters& params);
        bool startup();
    };


    /**
     * Initialise. Call upwards.
     * @param params The parameters to be passed upwards
     * @return the result of all the initialisation
     */

    template<class TApplicationLayer>
    bool NetworkStack<TApplicationLayer>::initialise(Parameters& params) {
      return TApplicationLayer::initialise(params);
    }


    /**
     * Start up the stack
     * @return
     */

    template<class TApplicationLayer>
    bool NetworkStack<TApplicationLayer>::startup() {
      return TApplicationLayer::startup();
    }
  }
}
