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
 * ST7783 LCD test, show a looping graphics demo
 *
 * It's a 16-bit device and we control it in this demo
 * using the FSMC peripheral on bank 1. The wiring that
 * you need to do is as follows:
 *
 * PE1  => RESET
 * PE3  => RS (D/CX) (note different to usual demos)
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
 * PD13 => Backlight PWM (if variable backlight)
 *
 * The code works without structural change on both the
 * F1 and F4.
 *
 * Compatible MCU:
 * 	 STM32F1
 * 	 STM32F4
 *
 * Tested on devices:
 *   STM32F103VET6
 *   STM32F407VGT6
 */

class ST7783Test {

	protected:
		typedef Fsmc16BitAccessMode<FsmcBank1NorSram1> LcdAccessMode;
		typedef ST7783_Portrait_64K<LcdAccessMode> LcdPanel;

		LcdAccessMode *_accessMode;
		LcdPanel *_gl;
		Font *_font;

	public:
		void run() {

			// reset is on PE1 and RS (D/CX) is on PE3

			GpioE<DefaultDigitalOutputFeature<1>,
						DefaultAlternateFunctionFeature<GPIO_AF_FSMC,3> > pe;

			// set up the FSMC timing. these numbers (particularly the data setup time) are dependent on
			// both the FSMC bus speed and the panel timing parameters.

			Fsmc8080LcdTiming fsmcTiming(2,4);

			// set up the FSMC with RS=A19 (PE3)

			_accessMode=new LcdAccessMode(fsmcTiming,19,pe[1]);
			_gl=new LcdPanel(*_accessMode);

			// apply gamma settings

			ST7783Gamma gamma(0,0x0107,0,0x0203,0x0402,0,0x0207,0,0x0203,0x0403);
			_gl->applyGamma(gamma);

			// clear to black while the lights are out

			_gl->setBackground(0);
			_gl->clearScreen();

			// create the backlight on timer4, channel2 (PD13)

			DefaultBacklight backlight;

			// fade up to 100% in 4ms steps

			backlight.fadeTo(100,4);

			// create a font

			_font=new Font_VOLTER__28GOLDFISH_299;
			*_gl << *_font;

			_gl->setForeground(ColourNames::RED);
			_gl->fillRectangle(Rectangle(10,20,30,40));

			for(;;) {
				jpegTest();
				lzgTest();
				basicColoursTest();
				textTest();
				ellipseTest();
				gradientTest();
				rectTest();
				lineTest();
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

			if(_gl->getHeight()==320 && _gl->getWidth()==240) {

				prompt("JPEG bitmap test");

				LinearBufferInputOutputStream compressedData((uint8_t *)&JpegTest0Pixels,(uint32_t)&JpegTest0PixelsSize);
				_gl->drawJpeg(Rectangle(0,0,240,320),compressedData);

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

			drawCompressedBitmap((uint8_t *)&BulbPixels,(uint32_t)&BulbPixelsSize,89,148,false,dma);
			drawCompressedBitmap((uint8_t *)&AudioPixels,(uint32_t)&AudioPixelsSize,150,161,true,dma);
			drawCompressedBitmap((uint8_t *)&FlagPixels,(uint32_t)&FlagPixelsSize,144,220,false,dma);
			drawCompressedBitmap((uint8_t *)&DocPixels,(uint32_t)&DocPixelsSize,200,240,true,dma);
			drawCompressedBitmap((uint8_t *)&GlobePixels,(uint32_t)&GlobePixelsSize,193,219,false,dma);
		}


		void drawCompressedBitmap(uint8_t *pixels,uint32_t size,uint16_t width,uint16_t height,bool useDma,DmaFsmcLcdMemoryCopyFeature<LcdAccessMode>& dma) {

			_gl->setBackground(ColourNames::WHITE);
			_gl->clearScreen();

			LinearBufferInputOutputStream compressedData(pixels,size);
			LzgDecompressionStream decompressor(compressedData,size);

			if(useDma) {
				_gl->drawBitmap(
						Rectangle(((_gl->getXmax()+1)-width)/2,
								((_gl->getYmax()+1)-height)/2,
								width,height),
								decompressor,
								dma);
			}
			else {
				_gl->drawBitmap(
						Rectangle(((_gl->getXmax()+1)-width)/2,
								((_gl->getYmax()+1)-height)/2,
								width,height),
								decompressor);
			}

			MillisecondTimer::delay(3000);
		}


		void textTest() {

			int i;
			const char *str="The quick brown fox";
			Size size;
			Point p;
			uint32_t start,before,elapsed,chars;

			prompt("Stream operators test");

			*_gl << Point::Origin << "Let's see PI:";

			for(i=0;i<=7;i++)
				*_gl << Point(0,(1+i)*_font->getHeight()) << DoublePrecision(3.1415926535,i);

			MillisecondTimer::delay(5000);

			prompt("Opaque text test");

			size=_gl->measureString(*_font,str);

			before=MillisecondTimer::millis();
			chars=0;

			for(start=MillisecondTimer::millis();MillisecondTimer::millis()-start<5000;) {

				p.X=rand() % (_gl->getXmax()-size.Width);
				p.Y=rand() % (_gl->getYmax()-size.Height);

				_gl->setForeground(rand());
				_gl->writeString(p,*_font,str);

				chars+=19;
			}

			elapsed=MillisecondTimer::millis()-before;

			_gl->clearScreen();
			_gl->setForeground(ColourNames::WHITE);
			*_gl << Point::Origin << (chars*1000/elapsed)  << " characters/sec";

			MillisecondTimer::delay(3000);
		}


		void clearTest() {

			uint32_t start;

			prompt("Clear screen test");

			for(start=MillisecondTimer::millis();MillisecondTimer::millis()-start<5000;) {
				_gl->setBackground(rand());
				_gl->clearScreen();
			}
		}


		void basicColoursTest() {

			uint16_t i;

			static const uint32_t colours[7]={
			  ColourNames::RED,
			  ColourNames::GREEN,
			  ColourNames::BLUE,
			  ColourNames::CYAN,
			  ColourNames::MAGENTA,
			  ColourNames::YELLOW,
			  ColourNames::BLACK,
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
			uint32_t start;

			prompt("Line test");

			for(start=MillisecondTimer::millis();MillisecondTimer::millis()-start<5000;) {
				p1.X=rand() % _gl->getXmax();
				p1.Y=rand() % _gl->getYmax();
				p2.X=rand() % _gl->getXmax();
				p2.Y=rand() % _gl->getYmax();

				_gl->setForeground(rand());
				_gl->drawLine(p1,p2);
			}
		}

		void rectTest() {

			Rectangle rc;
			uint32_t start;

			prompt("Rectangle test");

			for(start=MillisecondTimer::millis();MillisecondTimer::millis()-start<5000;) {

				rc.X=(rand() % _gl->getXmax()/2);
				rc.Y=(rand() % _gl->getXmax()/2);
				rc.Width=rand() % (_gl->getXmax()-rc.X);
				rc.Height=rand() % (_gl->getYmax()-rc.Y);

				if(rc.Width>0 && rc.Height>0) {
					_gl->setForeground(rand());
					_gl->fillRectangle(rc);
				}
			}

			_gl->clearScreen();

			for(start=MillisecondTimer::millis();MillisecondTimer::millis()-start<5000;) {

				rc.X=(rand() % _gl->getXmax()/2);
				rc.Y=(rand() % _gl->getXmax()/2);
				rc.Width=rand() % (_gl->getXmax()-rc.X);
				rc.Height=rand() % (_gl->getYmax()-rc.Y);

				_gl->setForeground(rand());
				_gl->drawRectangle(rc);
			}
		}


		void ellipseTest() {

			int16_t i;
			Point p;
			Size s;
			uint32_t start;

			prompt("Ellipse test");
			_gl->setBackground(0);

			for(start=MillisecondTimer::millis();MillisecondTimer::millis()-start<5000;) {

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

				if(i % 1000==0)
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

	ST7783Test test;
	test.run();

	// not reached
	return 0;
}
