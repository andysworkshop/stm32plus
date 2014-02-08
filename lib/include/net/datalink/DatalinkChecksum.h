/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * The type of checksum calculation requested
     */

    enum class DatalinkChecksum {
      NONE,                       //!< NONE: Already done
      IP_HEADER,                  //!< IP_HEADER: Just the IP header
      IP_HEADER_AND_PROTOCOL      //!< IP_HEADER_AND_PROTOCOL: IP header and protocol
    };
  }
}
