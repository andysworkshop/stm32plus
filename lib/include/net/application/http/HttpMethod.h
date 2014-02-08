/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

namespace stm32plus {
  namespace net {


    /**
     * HTTP method constants
     */

    enum class HttpMethod : uint8_t {
      OPTIONS,    //!< OPTIONS
      GET,        //!< GET
      HEAD,       //!< HEAD
      POST,       //!< POST
      PUT,        //!< PUT
      DELETE,     //!< DELETE
      TRACE,      //!< TRACE
      CONNECT     //!< CONNECT
    };
  }
}
