/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/usb/device/msc.h"
#include "config/sdcard.h"


using namespace stm32plus;
using namespace stm32plus::usb;


/**
 * This example demonstrates the somewhat over-exampled SD-Card-as-an-external-disk mass storage
 * device, also known as a USB thumb drive. This example is compatible with the F4 discovery
 * board if you connect an external SD card cage to the SDIO pins as follows:
 *
 * PC8  => D0
 * PC9  => D1
 * PC10 => D2
 * PC11 => D3
 * PC12 => CLK
 * PD2  => CMD
 *
 * The internal pullups are activated for D0..3 and CMD so you should not need to add anything
 * externally.
 *
 * This example assumes that an SDCard is inserted and ready when the device is plugged into
 * the host. The card should remain inserted while the device is plugged in. You can unplug
 * the device from the host, change cards, then re-insert the cable to the host. If there is
 * a problem during card initialisation then the red LED on the discovery board (PD5) will
 * light up.
 *
 * Compatible MCU:
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F407VGT6 / Windows 8.1 x64 host
 */

class UsbDeviceMscSdcardTest {

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
     * Declare the pointer to the SDCard implementation
     */

    scoped_ptr<SdioDmaSdCard> _sdcard;


    /*
     * PD5 is attached to a LED that we use to indicate problems
     */

    GpioPinRef _errorLed;


    /*
     * Run the example
     */

