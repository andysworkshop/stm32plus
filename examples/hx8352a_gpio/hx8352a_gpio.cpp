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
 * This example shows a looping graphics library demo running on an
 * HX8352A display panel. The panel in this demo is from the LG KF700
 * cellphone (see my website for writeup). The access mode is the
 * highly optimised GPIO mode in 64K colours, landscape mode.
 *
 * The pinout is changeable to your needs. The demo setup is as follows.
 *
 * LCD_D0..15 = PD[0..15]				// a whole port is used for the 16 data lines
 * LCD RESET  = PE0             // RESET/WR/RS must be on the same port
 * LCD_WR     = PE1
 * LCD_RS     = PE2
 * LCD_VSYNC  = PE3							// input pin for vsync synchronisation
 *
 * Backlight PWM output = PA1
 *
 * Compatible MCU:
 * 	 STM32F1
 * 	 STM32F4
 *
 * Tested on devices:
 *   STM32F103VET6
 *   STM32F407VGT6
 */

class HX8352ATest {

	public:

		/**
		 * Gpio16AccessMode is templated with the HX8352ATest class. Therefore it will expect to
		 * find the following constants available for static access
		 */

		enum {
			Port_CONTROL = GPIOE_BASE,				// will use [0..2]
			Port_DATA    = GPIOD_BASE,				// will use whole port as data bus D0..15

			Pin_RESET    = GPIO_Pin_0,
			Pin_WR       = GPIO_Pin_1,
			Pin_RS       = GPIO_Pin_2
		};

	protected:
		typedef Gpio16BitAccessMode_HX8352A_64K<HX8352ATest> LcdAccessMode;
		typedef LG_KF700_Landscape_64K<LcdAccessMode> LcdPanel;

		// define the PWM backlight to come from timer5, channel 2 (PA1)

		typedef Backlight<Timer5<Timer5InternalClockFeature,
														 TimerChannel2Feature,
		                         Timer5GpioFeature<TIMER_REMAP_NONE,TIM5_CH2_OUT>
		                         > > MyBacklight;

		LcdAccessMode *_accessMode;
		LcdPanel *_gl;
		Exti3 *_exti;
		Font *_font;
		volatile bool _vsync;

