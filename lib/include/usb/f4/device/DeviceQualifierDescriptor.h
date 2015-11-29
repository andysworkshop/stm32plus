/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * USB device qualifier descriptor
     */

    struct DeviceQualifierDescriptor {

      DescriptorHeader header;

      /**
       * BCD USB Specification Release Number in Binary-Coded Decimal (i.e., 2.10 is 210H).
       * This field identifies the release of the USB Specification with which the device
       * and its descriptors are compliant.
       * At least V2.00 is required to use this descriptor.
       */

      uint16_t bcdUSB;

      /**
       * Class Class code (assigned by the USB-IF).
       * If this field is reset to zero, each interface within a configuration specifies its own
       * class information and the various interfaces operate independently. If this field is set
       * to a value between 1 and FEH, the device supports different class specifications on
       * different interfaces and the interfaces may not operate independently. This value identifies
       * the class definition used for the aggregate interfaces.
       * If this field is set to FFH, the device class is vendor-specific.
       */

      uint8_t bDeviceClass;

      /**
       * SubClass  Subclass code (assigned by the USB-IF).
       * These codes are qualified by the value of the bDeviceClass field. If the bDeviceClass field
       * is reset to zero, this field must also be reset to zero. If the bDeviceClass field is not set to
       * FFH, all values are reserved for assignment by the USB-IF.
       */

      uint8_t bDeviceSubClass;

      /**
       * Protocol code (assigned by the USB-IF). These codes are qualified by the value of the bDeviceClass
       * and the bDeviceSubClass fields. If a device supports class-specific protocols on a device basis as
       * opposed to an interface basis, this code identifies the protocols that the device uses as
       * defined by the specification of the device class. If this field is reset to zero, the device
       * does not use class-specific protocols on a device basis. However, it may use classspecific
       * protocols on an interface basis. If this field is set to FFH, the device uses a vendor-specific
       * protocol on a device basis.
       */

      uint8_t bDeviceProtocol;

      uint8_t bMaxPacketSize0;      // Maximum packet size for other speed.
      uint8_t bNumConfigurations;   // Number of other-speed configurations.
      uint8_t bReserved;

      DeviceQualifierDescriptor()
        : header(sizeof(DeviceQualifierDescriptor),USB_DESC_TYPE_DEVICE_QUALIFIER) {

        static_assert(sizeof(DeviceQualifierDescriptor)==10,"Compiler error: sizeof(DeviceQualifierDescriptor)!=10");

        bcdUSB=0x0200;
        bReserved=0;
        bDeviceClass=0;
        bDeviceSubClass=0;
        bDeviceProtocol=0;
      }
    } __attribute__((packed));
  }
}
