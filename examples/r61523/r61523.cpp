/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/display/tft.h"


extern uint32_t BulbPixelsSize,BulbPixels;
extern uint32_t AudioPixelsSize,AudioPixels;
extern uint32_t FlagPixelsSize,FlagPixels;
extern uint32_t DocPixelsSize,DocPixels;
extern uint32_t GlobePixelsSize,GlobePixels;
extern uint32_t JpegTest0Pixels,JpegTest0PixelsSize;


using namespace stm32plus;
using namespace stm32plus::display;


/**
 * R61523 LCD test, show a looping graphics demo. We will make
 * use of the built-in PWM generator to control the backlight.
 * This saves us an MCU output pin and a timer peripheral.
 *
 * It's a 16-bit device and we control it in this demo
 * using the FSMC peripheral on bank 1. The wiring
 * that you need to do is as follows:
 *
 * PE1  => RESET
 * PD11 => RS (D/CX)
 * PD7  => CS
 * PD4  => RD
 * PD5  => WR
 * PD14 => D0    PE11 => D8
 * PD15 => D1    PE12 => D9
 * PD0  => D2    PE13 => D10
 * PD1  => D3    PE14 => D11
 * PE7  => D4    PE15 => D12
 * PE8  => D5    PD8  => D13
 * PE9  => D6    PD9  => D14
 * PE10 => D7    PD10 => D15
 *
 * And for the backlight, connect together the following
 * two pins on the LCD breakout board:
 *
 * BL_PWM => EN
 *
 * Compatible MCU:
 *   STM32F1
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F103VET6
 *   STM32F407VGT6
 */

class R61523Test {

  protected:
    typedef Fsmc16BitAccessMode<FsmcBank1NorSram1> LcdAccessMode;
    typedef R61523_Landscape_16M<LcdAccessMode> LcdPanel;
    typedef R61523PwmBacklight<LcdAccessMode> LcdBacklight;

    LcdAccessMode *_accessMode;
    LcdPanel *_gl;
    Font *_font;

  public:
    void run() {

      // reset is on PE1 and RS (D/CX) is on PD11

      GpioE<DefaultDigitalOutputFeature<1> > pe;
      GpioD<DefaultFsmcAlternateFunctionFeature<11>> pd;

      // Set up the FSMC timing. These numbers (particularly the data setup time) are dependent on
      // both the FSMC bus speed and the panel timing parameters. If you see corrupted graphics then
      // you may need to adjust these to uit the characteristics of your particular panel.

#if defined(STM32PLUS_F4)
      Fsmc8080LcdTiming fsmcReadTiming(0,20);
      Fsmc8080LcdTiming fsmcWriteTiming(0,4);
#elif defined(STM32PLUS_F1)
      Fsmc8080LcdTiming fsmcReadTiming(0,20);
      Fsmc8080LcdTiming fsmcWriteTiming(0,4);
#else
#error Invalid MCU
#endif

      // set up the FSMC with RS=A16 (PD11)

      _accessMode=new LcdAccessMode(fsmcReadTiming,fsmcWriteTiming,16,pe[1]);

      // declare a panel

      _gl=new LcdPanel(*_accessMode);

      // apply the gamma curve. Note that gammas are panel specific. This curve is appropriate
      // to a replacement (non-original) panel obtained from ebay.

      uint8_t levels[13]={ 0xe,0,1,1,0,0,0,0,0,0,3,4,0 };
      R61523Gamma gamma(levels);
      _gl->applyGamma(gamma);

      // clear to black while the lights are out

      _gl->setBackground(0);
      _gl->clearScreen();

      // create the backlight using default template parameters

      LcdBacklight backlight(*_accessMode);

      // fade up the backlight to 100% using the hardware to do the smooth fade

      backlight.setPercentage(100);

      // Create a font. A wide range of sample fonts are available. See the
      // "lib/include/display/graphic/fonts" directory for a full list and
      // you can always download and convert your own using the FontConv utility.

      _font=new Font_PROGGYCLEAN16;
      *_gl << *_font;

      for(;;) {
        lzgTest();
        jpegTest();
        basicColoursTest();
        gradientTest();
        textTest();
        rectTest();
        lineTest();
        ellipseTest();
        clearTest();
        sleepTest();
      }
    }

    void sleepTest() {

      prompt("Sleep test");

      // go to sleep

      *_gl << Point::Origin << "Sleeping now...";
      MillisecondTimer::delay(1000);
      _gl->sleep();
      MillisecondTimer::delay(3000);

      // wake up

      _gl->wake();
      _gl->clearScreen();
      *_gl << Point::Origin << "Woken up again...";
      MillisecondTimer::delay(3000);
    }