    void run() {

      /*
       * Set up the error LED that we might need later
       */

      GpioD<DefaultDigitalOutputFeature<5>> pd;
      _errorLed=pd[5];

      /*
       * set up the parameters for the USB MSC device. Do not attempt to reuse vid/pid combinations unless
       * you know how to flush your PC's USB driver cache because Windows caches the characteristics of each
       * device and will suspend your device if it suddenly re-appears as a different device type.
       */

      MyUsb::Parameters usbParams;

      usbParams.device_vid=0xDEAD;           // demo vendor ID
      usbParams.device_pid=0x0020;           // demo product ID

      usbParams.device_manufacturer_text="Andy's Workshop";   // see params.device_language_[ids/count] to change the languages
      usbParams.device_product_text="stm32plus flash drive";
      usbParams.device_configuration_text="My configuration";

      /*
       * The serial number is required and important for the MSC device class. See online
       * documentation for the length and format requirements.
       */

      usbParams.device_serial_text="002020141223";

      /*
       * Declare the USB object - this will initialise internal variables but will not
       * start the peripheral
       */

      MyUsb usb;

      /*
       * Subscribe to errors
       */

      usb.UsbErrorEventSender.insertSubscriber(UsbErrorEventSourceSlot::bind(this,&UsbDeviceMscSdcardTest::onError));

      /*
       * Subscribe to USB events
       */

      usb.UsbEventSender.insertSubscriber(UsbEventSourceSlot::bind(this,&UsbDeviceMscSdcardTest::onEvent));

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
     * we're only interested in data arriving from the host and control messages
     */

    void onEvent(UsbEventDescriptor& ued) {

      switch(ued.eventType) {

        /*
         * CLASS_INIT is called when the device is connecting to the host.
         */

        case UsbEventDescriptor::EventType::CLASS_INIT:
          onInit();
          break;

        /*
         * CLASS_DEINIT is called when the device is removed from the host. i.e. the
         * host is powered down or the cable has been removed.
         */

        case UsbEventDescriptor::EventType::CLASS_DEINIT:
          onDeInit();
          break;

        case UsbEventDescriptor::EventType::MSC_BOT_IS_READY:
          onIsReady(static_cast<MscBotIsReadyEvent&>(ued));
          break;

        case UsbEventDescriptor::EventType::MSC_BOT_GET_ENQUIRY_PAGE:
          onGetEnquiryPage(static_cast<MscBotGetEnquiryPageEvent&>(ued));
          break;

        case UsbEventDescriptor::EventType::MSC_BOT_GET_CAPACITY:
          onGetCapacity(static_cast<MscBotGetCapacityEvent&>(ued));
          break;

        case UsbEventDescriptor::EventType::MSC_BOT_READ:
          onRead(static_cast<MscBotReadEvent&>(ued));
          break;

        case UsbEventDescriptor::EventType::MSC_BOT_WRITE:
          onWrite(static_cast<MscBotWriteEvent&>(ued));
          break;

        case UsbEventDescriptor::EventType::MSC_BOT_IS_WRITE_PROTECTED:
          onIsWriteProtected(static_cast<MscBotIsWriteProtectedEvent&>(ued));
          break;

        default:   // warning suppression
          break;
      }
    }


    /**
     * The device is connecting to the host, create a new SDIO implementation. A card
     * must be inserted and ready and must remain for the duration of the session. This
     * example does not support hot swapping of cards while the device is connected
     * to the host.
     */

    void onInit() {

      // create the new instance, power up the card and initialise

      _sdcard.reset(new SdioDmaSdCard);

      // check for error and delete the instance if not working

      if(errorProvider.hasError()) {
        _sdcard.reset(nullptr);
        _errorLed.set();
      }
      else
        _errorLed.reset();
    }


    /**
     * The device is disconnecting from the host, release the SDIO implementation
     * and power down the card
     */

    void onDeInit() {
      _sdcard.reset(nullptr);
      _errorLed.reset();
    }


    /**
     * Read a number of blocks from the disk
     * @param event
     */

    void onRead(MscBotReadEvent& event) {

      if(_sdcard==nullptr)
        event.success=false;      // card failed to be detected on init
      else {

        // read single/multiblock depending on block count

        if(event.blockCount==1)
          event.success=_sdcard->readBlock(event.buffer,event.blockAddress);
        else
          event.success=_sdcard->readBlocks(event.buffer,event.blockAddress,event.blockCount);
      }
    }


    /**
     * Write a number of blocks to the disk
     * @param event
     */

    void onWrite(MscBotWriteEvent& event) {

      if(_sdcard==nullptr)
        event.success=false;      // card failed to be detected on init
      else {

        // write single/multiblock depending on block count

        if(event.blockCount==1)
          event.success=_sdcard->writeBlock(event.buffer,event.blockAddress);
        else
          event.success=_sdcard->writeBlocks(event.buffer,event.blockAddress,event.blockCount);
      }
    }


    /**
     * Handle the onReady event: the success of the init call determines readiness
     * @param event The event structure
     */

    void onIsReady(MscBotIsReadyEvent& event) {
      event.isReady=_sdcard!=nullptr;
    }


    /**
     * Handle the is-write-protected event: we'll limit this example to always being
     * write-enabled (the WP slider on an SD card is normally mapped to a GPIO but is
     * not a part of the SDIO standard)
     * @param event The event structure
     */

    void onIsWriteProtected(MscBotIsWriteProtectedEvent& event) {
      event.isWriteProtected=false;
    }


    /**
     * Get the capacity for this disk. The LUN index is in the event which will be zero
     * for us. We need to set the block size and count as well as the ready flag that says
     * we have a disk online.
     * @param event event data
     */

    void onGetCapacity(MscBotGetCapacityEvent& event) {

      if(_sdcard==nullptr)
        event.ready=false;            // the card was not initialised successfully
      else {

        // fill in this card's parameters

        event.blockSize=_sdcard->getBlockSizeInBytes();
        event.blockCount=_sdcard->getTotalBlocksOnDevice();
        event.ready=true;
      }
    }


    /**
     * Get the enquiry page for a LUN. Returns some basic stuff about the manufacturer, product
     * and version.
     * @param event event data
     */

    void onGetEnquiryPage(MscBotGetEnquiryPageEvent& event) {

      // we only support a single LUN

      static const uint8_t data[0x24]={
        0,0x80,2,2,0x24-5,0,0,0,
       'A', 'n', 'd', 'y', 'W', 'k', 's', 'p',    // 8 byte manufacturer
       's', 't', 'm', '3', '2', 'p', 'l', 'u','s', ' ', 'S', 'D', 'C', 'a', 'r', 'd',   // 16 byte product
       '4', '.', '0','0',                         // 4 byte version
      };

      event.enquiryPage=data;
    }


    /**
     * USB error event received
     * @param uee the event descriptor
     */

    void onError(UsbErrorEvent& /* uee */) {

      // flash the RED led on PD5 at 1Hz

      for(;;) {
        _errorLed.reset();
        MillisecondTimer::delay(500);
        _errorLed.set();
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

  UsbDeviceMscSdcardTest test;
  test.run();

  // not reached
  return 0;
}
