/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/display/tft.h"
#include "config/display/touch.h"


using namespace stm32plus;
using namespace stm32plus::display;


/**
 * ADS7843 touch screen test, implements a basic drawing
 * application using an ILI9325 LCD.
 *
 * Compatible MCU:
 *   STM32F1
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F103VET6
 */

class ADS7843Test : public ThreePointTouchScreenCalibrator::GuiCallback {

  protected:

    // declare a type for the LCD that we're using

    typedef Fsmc16BitAccessMode<FsmcBank1NorSram1> LcdAccessMode;   // my access mode
    typedef ILI9325_Landscape_64K<LcdAccessMode> LcdPanel;          // the panel

    // LCD-related objects

    LcdAccessMode *_accessMode;             // access mode for the LCD
    LcdPanel *_lcd;                         // 64K colour interface to the ILI9325
    DefaultBacklight *_backlight;           // backlight PWM output
    Font *_font;                            // font that we'll use

    // touch screen related objects

    ADS7843AsyncTouchScreen *_touchScreen;  // the touch screen object
    Spi *_spi;                              // The SPI peripheral
    ExtiPeripheralBase *_exti;              // An EXTI interrupt line for the pen IRQ
    GpioPinRef _penIrqPin;                  // The GPIO pin that receives the pin IRQ

    // calibrator and error-correction post-processors

    PassThroughTouchScreenCalibration *_passThroughCalibration;
    AveragingTouchScreenPostProcessor *_averagingPostProcessor;
    PassThroughTouchScreenPostProcessor *_passThroughPostProcessor;

    // the observer implementation will set this when the interrupt fires

    volatile bool _clicked;

    // these are the variables that the graphical demo will use

    bool _fgSelected;
    bool _accurate;
    TouchScreenCalibration* _calibrationResults;
    Size _boxSize;
    LcdPanel::tCOLOUR _fg,_bg,_colours[8];
    Rectangle _selectionBoxes[7];
    Point _lastPoint;
    uint16_t _backlightPercentage;

  public:

    /*
     * Demo setup and preparation
     */

    void run() {

      // set up the LCD
      initLcd();

      // set up the touch screen
      initTouchScreen();

      // set up variables for the demo
      _fgSelected=true;
      _accurate=false;
      _calibrationResults=NULL;
      _boxSize.Width=_boxSize.Height=0;
      _backlightPercentage=100;

      _colours[0]=ColourNames::RED;
      _colours[1]=ColourNames::GREEN;
      _colours[2]=ColourNames::BLUE;
      _colours[3]=ColourNames::BLACK;
      _colours[4]=ColourNames::WHITE;
      _colours[5]=ColourNames::CYAN;
      _colours[6]=ColourNames::MAGENTA;
      _colours[7]=ColourNames::YELLOW;

      // run the demo and don't come back
      doDemo();
    }


    /*
     * initialise the LCD panel
     */

    void initLcd() {

      // we've got RESET on PE1, backlight on PD13 and RS (D/CX) on PD11

      GpioE<DefaultDigitalOutputFeature<1>> pe;
      GpioD<DefaultDigitalOutputFeature<13>,DefaultFsmcAlternateFunctionFeature<11>> pd;

      // set up the FSMC timing for this panel
      Fsmc8080LcdTiming fsmcTiming(2,5);

      // set up the FSMC on bank 1 with A16 as the RS line (this is compatible with 100 pin devices)
      _accessMode=new LcdAccessMode(fsmcTiming,16,pe[1]);

      // create the LCD interface in landscape mode
      // this will power it up and do the reset sequence
      _lcd=new LcdPanel(*_accessMode);

      // set up some gamma values for this panel
      ILI9325Gamma gamma(0x0006,0x0101,0x0003,0x0106,0x0b02,0x0302,0x0707,0x0007,0x0600,0x020b);
      _lcd->applyGamma(gamma);

      // create a font and select it for stream operations
      _font=new Font_KYROU_9_REGULAR8();
      *_lcd << *_font;
      _lcd->setFontFilledBackground(false);

      // turn on the backlight at 100%

      _backlight=new DefaultBacklight;

      _backlightPercentage=100;
      _backlight->fadeTo(_backlightPercentage,4);
    }


    /*
     * initialise the touch screen
     */