    void jpegTest() {

      // only draw if it can fit on screen

      if(_gl->getHeight()==360 && _gl->getWidth()==640) {

        prompt("JPEG bitmap test");

        // draw it centered

        LinearBufferInputOutputStream compressedData((uint8_t *)&JpegTest0Pixels,(uint32_t)&JpegTest0PixelsSize);
        _gl->drawJpeg(Rectangle(0,0,640,360),compressedData);

        MillisecondTimer::delay(3000);
      }
    }

    void lzgTest() {

      prompt("LZG bitmap test");

      // declare a DMA instance with a copy-to-fsmc feature

#if defined(STM32PLUS_F1)
      Dma1Channel6<DmaFsmcLcdMemoryCopyFeature<LcdAccessMode> > dma;
#elif defined(STM32PLUS_F4)
      Dma2Channel1Stream2<DmaFsmcLcdMemoryCopyFeature<LcdAccessMode> > dma;
#endif

      drawCompressedBitmap((uint8_t *)&BulbPixels,(uint32_t)&BulbPixelsSize,89,148,true,dma);
      drawCompressedBitmap((uint8_t *)&AudioPixels,(uint32_t)&AudioPixelsSize,150,161,false,dma);
      drawCompressedBitmap((uint8_t *)&FlagPixels,(uint32_t)&FlagPixelsSize,144,220,true,dma);
      drawCompressedBitmap((uint8_t *)&DocPixels,(uint32_t)&DocPixelsSize,200,240,false,dma);
      drawCompressedBitmap((uint8_t *)&GlobePixels,(uint32_t)&GlobePixelsSize,193,219,true,dma);
    }


    void drawCompressedBitmap(uint8_t *pixels,uint32_t size,uint16_t width,uint16_t height,bool useDma,DmaFsmcLcdMemoryCopyFeature<LcdAccessMode>& dma) {

      _gl->setBackground(ColourNames::WHITE);
      _gl->clearScreen();

      LinearBufferInputOutputStream compressedData(pixels,size);
      LzgDecompressionStream decompressor(compressedData,size);

      if(useDma) {
        _gl->drawBitmap(
            Rectangle((_gl->getWidth()-width)/2,
                (_gl->getHeight()-height)/2,
                width,height),
                decompressor,
                dma);
      }
      else {
        _gl->drawBitmap(
            Rectangle((_gl->getWidth()-width)/2,
                (_gl->getHeight()-height)/2,
                width,height),
                decompressor);
      }

      MillisecondTimer::delay(3000);
    }


    void textTest() {

      const char *str="The quick brown fox";
      Size size;
      Point p;
      uint32_t i,start;

      prompt("Stream operators test");

      *_gl << Point::Origin << "Let's see PI:";

      for(i=0;i<=7;i++)
        *_gl << Point(0,(1+i)*_font->getHeight()) << DoublePrecision(3.1415926535,i);

      MillisecondTimer::delay(5000);

      prompt("Opaque text test");

      size=_gl->measureString(*_font,str);

      for(i=0,start=MillisecondTimer::millis();MillisecondTimer::millis()-start<5000;i++) {

        p.X=rand() % (_gl->getXmax()-size.Width);
        p.Y=rand() % (_gl->getYmax()-size.Height);

        _gl->setForeground(rand());
        _gl->writeString(p,*_font,str);
      }
    }


    void clearTest() {

      int i;
      uint32_t start;

      prompt("Clear screen test");

      for(i=0;i<200;i++) {
        _gl->setBackground(rand());

        start=MillisecondTimer::millis();
        _gl->clearScreen();
        stopTimer(" to clear",MillisecondTimer::millis()-start);
      }
    }


    void basicColoursTest() {

      uint16_t i;

      static const uint32_t colours[8]={
        ColourNames::RED,
        ColourNames::GREEN,
        ColourNames::BLUE,
        ColourNames::CYAN,
        ColourNames::MAGENTA,
        ColourNames::YELLOW,
        ColourNames::BLACK,
        ColourNames::WHITE,
      };

      prompt("Basic colours test");

      for(i=0;i<sizeof(colours)/sizeof(colours[0]);i++) {
        _gl->setBackground(colours[i]);
        _gl->clearScreen();

        MillisecondTimer::delay(500);
      }
    }


    void lineTest() {

      Point p1,p2;
      uint32_t i,start;

      prompt("Line test");

      for(i=0,start=MillisecondTimer::millis();MillisecondTimer::millis()-start<5000;i++) {

        p1.X=rand() % _gl->getXmax();
        p1.Y=rand() % _gl->getYmax();
        p2.X=rand() % _gl->getXmax();
        p2.Y=rand() % _gl->getYmax();

        _gl->setForeground(rand());
        _gl->drawLine(p1,p2);
      }

      _gl->setForeground(ColourNames::WHITE);
      _gl->clearScreen();
      *_gl << Point::Origin << i << " lines in 5 seconds";
      MillisecondTimer::delay(3000);
    }

