/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Event descriptor for a debug message.
     */

    struct NetworkDebugEvent : NetEventDescriptor {

      const char *message;

      /**
       * Constructor
       * @param msg Message to send
       */

      NetworkDebugEvent(const char *msg)
        : NetEventDescriptor(NetEventDescriptor::NetEventType::DEBUG_MESSAGE),
          message(msg) {
      }
    };
  }
}
