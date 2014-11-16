/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /*
     * Device descriptor structure, 18 bytes
     */

    struct DeviceDescriptor {

      DescriptorHeader header;
      uint16_t bcdUSB;
      uint8_t  bDeviceClass;
      uint8_t  bDeviceSubClass;
      uint8_t  bDeviceProtocol;
      uint8_t  bMaxPacketSize;
      uint16_t idVendor;
      uint16_t idProduct;
      uint16_t bcdDevice;
      uint8_t  iManufacturer;
      uint8_t  iProduct;
      uint8_t  iSerialNumber;
      uint8_t bNumConfigurations;

      /**
       * Constructor - set up the constants
       */

      DeviceDescriptor()
        : header(USB_LEN_DEV_DESC,USB_DESC_TYPE_DEVICE) {

        static_assert(sizeof(DeviceDescriptor)==USB_LEN_DEV_DESC,"Compilation error: sizeof(DeviceDescriptor)!=0x12");

        bDeviceClass=0;                                 // use class in interface desc
        bDeviceSubClass=0;                              // as above
        bDeviceProtocol=0;                              // as above
        bMaxPacketSize=USB_MAX_EP0_SIZE;                // max size of endpoint 0
        iManufacturer=0;                                // index of manufacturer string
        iProduct=0;                                     // index of product string
        iSerialNumber=0;                                // index of serial number string
        bNumConfigurations=USBD_MAX_NUM_CONFIGURATION;  // bNumConfigurations

      }
    } __attribute((packed));
  }
}
