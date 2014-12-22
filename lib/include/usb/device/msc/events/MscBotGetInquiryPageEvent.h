/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * Sent to request the page data for a SCSI enquiry. The receiver should set enquiryPage to a
     * pointer to the enquiry response. The enquiry response is:

      +=====-========-========-========-========-========-========-========-========+
      |  Bit|   7    |   6    |   5    |   4    |   3    |   2    |   1    |   0    |
      |Byte |        |        |        |        |        |        |        |        |
      |=====+==========================+============================================|
      | 0   | Peripheral Qualifier     |           Peripheral Device Type           |
      |-----+-----------------------------------------------------------------------|
      | 1   |  RMB   |                  Device-Type Modifier                        |
      |-----+-----------------------------------------------------------------------|
      | 2   |   ISO Version   |       ECMA Version       |  ANSI-Approved Version   |
      |-----+-----------------+-----------------------------------------------------|
      | 3   |  AENC  | TrmIOP |     Reserved    |         Response Data Format      |
      |-----+-----------------------------------------------------------------------|
      | 4   |                           Additional Length (n-4)                     |
      |-----+-----------------------------------------------------------------------|
      | 5   |                           Reserved                                    |
      |-----+-----------------------------------------------------------------------|
      | 6   |                           Reserved                                    |
      |-----+-----------------------------------------------------------------------|
      | 7   | RelAdr | WBus32 | WBus16 |  Sync  | Linked |Reserved| CmdQue | SftRe  |
      |-----+-----------------------------------------------------------------------|
      | 8   | (MSB)                                                                 |
      |- - -+---                        Vendor Identification                    ---|
      | 15  |                                                                 (LSB) |
      |-----+-----------------------------------------------------------------------|
      | 16  | (MSB)                                                                 |
      |- - -+---                        Product Identification                   ---|
      | 31  |                                                                 (LSB) |
      |-----+-----------------------------------------------------------------------|
      | 32  | (MSB)                                                                 |
      |- - -+---                        Product Revision Level                   ---|
      | 35  |                                                                 (LSB) |
      |-----+-----------------------------------------------------------------------|
    */

    struct MscBotGetEnquiryPageEvent : UsbEventDescriptor {

      uint8_t lun;
      const uint8_t *enquiryPage;

      MscBotGetEnquiryPageEvent(uint8_t l)
        : UsbEventDescriptor(EventType::MSC_BOT_GET_ENQUIRY_PAGE),
          lun(l) {
      }
    };
  }
}

