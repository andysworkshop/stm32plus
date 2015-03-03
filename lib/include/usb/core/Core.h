/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /*
     * USB core template, inherits from the PHY implementation and is inherited by the
     * role (device/host) class. Any common functionality between devices and hosts will
     * be placed here.
     */

    template<class TPhy>
    class Core : public TPhy {

      public:
        struct Parameters : TPhy::Parameters {
        };

      public:
        bool initialise(Parameters& params);
    };


    /**
     * Initialise the class
     * @param params The parameters
     * @return true if it worked
     */

    template<class TPhy>
    bool Core<TPhy>::initialise(Parameters& params) {

      // no specific implementation, just call upwards

      return TPhy::initialise(params);
    }
  }
}
