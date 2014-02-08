/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#include "config/stm32plus.h"
#include "config/display/touch.h"


namespace stm32plus {

  namespace display {

    /**
     * Constructor
     * @param[in] calibration The class used to translate raw readings to display points.
     * @param[in] postProcessor The postprocessor for the points received from the screen.
     * @param[in] spi Reference to the SPI instance.
     * @param[in] exti Reference to the EXTI instance - must not go out of scope.
     * @param[in] irqPin Reference to the pin used to raise the IRQ on the EXTI line.
     */

    ADS7843AsyncTouchScreen::ADS7843AsyncTouchScreen(
        TouchScreenCalibration& calibration,
        TouchScreenPostProcessor& postProcessor,
        Spi& spi,
        const GpioPinRef& irqPin,
        ExtiPeripheralBase& exti) : TouchScreen(calibration,postProcessor),
                                    _spi(spi),
                                    _irqPin(irqPin),
                                    _exti(exti) {

      uint16_t xvalues[7],yvalues[7];

      // register ourselves with the event source

      exti.ExtiInterruptEventSender.insertSubscriber(ExtiInterruptEventSourceSlot::bind(this,&ADS7843AsyncTouchScreen::onNotify));

      // make sure the device has penirq enabled

      getSamples(xvalues,yvalues,7);
    }


    /**
     * Get some samples from the panel
     * @param[in] channelBits The channel bits (A0,A1,A2) to sample
     * @param[out] buffer Where to place the samples
     * @param[in] The number of samples to take
     */

    void ADS7843AsyncTouchScreen::getSamples(uint16_t *xvalues,uint16_t *yvalues,uint16_t sampleCount) const {

      // mask all interrupts while we're doing the sampling

      __disable_irq();

      // set CS = low

      _spi.setNss(false);

      // get the X values and don't power down afterwards

      getSamples(ChannelX | START | PD0 | PD1,ChannelX | START | PD0 | PD1,xvalues,sampleCount);

      // get the Y samples and power down afterwards

      getSamples(ChannelY | START | PD0 | PD1,ChannelY | START,yvalues,sampleCount);

      // set CS = high

      _spi.setNss(true);

      // if PENIRQ went low during the sampling then the EXTI interrupt will be pending so
      // we clear the pending bit now before re-enabling interrupts

      _exti.clearPendingInterrupt();
      __enable_irq();
    }


    /**
     * Get a set of samples
     */

    void ADS7843AsyncTouchScreen::getSamples(uint8_t firstCommand,uint8_t lastCommand,uint16_t *values,uint16_t sampleCount) const {

      uint8_t dummy,data[2] = { 0 };

      dummy=0;

      _spi.send(&firstCommand,1);
      _spi.send(&dummy,1);
      _spi.send(&dummy,1);

      while(sampleCount--!=1) {

        _spi.send(&firstCommand,1);

        _spi.send(&dummy,1,&data[0]);
        _spi.send(&dummy,1,&data[1]);

        // add the 12 bit response. The MSB of the first response is discarded
        // because it's a NUL bit output during the BUSY line period

        *values++=static_cast<uint16_t>(data[0] & 0x7f) << 5 | (data[1] >> 3);
      }

      // the last sample

      _spi.send(&lastCommand,1);

      _spi.send(&dummy,1,&data[0]);
      _spi.send(&dummy,1,&data[1]);

      *values=static_cast<uint16_t>(data[0] & 0x7f) << 5 | (data[1] >> 3);
    }


    /**
     * Get the co-ordinates of the touch. Sampling the panel is essentially an ADC operation with all the problems of
     * outliers and noise that comes with it. To minimise the likelihood of bad data which would result in bizarre
     * 'phantom touches' to the user, we will take 5 samples and use the median.
     *
     * If the user lifts off the screen during the sampling then we abort.
     *
     * @param[in] point The converted co-ordinate.
     */

    bool ADS7843AsyncTouchScreen::getCoordinates(Point& point) {

      uint16_t xvalues[7],yvalues[7];
      Point p;
      TouchScreenPostProcessor::PostProcessAction ppa;
      int16_t sequenceNumber;

      sequenceNumber=0;

      do {

        // get the samples

        getSamples(xvalues,yvalues,7);

        // the panel must have been touched all this time

        if(!isTouched())
          return errorProvider.set(ErrorProvider::ERROR_PROVIDER_TOUCH_SCREEN,E_NO_TOUCH,0);

        // get the median values

        p.X=fastMedian(xvalues);
        p.Y=fastMedian(yvalues);

        // translate the co-ordinates using the calibration routine

        point=_calibration->translate(p);

        // pass the co-ordinate to the post processor and abort if it fails

        if((ppa=_postProcessor->postProcess(point,sequenceNumber++))==TouchScreenPostProcessor::Error)
          return false;

        // keep going until the post processor signals it's finished

      } while(ppa==TouchScreenPostProcessor::MoreSamplesRequired);

      return true;
    }


    /**
     * Do a fast 7-sample median selection on the buffer of 7 samples.
     * @param[in] samples Pointer to 7 samples
     * @return The median of the 7 samples
     */

    uint16_t ADS7843AsyncTouchScreen::fastMedian(uint16_t *samples) const {

      // do a fast median selection (reference code available on internet). This code basically
      // avoids sorting the entire samples array

      #define PIX_SORT(a,b) { if ((a)>(b)) PIX_SWAP((a),(b)); }
      #define PIX_SWAP(a,b) { uint16_t temp=(a);(a)=(b);(b)=temp; }

      PIX_SORT(samples[0], samples[5]) ; PIX_SORT(samples[0], samples[3]) ; PIX_SORT(samples[1], samples[6]) ;
      PIX_SORT(samples[2], samples[4]) ; PIX_SORT(samples[0], samples[1]) ; PIX_SORT(samples[3], samples[5]) ;
      PIX_SORT(samples[2], samples[6]) ; PIX_SORT(samples[2], samples[3]) ; PIX_SORT(samples[3], samples[6]) ;
      PIX_SORT(samples[4], samples[5]) ; PIX_SORT(samples[1], samples[4]) ; PIX_SORT(samples[1], samples[3]) ;
      PIX_SORT(samples[3], samples[4]) ;

      return samples[3];
    }


    /*
     * Event notification
     */

    void ADS7843AsyncTouchScreen::onNotify(uint8_t /* extiNumber */) {

      // call the subscribers

      TouchScreenReadyEventSender.raiseEvent();
    }
  }
}
