/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/usb.h"
#include "config/spi.h"
#include "config/timer.h"


using namespace stm32plus;
using namespace stm32plus::usb;


/*
 * This example demonstrates using the accelerometer in your STM32F4DISCOVERY board as a mouse
 * by implementing the USB Human interface device (HID) protocol. The MouseHidDevice class is
 * used to implement the USB mouse boot protocol that requires no PC driver installation.
 *
 * By tilting your discovery board in the X,Y directions you will, amusingly, be able to move
 * your computer mouse pointer around the screen. Yes, entire minutes of fun can be had trying
 * to get your mouse pointer to go in the direction you want it to. For extra credit we'll also
 * light up the four LEDs that surround the LIS302DL on the board at an intensity proportional
 * to how much you're tilting the board in the direction of each respective LED.
 *
 * To use this example, compile it and flash it to your STM32F4DISCOVERY board. Attach a USB
 * cable from the micro-USB socket on the discovery board to your PC. The USB device should be
 * recognised automatically. Have fun with your mouse!
 *
 * Compatible MCU:
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F407VGT6
 */

class UsbDeviceHidMouseTest {

  public:

    /*
     * declare a type for the the USB stack
     */

    typedef MouseHidDevice<
      InternalFsPhy<>,                    // use the internal full speed PHY with no features
      ManufacturerTextFeature,            // we'll supply a manufacturer name
      ProductTextFeature,                 // ... and a product name
      SerialNumberTextFeature,            // ... and a serial number
      ConfigurationTextFeature,           // ... and a config text string
      InterfaceTextFeature                // ... and an interface text string
    > MyUsb;

    /*
     * Declare the SPI pins. This is almost the same as the default SPI1 except that
     * ST moved NSS (chip-select) to PE3
     */

    enum {
      Port_NSS=GPIOE_BASE,
      Port_SCK=GPIOA_BASE,
      Port_MISO=GPIOA_BASE,
      Port_MOSI=GPIOA_BASE,

      Pin_NSS  = GPIO_Pin_3,
      Pin_SCK  = GPIO_Pin_5,
      Pin_MISO = GPIO_Pin_6,
      Pin_MOSI = GPIO_Pin_7
    };

    /*
     * Declare a SPI type
     */

    typedef Spi1_Custom<UsbDeviceHidMouseTest> MySpi;

    /*
     * Run the example
     */