    void initTouchScreen() {

      // create the initial pass through calibration object that allows us to create
      // the touch screen object ready for calibrating for real

      _passThroughCalibration=new PassThroughTouchScreenCalibration;

      // create an averaging post-processor for use in accurate mode that
      // does 4x oversampling on the incoming data

      _averagingPostProcessor=new AveragingTouchScreenPostProcessor(4);

      // create the do-nothing post-processor that is used in non-accurate mode

      _passThroughPostProcessor=new PassThroughTouchScreenPostProcessor;

      // we've got the PENIRQ attached to GPIOB, port 6. Attach an EXTI line to it and since
      // it's active low we want to be called back via our Observer implementation when the
      // signal falls from high to low.

      GpioB<DefaultDigitalInputFeature<6> > pb;
      _penIrqPin=pb[6];

      _exti=new Exti6(EXTI_Mode_Interrupt,EXTI_Trigger_Falling,pb[6]);

      // we've got the SPI interface to the touchscreen wired to SPI1, and since SPI1 is the fast one on the
      // STM32 we'll divide the 72Mhz clock by the maximum of 256 instead of 128 which we'd use on SPI2.

      Spi1<>::Parameters params;
      params.spi_baudRatePrescaler=SPI_BaudRatePrescaler_256;
      params.spi_cpol=SPI_CPOL_Low;
      params.spi_cpha=SPI_CPHA_1Edge;

      _spi=new Spi1<>(params);

      // now create the touch screen, initially in non-accurate mode with some dummy calibration data because the first thing
      // we're going to do in the demo is calibrate it with the 3-point routine.

      _touchScreen=new ADS7843AsyncTouchScreen(
          *_passThroughCalibration,
          *_passThroughPostProcessor,
          *_spi,
          _penIrqPin,
          *_exti
        );
    }


    /*
     * Calibrate the touch screen using the accurate 3-point method
     */

    void calibrate() {

      ThreePointTouchScreenCalibrator calibrator(*_touchScreen,*this);
      TouchScreenCalibration* newResults;

      // important preparation for calibration: we must set the screen to pass through mode
      // so that the calibrator sees raw co-ordinates and not calibrated!

      _touchScreen->setCalibration(*_passThroughCalibration);

      // calibrate the screen and get the new results. A real application can use the serialise
      // and deserialise methods of the TouchScreenCalibration base class to read/write the
      // calibration data to a persistent stream

      if(!calibrator.calibrate(newResults))
        return;

      // store the new results

      if(_calibrationResults!=NULL)
        delete _calibrationResults;

      _calibrationResults=newResults;

      // re-initialise the touch screen with the calibration data

      _touchScreen->setCalibration(*_calibrationResults);
    }


    /*
     * Get the size of one of the menu boxes on the screen
     */

    void calcBoxSize(const char **boxTexts,int numBoxes) {

      Size s;
      int i;

      for(i=0;i<numBoxes;i++) {
        s=_lcd->measureString(*_font,boxTexts[i]);

        if(s.Width>_boxSize.Width)
          _boxSize.Width=s.Width;

        if(s.Height>_boxSize.Height)
          _boxSize.Height=s.Height;
      }

      // add on 4px for the left selection bar, 2px all around for space and 1px all around for the border
      // ignoring that the border is shared between vertical boxes :)

      _boxSize.Width+=1+4+2+2+1;
      _boxSize.Height+=1+2+2+1;
    }


    /*
     * Draw the tools menu at the edge of the screen
     */

    void drawTools() {

      int16_t y;
      uint16_t i;
      Point p;

      const char *boxTexts[]= {
        "","fore","back","clear","recal","accurate", ""
      };

      if(_boxSize.Width==0)
        calcBoxSize(boxTexts,sizeof(boxTexts)/sizeof(boxTexts[0]));

      // clear down

      _lcd->setForeground(ColourNames::BLACK);
      _lcd->fillRectangle(Rectangle(0,0,_boxSize.Width,(_boxSize.Height+2)*sizeof(boxTexts)/sizeof(boxTexts[0])));

      _lcd->setForeground(ColourNames::WHITE);

      y=0;
      p.X=1+4+2;

      for(i=0;i<sizeof(boxTexts)/sizeof(boxTexts[0]);i++) {

        _selectionBoxes[i].X=0;
        _selectionBoxes[i].Y=y;
        _selectionBoxes[i].Width=_boxSize.Width;
        _selectionBoxes[i].Height=_boxSize.Height;

        _lcd->drawRectangle(_selectionBoxes[i]);

        p.Y=y+1+2;
        *_lcd << p << boxTexts[i];

        y+=_boxSize.Height+2;
      }

      drawSelection(_fg,1);
      drawSelection(_bg,2);

      if(_accurate)
        drawSelection(ColourNames::GREEN,5);

      drawColours();
      drawBacklight();

      // don't return until the pen is up

      while(!_penIrqPin.read());
    }


    /*
     * Draw a selection (green bar) indicator in the menu box
     */

    void drawSelection(LcdPanel::tCOLOUR barColour,int boxIndex) {
      _lcd->setForeground(barColour);
      _lcd->fillRectangle(Rectangle(1,(_boxSize.Height+2)*boxIndex+1,4,_boxSize.Height-2));
    }


    /*
     * Draw the backlight percentage box
     */

    void drawBacklight() {
      int width,y;

      y=(_boxSize.Height+2)*6+1;
      width=((_boxSize.Width-2)*_backlightPercentage)/100;

      _lcd->setForeground(ColourNames::BLUE);
      _lcd->fillRectangle(Rectangle(1,y,width,_boxSize.Height-2));

      if(_backlightPercentage<100) {
        _lcd->setForeground(ColourNames::BLACK);
        _lcd->fillRectangle(Rectangle(1+width,y,_boxSize.Width-2-(1+width),_boxSize.Height-2));
      }

      _lcd->setForeground(ColourNames::WHITE);
      *_lcd << Point(1+4+2,y+1+2) << "LED: " << _backlightPercentage;
      _lcd->setForeground(_fg);
    }


