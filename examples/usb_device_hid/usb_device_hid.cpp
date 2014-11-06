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

class UsbDeviceHidTest {

  public:

    void run() {

      typedef UsbHidDevice<
          UsbInternalFsPhy,               // use the internal full speed PHY
          UsbManufacturerFeature,         // we'll supply a manufacturer name
          UsbProductFeature,              // ... and a product name
          UsbSerialNumberFeature> MyUsb;  // ... and a serial number

      MyUsb::Parameters params;

      params.device_vid=0xDEAD;
      params.device_pid=0xBEEF;
      params.device_manufacturer="Andy's Workshop";   // see params.device_language_id to change the language
      params.device_product="Joystick";
      params.device_serial="1234567890";

      MyUsb usb(params);

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
