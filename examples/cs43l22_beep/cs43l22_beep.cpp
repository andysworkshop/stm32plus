/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/dac.h"


using namespace stm32plus;


/**
 * This example will sound the CS43L22 internal beep tone
 * twice per second. From experimentation I have
 * determined that the CS43L22 needs an incoming data
 * stream in order to generate the beep - just supplying
 * MCLK is not enough.
 *
 * Therefore for this demo I stream in a continuous
 * stream of nothing (zeros) while sounding the beep.
 *
 * Compatible MCU:
 *   STM32F1
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F407VGT6
 */

class CS43L22BeepTest {

  public:

    /*
     * The F4 MCU packages match the pins on the F1 for equivalent peripherals but the F4 is not
     * limited to distinct sets of pins that make up a peripheral. Annoyingly the CS43L22 on the
     * F4Discovery board has I2C1 SCL and SDA split across what would be the default and remap
     * pin set on the F1 so we have to use 'custom' port/pin declarations instead of using one
     * of the I2C1_Default or I2C1_Remap pre-defined packages.
     */

    enum {
      Port_SCL=GPIOB_BASE,//!< SCL_PORT
      Port_SDA=GPIOB_BASE,//!< SDA_PORT

      Pin_SCL=GPIO_Pin_6, //!< SCL_PIN
      Pin_SDA=GPIO_Pin_9  //!< SDA_PIN
    };

    /*
     * Same goes for the I2S pins. They're using I2S3 on the F4Discovery board but the pin
     * choice for I2S3 does not match the F1.
     */

    enum {
      Port_WS   = GPIOA_BASE,
      Port_CK   = GPIOC_BASE,
      Port_SD   = GPIOC_BASE,
      Port_MCLK = GPIOC_BASE,

      Pin_WS   = GPIO_Pin_4,
      Pin_CK   = GPIO_Pin_10,
      Pin_SD   = GPIO_Pin_12,
      Pin_MCLK = GPIO_Pin_7
    };

  protected:

      /*
       * The CS43L22 has a control and a data interface. Here we define the type that will be used
       * for the control interface. It's going to be I2C.
       */

      typedef CS43L22ControlI2C<              // The I2C controller. It's templated with the I2C interface and features.
        I2C1_Custom<                          // F4 VLDiscovery pinning does not match one of the standard pinouts
          CS43L22BeepTest,                    // get the pinning from this class
          I2CSingleByteMasterPollingFeature   // we're going to be polling in master mode
        >
      > MyDacControlInterface;

      /*
       * The data interface for this example will be I2S. Here we define a type for it.
       */

      typedef I2S3_Custom<          // F4 VLDiscovery pinning does not match one of the standard pinouts
          CS43L22BeepTest,          // get the pinning from this class
          I2S3InterruptFeature      // we'll stream the data in the interrupt handler
        > MyDacDataInterface;

      /*
       * Now define the CS43L22 type with the control and data interface
       */

      typedef CS43L22<              // the device is parameterised with the I2C peripheral
        MyDacControlInterface,
        MyDacDataInterface
      > MyDac;

      /*
       * Declare the peripheral pointer
       */

      MyDac *_dac;

  public:

    void run() {

      /*
       * Declare the reset pin which is on PD4 on the F4 discovery board.
       */

      GpioD<DefaultDigitalOutputFeature<4> > pd;

      /*
       * Declare an instance of the DAC with default I2C parameters of 100kHz, ACK-on, 50% duty, 7-bit
       * and default I2S parameters of 44kHz, master, phillips, 16-bit, mclk-on, cpol-low
       * Leave the master polling feature bus timeout at 5 seconds.
       */

      MyDac::Parameters params;
      params.resetPin=pd[4];

      _dac=new MyDac(params);

      // set ourselves up to observe the interrupts

      _dac->I2S3InterruptFeature::SpiInterruptEventSender.insertSubscriber(
          SpiInterruptEventSourceSlot::bind(this,&CS43L22BeepTest::onNotify)
        );

      // reset the device

      _dac->reset();

      // send the I2C initialisation sequence

      if(!_dac->initialise())
        error(3);

      // headphones on - default volume level of 200 (out of 255)

      _dac->headphonesOn();

      // enable the interrupts - this will start the data transfer

      _dac->I2S3InterruptFeature::enableInterrupts(SPI_I2S_IT_TXE);

      // finished - interrupts are now supplying the null data stream

      for(;;) {

        // sound the beep and then wait 500ms

        _dac->beepSingle();
        MillisecondTimer::delay(500);
      }
    }


    /*
     * callback function. This is called when the TXE interrupt that we've enabled is fired.
     */

    void onNotify(SpiEventType set) {

      static const uint16_t NULL_DATA=0;

      // send the null data

      if(set==SpiEventType::EVENT_READY_TO_TRANSMIT)
        _dac->send(&NULL_DATA,1);
    }


    /*
     * Handle an error by flashing the LED on PD13 repeatedly
     */

    void error(uint8_t code) {

      GpioD<DefaultDigitalOutputFeature<13> > pd;

      for(;;) {

        for(uint8_t i=0;i<code;i++) {
          pd[13].set();
          MillisecondTimer::delay(250);
          pd[13].reset();
          MillisecondTimer::delay(250);
        }

        MillisecondTimer::delay(3000);
      }
    }
};


/*
 * Main entry point
 */

int main() {

  // timing is required

  MillisecondTimer::initialise();

  CS43L22BeepTest test;
  test.run();

  // not reached
  return 0;
}
