/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/usb/device/msc.h"


using namespace stm32plus;
using namespace stm32plus::usb;


extern uint32_t FloppyImageSize;
extern uint32_t FloppyImage;


/**
 * This is a self-contained example of the 'mass storage' USB device class. In this mode your
 * device appears as an external disk and can present a file system to the computer without actually
 * having to implement the file system. All the device needs to do is to be able to read and write
 * blocks as well as present some basic meta data and live status information about itself.
 *
 * The stm32plus driver implemnts the popular Bulk Only Transport (BOT) standard that provides
 * one IN and one OUT bulk endpoint. A SCSI command set is used to transfer data, commands, and
 * responses from and to the host.
 *
 * This example compiles in a 360Kb floppy disk image into the MCU flash and makes it available
 * as a read-only USB mass storage device. To use, simply compile and flash this example to the
 * F4 discovery board and then hook up the micro USB port to a computer. The 'flash disk' should
 * be automatically discovered and the files made available for browsing.
 *
 * Compatible MCU:
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F407VGT6 / Windows 8.1 x64 host
 */

class UsbDeviceMscInternalTest {

  public:

    /*
     * declare a type for the the USB stack
     */

    typedef BotMscDevice<
      InternalFsPhy<>,                    // use the internal full speed PHY with no features
      ManufacturerTextFeature,            // we'll supply a manufacturer name
      ProductTextFeature,                 // ... and a product name
      SerialNumberTextFeature,            // ... and a serial number
      ConfigurationTextFeature            // ... and a config text string
    > MyUsb;


    /*
     * Run the example
     */

    void run() {

      /*
       * set up the parameters for the USB mass storage device. The default MscScsi template
       * constructor sets up a media packet size of 8192 bytes. This is the the largest size
       * that the host will send/receive in read/write operations.
       */

      MyUsb::Parameters usbParams;

      usbParams.device_vid=0xDEAD;           // demo vendor ID
      usbParams.device_pid=0x0020;           // demo product ID

      usbParams.device_manufacturer_text="Andy's Workshop";   // see params.device_language_[ids/count] to change the languages
      usbParams.device_product_text="stm32plus flash drive";
      usbParams.device_configuration_text="My configuration";

      /*
       * The serial number length and format matters for the mass storage device class. Search online
       * for the rules that you have to follow.
       */

      usbParams.device_serial_text="002020141222";

      /*
       * Declare the USB object - this will initialise internal variables but will not
       * start the peripheral
       */

      MyUsb usb;

      /*
       * Subscribe to errors
       */

      usb.UsbErrorEventSender.insertSubscriber(UsbErrorEventSourceSlot::bind(this,&UsbDeviceMscInternalTest::onError));

      /*
       * Subscribe to USB events
       */

      usb.UsbEventSender.insertSubscriber(UsbEventSourceSlot::bind(this,&UsbDeviceMscInternalTest::onEvent));

      /*
       * Start the USB peripheral. It will run asynchronously. There is no requirement
       * for the parameters to remain in scope after the initialise call
       */

      if(!usb.initialise(usbParams))
        for(;;);      // onError() has already locked up

      // loop forever, or until an error interrupts us

      for(;;) {
      }
    }


    /**
     * Event callback from the USB stack. Lots of stuff will come through here but
     * we're only interested in the events that allow us to provide our device data
     * and information.
     */

    void onEvent(UsbEventDescriptor& ued) {

      switch(ued.eventType) {

        /*
         * MSC_BOT_IS_READY is used to test if the device is ready for IO. The Windows host
         * tends to poll this at least once per second.
         */

        case UsbEventDescriptor::EventType::MSC_BOT_IS_READY:
          onIsReady(static_cast<MscBotIsReadyEvent&>(ued));
          break;

        /*
         * MSC_BOT_GET_ENQUIRY_PAGE returns some basic control and metadata about the whole
         * device.
         */

        case UsbEventDescriptor::EventType::MSC_BOT_GET_ENQUIRY_PAGE:
          onGetEnquiryPage(static_cast<MscBotGetEnquiryPageEvent&>(ued));
          break;

        /*
         * MSC_BOT_GET_CAPACITY is used to return the number of blocks on your device and
         * the size of each block
         */

        case UsbEventDescriptor::EventType::MSC_BOT_GET_CAPACITY:
          onGetCapacity(static_cast<MscBotGetCapacityEvent&>(ued));
          break;

        /*
         * BOT_READ is the read operation. A block address and block count is provided.
         */

        case UsbEventDescriptor::EventType::MSC_BOT_READ:
          onRead(static_cast<MscBotReadEvent&>(ued));
          break;

        /*
         * BOT_IS_WRITE_PROTECTED tests if the media is write protected. This example is
         * always write protected
         */

        case UsbEventDescriptor::EventType::MSC_BOT_IS_WRITE_PROTECTED:
          onIsWriteProtected(static_cast<MscBotIsWriteProtectedEvent&>(ued));
          break;

        default:   // warning suppression
          break;
      }
    }


    /**
     * Read a number of blocks from the disk
     * @param event The read event parameters.
     */

    void onRead(MscBotReadEvent& event) {

      uint8_t *start;

      // copy the data to the buffer. assumes that the caller won't ask us to overrun

      start=reinterpret_cast<uint8_t *>(&FloppyImage)+(event.blockAddress*512);
      memcpy(event.buffer,start,static_cast<uint32_t>(event.blockCount)*512);
      event.success=true;
    }


    /**
     * Handle the onReady event: this device is always ready
     * @param event The event structure
     */

    void onIsReady(MscBotIsReadyEvent& event) {
      event.isReady=true;
    }


    /**
     * Handle the is-write-protected event: this device is always write protected because
     * the data is a virtual disk stored in flash.
     * @param event The event structure
     */

    void onIsWriteProtected(MscBotIsWriteProtectedEvent& event) {
      event.isWriteProtected=true;
    }


    /**
     * Get the capacity for this disk. The LUN index is in the event which will be zero
     * for us. We need to set the block size and count as well as the ready flag that says
     * we have a disk online.
     * @param event event data
     */

    void onGetCapacity(MscBotGetCapacityEvent& event) {

      // fill in this disks parameters

      event.blockSize=512;
      event.blockCount=reinterpret_cast<uint32_t >(&FloppyImageSize)/512;
      event.ready=true;
    }


    /**
     * Get the enquiry page for a LUN. Returns some basic stuff about the manufacturer, product
     * and version.
     * @see MscBotGetEnquiryPageEvent
     * @param event event data
     */

    void onGetEnquiryPage(MscBotGetEnquiryPageEvent& event) {

      // we only support a single LUN

      static const uint8_t data[0x24]={
        0,0x80,2,2,0x24-5,0,0,0,
       'A', 'n', 'd', 'y', 'W', 'k', 's', 'p',    // 8 byte manufacturer
       's', 't', 'm', '3', '2', 'p', 'l', 'u','s', ' ', 'U', 'S', 'B', ' ', ' ', ' ',   // 16 byte product
       '4', '.', '0','0',                         // 4 byte version
      };

      event.enquiryPage=data;
    }


    /**
     * USB error event received
     * @param uee the event descriptor
     */

    void onError(UsbErrorEvent& uee) {

      // ignore unconfigured errors from the HID device

      if(uee.provider==ErrorProvider::ERROR_PROVIDER_USB_DEVICE && uee.code==MyUsb::E_UNCONFIGURED)
        return;

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

  UsbDeviceMscInternalTest test;
  test.run();

  // not reached
  return 0;
}
