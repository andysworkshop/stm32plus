/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/usb.h"


using namespace stm32plus;
using namespace stm32plus::usb;


/**
 */

class UsbDeviceHidTest {

  public:

    // declare the USB stack

    typedef MouseHidDevice<
      InternalFsPhy<                      // use the internal full speed PHY
        FsLowPowerFeature                 // include the low power EXTI wakeup
      >,
      ManufacturerTextFeature,            // we'll supply a manufacturer name
      ProductTextFeature,                 // ... and a product name
      SerialNumberTextFeature,            // ... and a serial number
      ConfigurationTextFeature,           // ... and a config text string
      InterfaceTextFeature                // ... and an interface text string
    > MyUsb;

    void run() {

      MyUsb::Parameters params;

      params.device_vid=0xDEAD;
      params.device_pid=0xBEEF;
      params.device_manufacturer_text="Andy's Workshop";   // see params.device_language_[ids/count] to change the languages
      params.device_product_text="stm32plus";
      params.device_serial_text="0123456789";
      params.device_configuration_text="My configuration";
      params.device_interface_text="My interface";

      MyUsb usb;

      // there is no requirement for the parameters to remain in scope after the initialise call

      if(!usb.initialise(params))
        error();

      for(;;);
    }


    void error() {
      for(;;);
    }
};


/*
 * Main entry point
 */

int main() {

  // initialise the interrupt controller

  Nvic::initialise();

  // initialise the millisecond timer

  MillisecondTimer::initialise();

  // run the test

  UsbDeviceHidTest test;
  test.run();

  // not reached
  return 0;
}
