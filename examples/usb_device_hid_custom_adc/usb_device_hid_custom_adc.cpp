/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/usb.h"
#include "config/adc.h"
#include "CustomAdcHidDevice.h"


using namespace stm32plus;
using namespace stm32plus::usb;


/*
 * Work in progress... Not ready yet.
 *
 * Compatible MCU:
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F407VGT6 / Windows 8.1 x64 host
 */


/**
 * Declare the HID report descriptor. This descriptor tells the host how we expect to communicate
 * with it. Once defined, these descriptors can be hard-coded into embedded devices. Search the
 * internet for "HID Descriptor Tool" for a free tool that helps define these structures.
 *
 * This descriptor tells the host that we will send reports with an ID of #1 followed by a sequence
 * of 6 (REPORT_COUNT) 8-bit (REPORT_SIZE) data items. Therefore the actual packet sent to the host
 * will be 7 bytes long.
 */

const uint8_t CustomAdcHidReportDescriptor[22] = {
  0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
  0x09, 0x00,                    // USAGE (Undefined)
  0xa1, 0x01,                    // COLLECTION (Application)
  0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
  0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
  0x85, 0x01,                    //   REPORT_ID (1)
  0x75, 0x08,                    //   REPORT_SIZE (8)
  0x95, 0x06,                    //   REPORT_COUNT (6)
  0x09, 0x00,                    //   USAGE (Undefined)
  0x81, 0x82,                    //   INPUT (Data,Var,Abs,Vol)
  0xc0                           // END_COLLECTION
};


/**
 * Main program class
 */

class UsbDeviceCustomAdcTest {

  public:

    /*
     * declare a type for the the USB stack
     */

    typedef CustomAdcHidDevice<
      InternalFsPhy<>,                    // use the internal full speed PHY with no features
      ManufacturerTextFeature,            // we'll supply a manufacturer name
      ProductTextFeature,                 // ... and a product name
      SerialNumberTextFeature,            // ... and a serial number
      ConfigurationTextFeature,           // ... and a config text string
      InterfaceTextFeature                // ... and an interface text string
    > MyUsb;


    /*
     * Run the example
     */

    void run() {

      /*
       * set up the parameters for the USB hid device. Do not attempt to reuse vid/pid combinations unless
       * you know how to flush your PC's USB driver cache because Windows caches the characteristics of each
       * device and will suspend your device if it suddenly re-appears as a different device type.
       */

      MyUsb::Parameters usbParams;

      usbParams.device_vid=0xDEAD;           // demo vendor ID
      usbParams.device_pid=0x0004;           // demo product ID

      usbParams.device_manufacturer_text="Andy's Workshop";   // see params.device_language_[ids/count] to change the languages
      usbParams.device_product_text="ADC input device";
      usbParams.device_serial_text="0123456789";
      usbParams.device_configuration_text="My configuration";
      usbParams.device_interface_text="My interface";

      /*
       * Declare the USB object - this will initialise internal variables but will not
       * start the peripheral
       */

      MyUsb usb;

      /*
       * Start the USB peripheral. It will run asynchronously. There is no requirement
       * for the parameters to remain in scope after the initialise call
       */

      if(!usb.initialise(usbParams))
        for(;;);      // onError has already locked up

      /*
       * Subscribe to errors
       */

      usb.UsbErrorEventSender.insertSubscriber(UsbErrorEventSourceSlot::bind(this,&UsbDeviceCustomAdcTest::onError));

      for(;;) {

        MillisecondTimer::delay(1000);

        usb.sendAdcReport(1,2,3);
      }
    }


    /**
     * USB error event received
     * @param uee the event descriptor
     */

    void onError(UsbErrorEvent& uee) {

      // flash the RED led on PD5 at 1Hz

      GpioD<DefaultDigitalOutputFeature<5>> pd;

      // flash on

      if(uee.provider==ErrorProvider::ERROR_PROVIDER_USB_DEVICE) {

        if(uee.code==MyUsb::E_UNCONFIGURED) {

          // unconfigured - pulse for 50ms

          pd[5].reset();
          MillisecondTimer::delay(50);
          pd[5].set();
          MillisecondTimer::delay(50);

          return;
        }
        else if(uee.code==MyUsb::E_BUSY) {

          // unconfigured - pulse twice for 100ms

          for(int i=0;i<2;i++) {
            pd[5].reset();
            MillisecondTimer::delay(50);
            pd[5].set();
            MillisecondTimer::delay(50);
          }

          return;
        }
      }

      for(;;) {
        pd[5].reset();
        MillisecondTimer::delay(500);
        pd[5].set();
        MillisecondTimer::delay(500);
      }
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

  UsbDeviceCustomAdcTest test;
  test.run();

  // not reached
  return 0;
}
