/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /*
   * Device descriptor structure, 18 bytes
   */

  struct UsbDeviceDescriptor {

    uint8_t  bLength;
    uint8_t  bDescriptorType;
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

    UsbDeviceDescriptor() {

      static_assert(sizeof(UsbDeviceDescriptor)==USB_LEN_DEV_DESC,"Compilation error: sizeof(UsbDeviceDescriptor)!=0x12");

      bLength=USB_LEN_DEV_DESC;                       // structure size
      bDescriptorType=USB_DESC_TYPE_DEVICE;           // this is a device desc
      bDeviceClass=0;                                 // use class in interface desc
      bDeviceSubClass=0;                              // as above
      bDeviceProtocol=0;                              // as above
      bMaxPacketSize=USB_MAX_EP0_SIZE;                // max size of endpoint 0
      iManufacturer=USBD_IDX_MFC_STR;                 // Index of manufacturer string
      iProduct=USBD_IDX_PRODUCT_STR;                  // Index of product string
      iSerialNumber=USBD_IDX_SERIAL_STR;              // Index of serial number string
      bNumConfigurations=USBD_MAX_NUM_CONFIGURATION;  // bNumConfigurations

    }
  } __attribute((packed));
}
