/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

namespace stm32plus {
  namespace net {


    /**
     * HTTP version enumeration
     */

    enum class HttpVersion : uint8_t {
      HTTP_1_0,         // version 1.0 (1 request per connection)
      HTTP_1_1          // version 1.1 (connection keep alive)
    };
  }
}
