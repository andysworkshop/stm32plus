/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Base class for Tcp that declares the events. Lifting this up gets us out of the trap
     * of circular dependencies.
     */

    DECLARE_EVENT_SIGNATURE(TcpReceive,void (TcpSegmentEvent&));

    struct TcpEvents {
      DECLARE_EVENT_SOURCE(TcpReceive);
    };
  }
}
