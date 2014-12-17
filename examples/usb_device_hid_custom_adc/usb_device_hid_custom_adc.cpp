/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/usb/device/hid.h"
#include "config/adc.h"
#include "CustomAdcHidDevice.h"


using namespace stm32plus;
using namespace stm32plus::usb;


/*
 * This example defines a custom HID that we will use to send ADC conversion results from the
 * STM32 to the PC USB host. We configure ADC1, channels 0,1,2 to convert once per second using
 * DMA to tell us when the data is ready to send. Channels 0,1,2 are mapped to PA0, PA1, PA2.
 * This custom HID has a single interrupt IN endpoint, the minimum required by the HID standard.
 *
 * The key to a custom HID is its report descriptor which is used to define the format of the
 * data sent back to the host. We declare a single host input report with an ID of 1 and a
 * following sequence of 6 bytes that represent the converted values.
 *
 * To use, compile and flash the example to your STM32F4DISCOVERY board. Connect the USB device
 * to a PC host and it will be automtically recognised. Unlike mice and keyboards, custom devices
 * will be suspended by the host until an application opens the device for use. In this state
 * you will see the red LED on the discovery board flash briefly once per second.
 *
 * A sample Windows application can be found in the "hidreader" subdirectory that will connect
 * to the running device and log the incoming reports. Full source code is supplied and I've also
 * included a compiled exe if you don't happen to have Visual Studio 2013 or better.
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
 * will be 7 bytes long. It would be more true to the data to declare 3x12 bit data items and 4-bits
 * of padding to round it up but keeping it all byte-oriented makes it easier to pack and unpack.
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
     * Declare the ADC peripheral with an APB2 clock prescaler of 2, a resolution of
     * 12 bits. We will use 144-cycle conversions on ADC channels 0,1 and 2. Scan mode is used
     * with the default template parameter that causes EOC to be raised at the end of a complete
     * conversion group.
     */

    typedef Adc1<
      AdcClockPrescalerFeature<2>,                // prescaler of 2
      AdcResolutionFeature<12>,                   // 12 bit resolution
      Adc1Cycle144RegularChannelFeature<0,1,2>,   // using channels 0,1,2 on ADC1 with 144-cycle latency
      AdcScanModeFeature<>                        // scan mode with EOC after each group
    > MyAdc;

    /*
     * Declare the ADC1 DMA channel. The default is circular mode for the AdcDmaFeature
     * which means that it wil automatically refill our buffer on each conversion because
     * one conversion exactly matches the size of the memory buffer that we will give
     * to the DMA peripheral.
     */

    typedef Adc1DmaChannel<AdcDmaFeature<Adc1PeripheralTraits>,Adc1DmaChannelInterruptFeature> MyDma;

    /*
     * Declare the ADC conversion buffer and the flag that tells us when data is ready
     */

    uint16_t _values[3];
    volatile bool _ready;


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
       * Subscribe to USB errors
       */

      usb.UsbErrorEventSender.insertSubscriber(
          UsbErrorEventSourceSlot::bind(this,&UsbDeviceCustomAdcTest::onError)
      );

      /*
       * Declare the DMA and ADC instances
       */

      MyDma dma;
      MyAdc adc;

      _ready=false;

      /*
       * Subscribe to the DMA complete interrupt
       */

      dma.DmaInterruptEventSender.insertSubscriber(
          DmaInterruptEventSourceSlot::bind(this,&UsbDeviceCustomAdcTest::onComplete)
      );

      /*
       * Enable the DMA interrupt
       */

      dma.enableInterrupts(Adc1DmaChannelInterruptFeature::COMPLETE);

      /*
       * start the DMA (i.e. make it ready to receive requests from the ADC peripheral)
       */

      dma.beginRead(_values,3);

      /*
       * Go into an infinite loop converting
       */

      for(;;) {

        // wait a second

        MillisecondTimer::delay(1000);

        /*
         * Start a conversion and wait until the interrupt handler tells us that it's finished.
         */

        adc.startRegularConversion();

        while(!_ready);
        _ready=false;

        /*
         * Send the converted values as a report to the USB host
         */

        usb.sendAdcReport(_values[0],_values[1],_values[2]);
      }
    }


    /**
     * Interrupt handler for the DMA complete event. Set the ready flag when it's received
     */

    void onComplete(DmaEventType det) {
      if(det==DmaEventType::EVENT_COMPLETE)
        _ready=true;
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
