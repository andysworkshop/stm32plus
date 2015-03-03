/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {


    /**
     * Simple base class for PHY features. Enables access to the events
     * raised by the stack.
     */

    class PhyFeatureBase {
      protected:
        UsbEventSource& _eventSource;

      protected:
        PhyFeatureBase(UsbEventSource& eventSource);
    };


    /**
     * Constructor
     * @param eventSource The event source
     */

    inline PhyFeatureBase::PhyFeatureBase(UsbEventSource& eventSource)
      : _eventSource(eventSource) {
    }
  }
}
