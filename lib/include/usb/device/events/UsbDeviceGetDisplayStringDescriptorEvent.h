/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {

  /**
   * Get an optional string descriptor. The appropriate feature class will
   * process this event
   */

  struct UsbDeviceGetDisplayStringDescriptorEvent : UsbDeviceEventDescriptor {

    /*
     * Inputs
     */

    USBD_SpeedTypeDef speed;
    uint8_t stringIndex;

    /*
     * Outputs
     */

    uint16_t *string;
    uint16_t length;


    UsbDeviceGetDisplayStringDescriptorEvent(USBD_SpeedTypeDef s,uint8_t stringIndex)
      : UsbDeviceEventDescriptor(EventType::GET_STRING_DESCRIPTOR),
        speed(s),
        stringIndex(stringIndex),
        string(nullptr),
        length(0) {
    }
  };
}
