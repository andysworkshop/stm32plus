/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/usb.h"
#include "config/spi.h"
#include "config/timer.h"
#include "config/button.h"


using namespace stm32plus;
using namespace stm32plus::usb;


/**
 * Work in progress...
 *
 * Compatible MCU:
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F407VGT6
 */

class UsbDeviceHidKeyboardTest {

  public:

    /*
     * declare a type for the the USB stack
     */

    typedef KeyboardHidDevice<
      InternalFsPhy<>,                    // use the internal full speed PHY with no features
      ManufacturerTextFeature,            // we'll supply a manufacturer name
      ProductTextFeature,                 // ... and a product name
      SerialNumberTextFeature,            // ... and a serial number
      ConfigurationTextFeature,           // ... and a config text string
      InterfaceTextFeature                // ... and an interface text string
    > MyUsb;


    /*
     * The three keyboard LEDs will go to these pins
     */

    GpioPinRef _numLock;                  // PD12 (green)
    GpioPinRef _capsLock;                 // PD13 (orange)
    GpioPinRef _scrollLock;               // PD14 (red)


    /*
     * Run the example
     */

    void run() {

      /*
       * Set up the keyboard LED indicators and switch them off
       */

      GpioD<DefaultDigitalOutputFeature<12,13,14>> pd;

      _numLock=pd[12];
      _capsLock=pd[13];
      _scrollLock=pd[14];

      _numLock.reset();
      _capsLock.reset();
      _scrollLock.reset();

      /*
       * Set up the auto-repeat pushbutton on PA0 with 500ms initial delay
       * then 150ms auto-repeat.
       */

      GpioA<DefaultDigitalInputFeature<0>> pa;
      AutoRepeatPushButton button(pa[0],true,500,150);

      /*
       * set up the parameters for the USB hid device
       */

      MyUsb::Parameters usbParams;

      usbParams.device_vid=0xDEAD;           // demo vendor ID
      usbParams.device_pid=0xBEEF;           // demo product ID
      usbParams.device_manufacturer_text="Andy's Workshop";   // see params.device_language_[ids/count] to change the languages
      usbParams.device_product_text="stm32plus one-key keyboard";
      usbParams.device_serial_text="0123456789";
      usbParams.device_configuration_text="My configuration";
      usbParams.device_interface_text="My interface";

      /*
       * Declare the USB object - this will initialise internal variables but will not
       * start the peripheral
       */

      MyUsb usb;

      /*
       * Subscribe to errors
       */

      usb.UsbErrorEventSender.insertSubscriber(UsbErrorEventSourceSlot::bind(this,&UsbDeviceHidKeyboardTest::onError));

      /*
       * Subscribe to OUT reports from the host (keyboard LED status reports)
       */

      usb.UsbEventSender.insertSubscriber(UsbEventSourceSlot::bind(this,&UsbDeviceHidKeyboardTest::onEvent));

      /*
       * Start the USB peripheral. It will run asynchronously. There is no requirement
       * for the parameters to remain in scope after the initialise call
       */

      if(!usb.initialise(usbParams))
        for(;;);      // onError() has already locked up

      char key='a';
      uint8_t report[MyUsb::KEYBOARD_HID_KEYS_REPORT_SIZE];
      memset(report,'\0',sizeof(report));

      for(;;) {

        if(button.getState()==PushButton::Pressed) {

          // send the key to the host

          report[2]=key;
          usb.keyboardSendReport(report);

          // loop through the alphabet

          if(key=='z')
            key='a';
          else
            key++;
        }
      }
    }


    /**
     * Event callback from the USB stack. Lots of stuff will come through here but
     * we're only interested in data packets from the host
     */

    void onEvent(UsbEventDescriptor& ued) {

      // reject all events that we don't care about

      if(ued.eventType!=UsbEventDescriptor::EventType::HID_DATA_OUT)
        return;

      HidSdkDataOutEvent& event(static_cast<HidSdkDataOutEvent&>(ued));

      // num,caps and scroll lock are in byte 0, bits 0..2 respectively

      _numLock.setState(!!(event.buffer[0] & 0x1));
      _capsLock.setState(!!(event.buffer[0] & 0x2));
      _scrollLock.setState(!!(event.buffer[0] & 0x4));
    }


    /**
     * USB error event received
     * @param uee the event descriptor
     */

    void onError(UsbErrorEvent& /* uee */) {

      // flash the RED led on PD5 at 1Hz

      GpioD<DefaultDigitalOutputFeature<5>> pd;

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

  UsbDeviceHidKeyboardTest test;
  test.run();

  // not reached
  return 0;
}
