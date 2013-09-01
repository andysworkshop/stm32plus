/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

namespace stm32plus {
	namespace display {

		/**
		 * @brief Interactively set the gamma parameters for an LCD panel.
		 *
		 * Class to allow a user to interactively test and set gamma parameters for a panel.
		 * Displays red, green, blue, white, cyan, magenta, yellow and 50% grey boxes as well
		 * as a 0..100% grey scale band.
		 *
		 * The user can use the PC keyboard to communicate through a terminal attached to the
		 * USART to set the gamma levels and see those values displayed as text at the bottom
		 * of the panel. The keys recognised are:
		 *
		 * - 0-7: toggle the corresponding bit in the current gamma parameter.
		 * - q: move to the previous gamma value.
		 * - w: move to th next gamma value.
		 * - a: increase the current gamma value by 1.
		 * - z: decrease the current gamma value by 1.
		 * - s: increase the current gamma value by 10.
		 * - x: decrease the current gamma value by 10.
		 * - d: increase the current gamma value by 100.
		 * - c: decrease the current gamma value by 100.
		 */

		template<class TGraphicsLibrary,class TGamma>
		class InteractiveGamma {
			protected:
				TGraphicsLibrary& _gl;
				TGamma& _gamma;
				InputStream& _inputStream;
				int _activeIndex;
				int _count;
				int _fontHeight;

			protected:
				void drawBoxes();
				void drawBox(int16_t x,int16_t y,typename TGraphicsLibrary::tCOLOUR cr);
				void drawConsole();
				void move(int howMuch);
				void changeValue(int howMuch);
				void bitset(uint16_t bit);

			public:
				InteractiveGamma(TGraphicsLibrary& gl,TGamma& gamma,InputStream& is);

				void run();
		};

		/**
		 * Constructor. References taken by this constructor are owned by the caller and must not go
		 * out of scope for the life of this class.
		 *
		 * @param[in] gl A GraphicsLibrary instance for this LCD.
		 * @param[in] is The input stream to read the keyboard input from. Most probably a UsartInputStream
		 * @param[in] gamma The gamma values to start with
		 */

		template<class TGraphicsLibrary,class TGamma>
		InteractiveGamma<TGraphicsLibrary,TGamma>::InteractiveGamma(TGraphicsLibrary& gl,TGamma& gamma,InputStream& is) :
				_gl(gl), _gamma(gamma), _inputStream(is) {

			_activeIndex=0;
			_count=_gamma.getGammaCount();
			_fontHeight=gl.getStreamSelectedFont()->getHeight();
		}

		/**
		 * Draw the coloured boxes
		 */

		template<class TGraphicsLibrary,class TGamma>
		void InteractiveGamma<TGraphicsLibrary,TGamma>::drawBoxes() {

			uint16_t width,height;
			uint32_t i;

			width=_gl.getWidth() / 4;
			height=(_gl.getHeight() - (_fontHeight * 2)) / 3;

			_gl.setBackground(0);
			_gl.clearScreen();

			drawBox(0,0,ColourNames::RED);
			drawBox(width,0,ColourNames::GREEN);
			drawBox(width * 2,0,ColourNames::BLUE);
			drawBox(width * 3,0,ColourNames::WHITE);

			drawBox(0,height,ColourNames::CYAN);
			drawBox(width,height,ColourNames::MAGENTA);
			drawBox(width * 2,height,ColourNames::YELLOW);
			drawBox(width * 3,height,0x808080); // mid grey

			for(i=0;i < 255;i++) {
				_gl.setForeground(i << 16 | i << 8 | i);
				_gl.fillRectangle(Rectangle(i,height * 2,1,height));
			}

			drawConsole();
		}


		/*
		 * Draw one box
		 */

		template<class TGraphicsLibrary,class TGamma>
		void InteractiveGamma<TGraphicsLibrary,TGamma>::drawBox(int16_t x,int16_t y,typename TGraphicsLibrary::tCOLOUR cr) {
			int width,height;

			width=_gl.getWidth() / 4;
			height=(_gl.getHeight() - (_fontHeight * 2)) / 3;

			_gl.setForeground(cr);
			_gl.fillRectangle(Rectangle(x,y,width,height));
		}


		/*
		 * Draw the lower console
		 */

		template<class TGraphicsLibrary,class TGamma>
		void InteractiveGamma<TGraphicsLibrary,TGamma>::drawConsole() {
			char value[10];
			int16_t i;
			Point p;

			// clear out

			_gl.setForeground(0);
			_gl.fillRectangle(Rectangle(0,_gl.getHeight() - _fontHeight * 2,_gl.getWidth(),_fontHeight * 2));

			// draw console

			_gl << Point(0,_gl.getHeight() - (_fontHeight * 2));
			_gl.setForeground(ColourNames::WHITE);

			for(i=0;i < _count;i++) {

				if(i == _count / 2)
					_gl << Point(0,_gl.getHeight() - _fontHeight);

				_gl << StringUtil::itoa(i + 1,value,10) << "=" << StringUtil::itoa(_gamma[i],value,16) << " ";
			}
		}

		/**
		 * Move the active gamma
		 */

		template<class TGraphicsLibrary,class TGamma>
		void InteractiveGamma<TGraphicsLibrary,TGamma>::move(int howMuch) {
			_activeIndex+=howMuch;

			if(_activeIndex < 0)
				_activeIndex=0;
			else if(_activeIndex >= _count)
				_activeIndex=_count - 1;
		}

		/**
		 * Change the gamma by a value
		 */

		template<class TGraphicsLibrary,class TGamma>
		void InteractiveGamma<TGraphicsLibrary,TGamma>::changeValue(int howMuch) {
			_gamma[_activeIndex]+=howMuch;
			_gl.applyGamma(_gamma);

			drawConsole();
		}

		/**
		 * Set or reset a value
		 */

		template<class TGraphicsLibrary,class TGamma>
		void InteractiveGamma<TGraphicsLibrary,TGamma>::bitset(uint16_t bit) {
			uint16_t value=_gamma[_activeIndex];

			if((value & (1 << bit)) == 0)
				value|=1 << bit;
			else
				value&=~(1 << bit);

			_gamma[_activeIndex]=value;
			drawConsole();
		}

		/**
		 * Run interactively. Never returns.
		 */

		template<class TGraphicsLibrary,class TGamma>
		void InteractiveGamma<TGraphicsLibrary,TGamma>::run() {
			char c;

			drawBoxes();

			for(;;) {

				switch((c=_inputStream.read())) {

					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
						bitset(c - '0');
						break;

					case 'q':
						move(-1);
						break;

					case 'w':
						move(1);
						break;

					case 'a':
						changeValue(1);
						break;

					case 'z':
						changeValue(-1);
						break;

					case 's':
						changeValue(10);
						break;

					case 'x':
						changeValue(-10);
						break;

					case 'd':
						changeValue(100);
						break;

					case 'c':
						changeValue(-100);
						break;
				}
			}
		}
	}
}