    void rectTest() {

      uint32_t i,start;
      Rectangle rc;

      prompt("Rectangle test");

      for(i=0,start=MillisecondTimer::millis();MillisecondTimer::millis()-start<5000;i++) {

        rc.X=(rand() % _gl->getXmax()/2);
        rc.Y=(rand() % _gl->getYmax()/2);
        rc.Width=rand() % (_gl->getXmax()-rc.X);
        rc.Height=rand() % (_gl->getYmax()-rc.Y);

        _gl->setForeground(rand());
        _gl->fillRectangle(rc);
      }

      _gl->clearScreen();

      for(i=0,start=MillisecondTimer::millis();MillisecondTimer::millis()-start<5000;i++) {

        rc.X=(rand() % _gl->getXmax()/2);
        rc.Y=(rand() % _gl->getYmax()/2);
        rc.Width=rand() % (_gl->getXmax()-rc.X);
        rc.Height=rand() % (_gl->getYmax()-rc.Y);

        _gl->setForeground(rand());
        _gl->drawRectangle(rc);

        if(i % 1000 ==0)
          _gl->clearScreen();
      }
    }


    void ellipseTest() {

      uint32_t i,start;
      Point p;
      Size s;

      prompt("Ellipse test");
      _gl->setBackground(0);

      for(i=0,start=MillisecondTimer::millis();MillisecondTimer::millis()-start<5000;i++) {

        p.X=_gl->getXmax()/4+(rand() % (_gl->getXmax()/2));
        p.Y=_gl->getYmax()/4+(rand() % (_gl->getYmax()/2));

        if(p.X<_gl->getXmax()/2)
          s.Width=rand() % p.X;
        else
          s.Width=rand() % (_gl->getXmax()-p.X);

        if(p.Y<_gl->getYmax()/2)
          s.Height=rand() % p.Y;
        else
          s.Height=rand() % (_gl->getYmax()-p.Y);

        _gl->setForeground(rand());
        _gl->fillEllipse(p,s);
      }

      _gl->clearScreen();

      for(i=0,start=MillisecondTimer::millis();MillisecondTimer::millis()-start<5000;i++) {

        p.X=_gl->getXmax()/4+(rand() % (_gl->getXmax()/2));
        p.Y=_gl->getYmax()/4+(rand() % (_gl->getYmax()/2));

        if(p.X<_gl->getXmax()/2)
          s.Width=rand() % p.X;
        else
          s.Width=rand() % (_gl->getXmax()-p.X);

        if(p.Y<_gl->getYmax()/2)
          s.Height=rand() % p.Y;
        else
          s.Height=rand() % (_gl->getYmax()-p.Y);

        if(s.Height>0 && s.Width>0 && p.X+s.Width<_gl->getXmax() && p.Y+s.Height<_gl->getYmax()) {
          _gl->setForeground(rand());
          _gl->drawEllipse(p,s);
        }

        if(i % 500==0)
          _gl->clearScreen();
      }
    }

    void doGradientFills(Direction dir) {

      Rectangle rc;
      uint16_t i;
      static uint32_t colours[7]={
        ColourNames::RED,
        ColourNames::GREEN,
        ColourNames::BLUE,
        ColourNames::CYAN,
        ColourNames::MAGENTA,
        ColourNames::YELLOW,
        ColourNames::WHITE,
      };


      rc.Width=_gl->getXmax()+1;
      rc.Height=(_gl->getYmax()+1)/2;

      for(i=0;i<sizeof(colours)/sizeof(colours[0]);i++) {

        rc.X=0;
        rc.Y=0;

        _gl->gradientFillRectangle(rc,dir,ColourNames::BLACK,colours[i]);
        rc.Y=rc.Height;
        _gl->gradientFillRectangle(rc,dir,colours[i],ColourNames::BLACK);

        MillisecondTimer::delay(1000);
      }
    }

    void gradientTest() {

      prompt("Gradient test");

      doGradientFills(HORIZONTAL);
      doGradientFills(VERTICAL);
    }

    void prompt(const char *prompt) {

      _gl->setBackground(ColourNames::BLACK);
      _gl->clearScreen();

      _gl->setForeground(ColourNames::WHITE);
      *_gl << Point(0,0) << prompt;

      MillisecondTimer::delay(2000);
      _gl->clearScreen();
    }


    void stopTimer(const char *prompt,uint32_t elapsed) {

      _gl->setForeground(0xffffff);
      *_gl << Point(0,0) << (int32_t)elapsed << "ms " << prompt;
    }
};


/*
 * Main entry point
 */

int main() {

  // set up SysTick at 1ms resolution
  MillisecondTimer::initialise();

  R61523Test test;
  test.run();

  // not reached
  return 0;
}
