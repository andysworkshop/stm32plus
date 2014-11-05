/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/usb.h"


using namespace stm32plus;


/**
 */


template<class TUsbDevice>
struct TestFeature {

  struct Parameters {
  };

  TestFeature(TUsbDevice&,const Parameters&) {}
};


class UsbDeviceHidTest {

  public:

    void run() {

      UsbHidDevice<UsbInternalFsPhy,TestFeature>::Parameters params;
      UsbHidDevice<UsbInternalFsPhy,TestFeature> usb(params);

      for(;;);
    }
};


/*
 * Main entry point
 */

int main() {

  UsbDeviceHidTest test;
  test.run();

  // not reached
  return 0;
}
