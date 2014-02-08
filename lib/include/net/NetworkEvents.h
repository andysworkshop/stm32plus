/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {

    /**
     * Declare the signatures for the transmit, receive, error and notification events. Splitting the
     * events out into separate slots makes it more efficient at dispatch time as there will be fewer
     * false calls.
     */

    DECLARE_EVENT_SIGNATURE(NetworkReceive,void (NetEventDescriptor&));
    DECLARE_EVENT_SIGNATURE(NetworkSend,void (NetEventDescriptor&));
    DECLARE_EVENT_SIGNATURE(NetworkError,void (NetEventDescriptor&));
    DECLARE_EVENT_SIGNATURE(NetworkNotification,void (NetEventDescriptor&));

    /**
     * Network events base classes. Just declares the event slot that
     * users can subscribe to and raise events from. The PhysicalLayer gets to inherit
     * from these because it's at the top of stack.
     */

    struct NetworkReceiveEvents {
      DECLARE_EVENT_SOURCE(NetworkReceive);
    };

    struct NetworkSendEvents {
      DECLARE_EVENT_SOURCE(NetworkSend);
    };

    struct NetworkNotificationEvents {
      DECLARE_EVENT_SOURCE(NetworkNotification);
    };

    struct NetworkErrorEvents {

      DECLARE_EVENT_SOURCE(NetworkError);

      /**
       * Set an error in the error provider and raise an event
       */

      bool setError(int provider,uint32_t code,uint32_t cause=0) const {
        NetworkErrorEventSender.raiseEvent(NetworkErrorEvent(provider,code,cause));
        return false;
      }
    };
  }
}
