/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {

    /**
     * Do-nothing container that holds all the base structures that are required by
     * all layers of the network stack. This is convenient for passing as a reference
     * to utility classes that are not directly linked into the stack but do need to
     * make use of its events and clock during their lifetime.
     */

    struct NetworkUtilityObjects : virtual NetworkReceiveEvents,
                                   virtual NetworkSendEvents,
                                   virtual NetworkErrorEvents,
                                   virtual NetworkNotificationEvents,
                                   virtual NetworkIntervalTicker,
                                   virtual DefaultRng {
    };
  }
}