	public:
		void run() {

			// set up the access mode

			_accessMode=new LcdAccessMode;

			// declare a panel

			_gl=new LcdPanel(*_accessMode);

			// apply gamma settings

			HX8352AGamma gamma(0xA0,0x03,0x00,0x45,0x03,0x47,0x23,0x77,0x01,0x1F,0x0F,0x03);
			_gl->applyGamma(gamma);

			// clear to black while the lights are out

			_gl->setBackground(0);
			_gl->clearScreen();

			// the LG KF700 used for this demo gives us a vsync signal that we can synchronize
			// our drawing with. let's connect it to PE3 but don't subscribe to interrupts
			// until we get to that part of the demo

			GpioE<DefaultDigitalInputFeature<3>> pe;
			_exti=new Exti3(EXTI_Mode_Interrupt,EXTI_Trigger_Rising,pe[3]);

			// create the backlight on timer5, channel2 (PA1)

			MyBacklight backlight;

			// fade up to 100% in 4ms steps

			backlight.fadeTo(100,4);

			// create a font

			_font=new Font_VOLTER__28GOLDFISH_299;
			*_gl << *_font;

			for(;;) {
				clearTest();
				textTest();
				basicColoursTest();
				rectTest();
				lzgTest();
				lineTest();
				jpegTest();
				scrollTest();
				ellipseTest();
				gradientTest();
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

			// only draw in portrait mode and if it can fit on screen

			if(_gl->getHeight()>_gl->getWidth() && _gl->getHeight()>=320 && _gl->getWidth()>=240) {

				prompt("JPEG bitmap test");

				// draw it centered

				LinearBufferInputOutputStream compressedData((uint8_t *)&JpegTest0Pixels,(uint32_t)&JpegTest0PixelsSize);
				_gl->drawJpeg(Rectangle((_gl->getWidth()-240)/2,(_gl->getHeight()-320)/2,240,320),compressedData);

				MillisecondTimer::delay(3000);
			}
		}

		void lzgTest() {

			prompt("LZG bitmap test");

			drawCompressedBitmap((uint8_t *)&BulbPixels,(uint32_t)&BulbPixelsSize,89,148);
			drawCompressedBitmap((uint8_t *)&AudioPixels,(uint32_t)&AudioPixelsSize,150,161);
			drawCompressedBitmap((uint8_t *)&FlagPixels,(uint32_t)&FlagPixelsSize,144,220);
			drawCompressedBitmap((uint8_t *)&DocPixels,(uint32_t)&DocPixelsSize,200,240);
			drawCompressedBitmap((uint8_t *)&GlobePixels,(uint32_t)&GlobePixelsSize,193,219);
		}


		void drawCompressedBitmap(uint8_t *pixels,uint32_t size,uint16_t width,uint16_t height) {

			_gl->setBackground(ColourNames::WHITE);
			_gl->clearScreen();

			LinearBufferInputOutputStream compressedData(pixels,size);
			LzgDecompressionStream decompressor(compressedData,size);

			_gl->drawBitmap(
					Rectangle((_gl->getWidth()-width)/2,
							(_gl->getHeight()-height)/2,
							width,height),
							decompressor);

			MillisecondTimer::delay(3000);
		}


		void textTest() {

			int i;
			const char *str="The quick brown fox";
			Size size;
			Point p;
			uint32_t before,elapsed;

			prompt("Stream operators test");

			*_gl << Point::Origin << "Let's see PI:";

			for(i=0;i<=7;i++)
				*_gl << Point(0,(1+i)*_font->getHeight()) << DoublePrecision(3.1415926535,i);

			MillisecondTimer::delay(5000);

			prompt("Opaque text test");

			size=_gl->measureString(*_font,str);

			before=MillisecondTimer::millis();

			for(i=0;i<3000;i++) {

				p.X=rand() % (_gl->getXmax()-size.Width);
				p.Y=rand() % (_gl->getYmax()-size.Height);

				_gl->setForeground(rand());
				_gl->writeString(p,*_font,str);
			}

			elapsed=MillisecondTimer::millis()-before;

			_gl->clearScreen();
			_gl->setForeground(ColourNames::WHITE);
			*_gl << Point::Origin << "Elapsed: " << (int32_t)elapsed << "ms";

			MillisecondTimer::delay(3000);
		}


		void scrollTest() {

			int32_t i,j,numRows;
			Point p;

			prompt("Hardware scrolling test");

			_gl->setForeground(0xffffff);
			_gl->setBackground(0);
			_gl->clearScreen();

			numRows=((_gl->getYmax()+1)/_font->getHeight())/3;

			p.X=0;

			for(i=0;i<numRows;i++) {

				p.Y=(numRows+i)*_font->getHeight();
				*_gl << p << "Test row " << i;
			}

			for(j=0;j<15;j++) {

				numRows=(_gl->getYmax()+1)/4;

				for(i=0;i<numRows;i++) {
					_gl->setScrollPosition(i);
					MillisecondTimer::delay(5);
				}

				for(i=0;i<numRows;i++) {
					_gl->setScrollPosition(numRows-i);
					MillisecondTimer::delay(5);
				}
			}

			_gl->setScrollPosition(0);
		}


		void clearTest() {

			int i;
			uint32_t start,duration;

			// first run doesn't synchronise with vsync. tearing will be visible.

			prompt("Clear screen test (no vsync)");

			start=MillisecondTimer::millis();
			for(i=0;i<200;i++) {
				_gl->setBackground(rand());
				_gl->clearScreen();
			}
			duration=(MillisecondTimer::millis()-start)/200;

			_gl->setForeground(ColourNames::WHITE);
			_gl->setBackground(ColourNames::BLACK);
			_gl->clearScreen();

			stopTimer("to clear one screen",duration);
			MillisecondTimer::delay(3000);

			prompt("Clear screen test (vsync synchronisation enabled)");

			// subscribe to the interrupts

			_exti->ExtiInterruptEventSender.insertSubscriber(
					ExtiInterruptEventSourceSlot::bind(this,&HX8352ATest::onVsyncInterrupt)
				);

			start=MillisecondTimer::millis();
			for(i=0;i<200;i++) {

				_gl->setBackground(rand());

				// wait for the next vsync signal

				_vsync=false;
				while(!_vsync);

				// clear screen and reset the flag

				_gl->clearScreen();
			}
			duration=(MillisecondTimer::millis()-start)/200;

			// unsubscribe from the interrupts

			_exti->ExtiInterruptEventSender.removeSubscriber(
					ExtiInterruptEventSourceSlot::bind(this,&HX8352ATest::onVsyncInterrupt)
				);

			_gl->setForeground(ColourNames::WHITE);
			_gl->setBackground(ColourNames::BLACK);
			_gl->clearScreen();

			stopTimer("to clear one screen",duration);
			MillisecondTimer::delay(3000);
		}

		/**
		 * Interrupt callback from the EXTI interrupt
		 */

		void onVsyncInterrupt(uint8_t /* extiLine */) {
			_vsync=true;
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

			int i;
			Rectangle rc;

			prompt("Rectangle test");

			for(i=0;i<1500;i++) {

				rc.X=(rand() % _gl->getXmax()/2);
				rc.Y=(rand() % _gl->getYmax()/2);
				rc.Width=rand() % (_gl->getXmax()-rc.X);
				rc.Height=rand() % (_gl->getYmax()-rc.Y);

				_gl->setForeground(rand());
				_gl->fillRectangle(rc);
			}

			_gl->clearScreen();

			for(i=0;i<10000;i++) {

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

			int16_t i;
			Point p;
			Size s;

			prompt("Ellipse test");
			_gl->setBackground(0);

			for(i=0;i<1000;i++) {

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

			for(i=0;i<1500;i++) {

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

	HX8352ATest test;
	test.run();

	// not reached
	return 0;
}