    void run() {

      /*
       * set up the parameters for the USB hid device
       */

      MyUsb::Parameters usbParams;

      usbParams.device_vid=0xDEAD;           // demo vendor ID
      usbParams.device_pid=0xBEEF;           // demo product ID
      usbParams.device_manufacturer_text="Andy's Workshop";   // see params.device_language_[ids/count] to change the languages
      usbParams.device_product_text="stm32plus wacky mouse";
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
        error();

      /*
       * The USB device is now up and running. Now we initialise the LIS302DL SPI1 interface
       */

      MySpi::Parameters spiParams;

      /*
       * override some of the default SPI parameters
       */

      spiParams.spi_baudRatePrescaler=SPI_BaudRatePrescaler_4;
      spiParams.spi_cpol=SPI_CPOL_Low;
      spiParams.spi_cpha=SPI_CPHA_1Edge;

      MySpi spi(spiParams);

      /*
       * initialise the accelerometer
       */

      initialiseAccelerometer(spi);

      /*
       * Initialise the 4 LEDs on the board, which are conveniently attached to TIM4
       * channels 1,2,3,4
       */

      Timer4<
        Timer4InternalClockFeature,       // the timer clock source is APB1 (APB on the F0)
        TimerChannel1Feature<>,           // we're going to use channel 1...
        TimerChannel2Feature<>,           // ...and we're going to use channel 2
        TimerChannel3Feature<>,           // ...and we're going to use channel 3
        TimerChannel4Feature<>,           // ...and we're going to use channel 4
        Timer4GpioFeature<                // we want to output something to GPIO
          TIMER_REMAP_FULL,               // the GPIO output will be remapped to PD12..15
          TIM4_CH1_OUT,                   // we will output channel 1 to GPIO...
          TIM4_CH2_OUT,                   // ...and we will output channel 2 to GPIO
          TIM4_CH3_OUT,                   // ...and we will output channel 2 to GPIO
          TIM4_CH4_OUT                    // ...and we will output channel 2 to GPIO
        >
      > timer;

      /**
       * Set the timer clock to 4.2MHz with a reload counter of 2000
       */
      timer.setTimeBaseByFrequency(4200000,2000);

      timer.TimerChannel1Feature<>::initCompareForPwmOutput();
      timer.TimerChannel2Feature<>::initCompareForPwmOutput();
      timer.TimerChannel3Feature<>::initCompareForPwmOutput();
      timer.TimerChannel4Feature<>::initCompareForPwmOutput();

      timer.enablePeripheral();

      /*
       * read the starting position to use as an offset
       */

      int8_t xoffset,yoffset,x,y;

      readXY(spi,xoffset,yoffset);

      /*
       * Go into a loop reading the X and Y position every 10ms (the USB poll frequency)
       */

      for(;;) {

        MillisecondTimer::delay(10);

        readXY(spi,x,y);

        x-=xoffset;
        y-=yoffset;

        // only consider movements >2 pixels

        if(Abs<int8_t>(x)<=2)
          x=0;

        if(Abs<int8_t>(y)<=2)
          y=0;

        // cap x,y at (-127 .. +127)

        x=Min<int8_t>(127,Max<int8_t>(-127,x));
        y=Min<int8_t>(127,Max<int8_t>(-127,y));

        // send the report to the USB host (your PC)

        usb.mouseSendReport(0,x,y);

        // give visual feedback via the LEDs, brighter means faster

        if(x>0) {
          timer.TimerChannel3Feature<>::setDutyCycle(x*2);
          timer.TimerChannel1Feature<>::setDutyCycle(0);
        }
        else if(x<0) {
          timer.TimerChannel3Feature<>::setDutyCycle(0);
          timer.TimerChannel1Feature<>::setDutyCycle(-x*2);
        }
        else {
          timer.TimerChannel3Feature<>::setDutyCycle(0);
          timer.TimerChannel1Feature<>::setDutyCycle(0);
        }

        if(y>0) {
          timer.TimerChannel4Feature<>::setDutyCycle(y*2);
          timer.TimerChannel2Feature<>::setDutyCycle(0);
        }
        else if(y<0) {
          timer.TimerChannel4Feature<>::setDutyCycle(0);
          timer.TimerChannel2Feature<>::setDutyCycle(-y*2);
        }
        else {
          timer.TimerChannel4Feature<>::setDutyCycle(0);
          timer.TimerChannel2Feature<>::setDutyCycle(0);
        }
      }
    }


    /*
     * Read the X and Y position
     */

    void readXY(MySpi& spi,int8_t& x,int8_t& y) {

      const uint8_t cmd=0x29 |    // LIS302DL_OUT_X_ADDR
                        0x80 |    // READWRITE_CMD
                        0x40;     // MULTIPLEBYTE_CMD

      uint8_t output[4];

      // read the data. OUT_X is 29 and OUT_Y is 2B. we'll grab both in one read

      spi.setNss(false);
      spi.send(&cmd,1);
      spi.receive(output,4);
      spi.setNss(true);

      // re-orient the chip so that the "ST" logo is upright

      y=output[0];
      x=output[2];
    }


    /*
     * Initialise the accelerometer by writing to the control register
     */

    void initialiseAccelerometer(MySpi& spi) {

      uint8_t buffer[2];

      buffer[0]=0x20;     // LIS302DL_CTRL_REG1_ADDR
      buffer[1]=0x40 |    // LIS302DL_LOWPOWERMODE_ACTIVE
                0x01 |    // LIS302DL_X_ENABLE
                0x02;     // LIS302DL_Y_ENABLE

      // send the data

      spi.setNss(false);
      while(!spi.readyToSend());
      spi.send(buffer,2);
      spi.setNss(true);

      // wait for ready

      MillisecondTimer::delay(30);
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

  UsbDeviceHidMouseTest test;
  test.run();

  // not reached
  return 0;
}