    /*
     * Draw the colour stripe boxes
     */

    void drawColours() {

      int i,numColours,width;
      Rectangle rc;

      numColours=sizeof(_colours)/sizeof(_colours[0]);
      width=(_boxSize.Width-2)/numColours;

      rc.X=1;
      rc.Y=1;
      rc.Height=_boxSize.Height-2;
      rc.Width=width;

      for(i=0;i<numColours;i++) {
        _lcd->setForeground(_colours[i]);
        _lcd->fillRectangle(rc);
        rc.X+=width;
      }
    }


    /*
     * Go into a loop running the demo
     */

    void doDemo() {

      Point p;
      int index;
      uint16_t newpercent;

      // clear down
      _lcd->setBackground(ColourNames::BLACK);
      _lcd->clearScreen();

      // calibrate the screen for first use
      calibrate();

      // clear down the screen

      _fg=ColourNames::WHITE;
      _bg=ColourNames::BLACK;

      _lcd->setBackground(_bg);
      _lcd->clearScreen();

      // draw the tools
      drawTools();

      // register as an observer for interrupts on the EXTI line

      _touchScreen->TouchScreenReadyEventSender.insertSubscriber(
          TouchScreenReadyEventSourceSlot::bind(this,&ADS7843Test::onTouchScreenReady)
        );

      for(;;) {

        // wait for a click

        _lastPoint.X=-1;
        for(_clicked=false;!_clicked;);

        do {

          // get click-coordinates from the panel

          if(_touchScreen->getCoordinates(p)) {

            // check if the click is in any of the menu boxes

            if(_selectionBoxes[0].containsPoint(p)) {
              index=(p.X-1)/8;
              if(index>=0 && index<=7) {
                if(_fgSelected) {
                  _fg=_colours[index];
                  _lcd->setForeground(_fg);
                }
                else {
                  _bg=_colours[index];
                  _lcd->setBackground(_bg);
                  _lcd->clearScreen();
                }
                drawTools();
              }
            }
            else if(_selectionBoxes[1].containsPoint(p)) {
              _fgSelected=true;
            }
            else if(_selectionBoxes[2].containsPoint(p)) {
              _fgSelected=false;
            }
            else if(_selectionBoxes[3].containsPoint(p)) {
              _lcd->clearScreen();
              drawTools();
            }
            else if(_selectionBoxes[4].containsPoint(p)) {
              while(!_penIrqPin.read());
              calibrate();
              _lcd->setBackground(_bg);
              _lcd->clearScreen();
              drawTools();
            }
            else if(_selectionBoxes[5].containsPoint(p)) {
              if(_accurate^=true)
                _touchScreen->setPostProcessor(*_averagingPostProcessor);
              else
                _touchScreen->setPostProcessor(*_passThroughPostProcessor);
              drawTools();
            }
            else if(_selectionBoxes[6].containsPoint(p)) {
              newpercent=(100*p.X-1)/(_boxSize.Width-2);
              if(newpercent!=_backlightPercentage && newpercent<=100) {
                _backlightPercentage=newpercent;
                _backlight->setDutyCycle(_backlightPercentage);
                drawBacklight();
              }
            }
            else {

              // if the click is on screen, plot it. This bounds check is necessary because
              // the touch screen can and does extend past the LCD edges.

              if(p.X>=0 && p.X<=_lcd->getXmax() && p.Y>=0 && p.Y<=_lcd->getYmax()) {

                  if(_lastPoint.X!=-1)
                    _lcd->drawLine(p,_lastPoint);
                else
                  _lcd->plotPoint(p);

                _lastPoint=p;
              }
            }
          }

        // carry on while the pen is still down

        } while(!_penIrqPin.read());
      }
    }


    /*
     * This will be called back when the EXTI interrupt fires.
     */

    void onTouchScreenReady() {
      _clicked=true;
    }


    /*
     * Display the prompt "Please tap the stylus on each red point"
     */

    virtual void displayPrompt(const char *text) {

      Size size;

      _lcd->setBackground(ColourNames::BLACK);
      _lcd->setForeground(ColourNames::WHITE);

      _lcd->clearScreen();

      // show the prompt at the top center

      size=_lcd->measureString(*_font,text);

      *_lcd << Point((_lcd->getWidth()/2)-(size.Width/2),0) << text;
    }


    /*
     * Display a hit point for the user to aim at
     */

    virtual void displayHitPoint(const Point& pt) {

      int16_t i,j,x,y;

      x=pt.X-1;
      y=pt.Y-1;

      _lcd->setForeground(ColourNames::RED);

      for(i=0;i<3;i++)
        for(j=0;j<3;j++)
          _lcd->plotPoint(Point(x+j,y+i));
    }


    /*
     * Get the size of the panel
     */

    virtual Size getPanelSize() {
      return Size(_lcd->getWidth(),_lcd->getHeight());
    }
};


/*
 * Main entry point
 */

int main() {

  // set up SysTick at 1ms resolution
  MillisecondTimer::initialise();

  ADS7843Test test;
  test.run();

  // not reached
  return 0;
}
