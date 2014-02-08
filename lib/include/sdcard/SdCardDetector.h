/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {

  /**
   * Helper class to assist with SD card detection
   */

  class SdCardDetector {

    protected:
      GpioPinRef _cardDetect;

    public:
      SdCardDetector(const GpioPinRef& pin);

      bool isCardPresent() const;
  };


  /**
   * Constructor
   * @param pin
   */

  inline SdCardDetector::SdCardDetector(const GpioPinRef& pin)
    : _cardDetect(pin) {
  }


  /**
   * Return true if the card is present (the line is active low)
   * @return true if present
   */

  inline bool SdCardDetector::isCardPresent() const {
    return !_cardDetect.read();
  }
}
