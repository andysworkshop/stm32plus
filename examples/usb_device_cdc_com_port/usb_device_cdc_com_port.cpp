/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/usb/device/cdc.h"


using namespace stm32plus;
using namespace stm32plus::usb;


/**
 * This example demonstrates the USB Communications Device Class (CDC) in the form of
 * a virtual COM port. Virtual COM ports allow you to use software on your PC that was designed
 * to talk to a serial port to communicate over USB to your STM32.
 *
 * This example is compatible with the STM32F4 Discovery board. Flash the example code to your
 * board and connect the micro-USB socket to your PC. Now open up your PC's Device Manager
 * and locate "Other Devices -> stm32plus virtual COM port". Right-click, "Update Driver Software..."
 * and use the browse option to go to the "usb_device_cdc_com_port" example directory. Windows
 * should find the .INF file and use it to install the driver for this example. When it's completed
 * you should have a new "Virtual COM Port" entry under the "Ports (COM & LPT)" category.
 *
 * To test, launch a terminal application (I use the free RealTerm program) and connect to the
 * virtual COM port. Set the protocol to 9600/8/N/1. Go ahead and type something. Each character
 * that you type will be echo'd back to the terminal with the message "You pressed: X" where X is
 * the key that you pressed.
 *
 * Compatible MCU:
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F407VGT6 / Windows 8.1 x64 host
 */

class UsbDeviceCdcComPortTest {

  public:

    /*
     * declare a type for the the USB stack
     */

    typedef ComPortCdcDevice<
      InternalFsPhy<>,                    // use the internal full speed PHY with no features
      ManufacturerTextFeature,            // we'll supply a manufacturer name
      ProductTextFeature,                 // ... and a product name
      SerialNumberTextFeature,            // ... and a serial number
      ConfigurationTextFeature            // ... and a config text string
    > MyUsb;


    /*
     * Flag to communicate between IRQ and non-IRQ code and the message to send back
     */

    enum class LoopState {
      WAITING,
      REPORT_KEYPRESS
    };

    volatile LoopState _loopState;
    char _message[16];


    /*
     * The current line coding (baud rate, parity etc)
     * The CdcLineCoding constructor sets a default of 9600/8/N/1
     */

    CdcLineCoding _lineCoding;


    /*
     * Run the example
     */

    void run() {

      LoopState state;

      // initialise

      _loopState=LoopState::WAITING;
      strcpy(_message,"You pressed: X\r\n");

      /*
       * set up the parameters for the USB CDC device. Do not attempt to reuse vid/pid combinations unless
       * you know how to flush your PC's USB driver cache because Windows caches the characteristics of each
       * device and will suspend your device if it suddenly re-appears as a different device type.
       */

      MyUsb::Parameters usbParams;

      usbParams.device_vid=0xDEAD;           // demo vendor ID
      usbParams.device_pid=0x0010;           // demo product ID

      usbParams.device_manufacturer_text="Andy's Workshop";   // see params.device_language_[ids/count] to change the languages
      usbParams.device_product_text="stm32plus virtual COM port";
      usbParams.device_serial_text="0123456789";
      usbParams.device_configuration_text="My configuration";

      usbParams.cdc_com_port_rx_buffer_size=16;  // default of 1Kb is far too big for this app

      /*
       * Declare the USB object - this will initialise internal variables but will not
       * start the peripheral
       */

      MyUsb usb;

      /*
       * Subscribe to errors
       */

      usb.UsbErrorEventSender.insertSubscriber(UsbErrorEventSourceSlot::bind(this,&UsbDeviceCdcComPortTest::onError));

      /*
       * Subscribe to USB events - data received from the host will be notified to us
       * asynchronously.
       */

      usb.UsbEventSender.insertSubscriber(UsbEventSourceSlot::bind(this,&UsbDeviceCdcComPortTest::onEvent));

      /*
       * Start the USB peripheral. It will run asynchronously. There is no requirement
       * for the parameters to remain in scope after the initialise call
       */

      if(!usb.initialise(usbParams))
        for(;;);      // onError() has already locked up

      // loop forever, or until an error interrupts us

      for(;;) {

        // block until a response is ready to send

        while((state=_loopState)==LoopState::WAITING);
        _loopState=LoopState::WAITING;

        if(state==LoopState::REPORT_KEYPRESS) {

          // transmit the reponse

          usb.transmit(_message,sizeof(_message));

          // we're ready to receive the next packet from the host

          usb.beginReceive();
        }
      }
    }


    /**
     * Event callback from the USB stack. Lots of stuff will come through here but
     * we're only interested in data arriving from the host and control messages
     */

    void onEvent(UsbEventDescriptor& ued) {

      // reject all events that we don't care about

      if(ued.eventType==UsbEventDescriptor::EventType::CDC_DATA_RECEIVED)
        onData(static_cast<CdcDataReceivedEvent&>(ued));
      else if(ued.eventType==UsbEventDescriptor::EventType::CDC_CONTROL)
        onControl(static_cast<CdcControlEvent&>(ued));
    }


    /**
     * Control event received from the host
     */

    void onControl(CdcControlEvent& event) {

      switch(event.opcode) {

        /*
         * We don't process these. They're enumerated here so you can
         * see what's available.
         */

        case CdcControlCommand::SEND_ENCAPSULATED_COMMAND:
        case CdcControlCommand::GET_ENCAPSULATED_RESPONSE:
        case CdcControlCommand::SET_COMM_FEATURE:
        case CdcControlCommand::GET_COMM_FEATURE:
        case CdcControlCommand::CLEAR_COMM_FEATURE:
        case CdcControlCommand::SET_CONTROL_LINE_STATE:
        case CdcControlCommand::SEND_BREAK:
          break;

        case CdcControlCommand::SET_LINE_CODING:      // set a new line encoding
          memcpy(&_lineCoding,event.data,sizeof(_lineCoding));
          break;

        case CdcControlCommand::GET_LINE_CODING:      // return the current line encoding
          event.data=reinterpret_cast<uint8_t *>(&_lineCoding);
          break;

        default:
          break;
      }
    }


    /**
     * Data received from the host
     */

    void onData(CdcDataReceivedEvent& event) {

      // add character to the message to send

      _message[13]=event.data[0];

      // signal to the main loop that a response is ready

      _loopState=LoopState::REPORT_KEYPRESS;
    }


    /**
     * USB error event received
     * @param uee the event descriptor
     */

    void onError(UsbErrorEvent& uee) {

      // ignore unconfigured errors from the CDC device

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

  UsbDeviceCdcComPortTest test;
  test.run();

  // not reached
  return 0;
}
