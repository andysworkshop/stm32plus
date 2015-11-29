/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * USB endpoint descriptor
     */

    struct EndpointDescriptor {

      DescriptorHeader header;

      /**
       * Endpoint Address
       *    Bits 0..3b Endpoint Number.
       *    Bits 4..6b Reserved. Set to Zero
       *    Bits 7 Direction 0 = Out, 1 = In (Ignored for Control Endpoints)
       */

      enum {
        OUT = 0x00,
        IN  = 0x80
      };

      uint8_t bEndpointAddress;

      /**
       * Bitmap
       *  Bits 0..1 Transfer Type
       *    00 = Control
       *    01 = Isochronous
       *    10 = Bulk
       *    11 = Interrupt
       *  Bits 2..7 are reserved. If Isochronous endpoint,
       *  Bits 3..2 = Synchronisation Type (Iso Mode)
       *    00 = No Synchonisation
       *    01 = Asynchronous
       *    10 = Adaptive
       *    11 = Synchronous
       *  Bits 5..4 = Usage Type (Iso Mode)
       *    00 = Data Endpoint
       *    01 = Feedback Endpoint
       *    10 = Explicit Feedback Data Endpoint
       *    11 = Reserved
       */

      enum {
        CONTROL = 0,
        ISOCHRONOUS = 1,
        BULK = 2,
        INTERRUPT = 3,

        SYNC_NONE = 0,
        SYNC_ASYNC = 1 << 2,
        SYNC_ADAPTIVE = 2 << 2,
        SYNC_SYNC = 3 << 2,

        USAGE_DATA = 0,
        USAGE_FEEDBACK = 1 << 4,
        USAGE_EXPLICIT_FEEDBACK_DATA = 2 << 4
      };

      uint8_t bmAttributes;

      uint16_t wMaxPacketSize;  // Maximum Packet Size this endpoint is capable of sending or receiving
      uint8_t bInterval;        // Interval for polling endpoint data transfers. Value in frame counts. Ignored for Bulk & Control Endpoints. Isochronous must equal 1 and field may range from 1 to 255 for interrupt endpoints.

      EndpointDescriptor()
        : header(sizeof(EndpointDescriptor),USB_DESC_TYPE_ENDPOINT) {

        static_assert(sizeof(EndpointDescriptor)==7,"Compiler error: sizeof(EndpointDescriptor)!=7");

        bInterval=0;
      }
    } __attribute__((packed));
  }
}
