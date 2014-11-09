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

    // declare the interrupt endpoint that we'll use to send data into the host

    template<class TDevice> using Endpoint1=InterruptInEndpointFeature<1,TDevice>;

    // declare the USB stack

    typedef HidDevice<
      InternalFsPhy,                   // use the internal full speed PHY
      ManufacturerTextFeature,         // we'll supply a manufacturer name
      ProductTextFeature,              // ... and a product name
      SerialNumberTextFeature,         // ... and a serial number
      ConfigurationTextFeature,        // ... and a config text string
      InterfaceTextFeature,            // ... and an interface text string
      Endpoint1                        // the interrupt endpoint
    > MyUsb;

    void run() {

      MyUsb::Parameters params;

      params.device_vid=0xDEAD;
      params.device_pid=0xBEEF;
      params.device_manufacturer_text="Andy's Workshop";   // see params.device_language_id to change the language
      params.device_product_text="Joystick";
      params.device_serial_text="1234567890";
      params.device_configuration_text="My configuration";
      params.device_interface_text="My interface";

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
