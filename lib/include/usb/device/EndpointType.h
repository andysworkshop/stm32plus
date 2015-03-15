/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /*
     * Endpoint type enumeration. This matches the SDK enumeration in usbd_def.h
     */

    enum class EndpointType : uint8_t {
      CONTROL     = 0,
      ISOCHRONOUS = 1,
      BULK        = 2,
      INTERRUPT   = 3
    };
  }
}
