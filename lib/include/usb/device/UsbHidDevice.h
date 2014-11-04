/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /*
   * Template class for USB HID devices
   */

  template<class TPhy,class... Features>
  class UsbHidDevice : public UsbDevice<TPhy,Features...> {
  };
}
