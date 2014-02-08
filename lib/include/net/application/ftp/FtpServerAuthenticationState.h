/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * The client's authentication state
     */

    enum class FtpServerAuthenticationState : uint8_t {
      STARTING,           ///< The server is starting up
      UNAUTHENTICATED,    ///< The server is ready to get credentials
      AUTHENTICATED       ///< The user has authenticated
    };
  }
}
