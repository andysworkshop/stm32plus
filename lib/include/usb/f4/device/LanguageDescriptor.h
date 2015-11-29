/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * USB language descriptor. The standard supports multiple languages but in reality the first
     * code should be 0x0409 (English/US) to maintain compatibility with Windows. There's a practical
     * limit of 126 languages.
     */

    struct LanguageDescriptor {

      scoped_array<uint8_t> _descriptor;

      /**
       * Constructor
       */

      LanguageDescriptor(const uint16_t *wLanguageIds,uint8_t languageCount) {

        uint8_t length;

        // allocate for the language ids plus descriptor size/type

        length=2+(languageCount*2);
        _descriptor.reset(new uint8_t[length]);

        // set up the header

        _descriptor[0]=length;
        _descriptor[1]=USB_DESC_TYPE_STRING;

        // copy in the language ids

        memcpy(&_descriptor[2],wLanguageIds,languageCount*2);
      }


      /**
       * Get a pointer to the descriptor bytes
       * @return The descriptor bytes
       */

      uint8_t *getDescriptor() {
        return &_descriptor[0];
      }
    };
  }
}
