/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
	namespace display {

		/**
		 * @brief Gamma values for the ST7783
		 *
		 * Sample for STM32 dev board display:
		 * 0x7,0x302,0x105,0x206,0x808,0x206,0x504,0x7,0x105,0x808
		 */

		class ST7783Gamma : public DisplayDeviceGamma {

			public:

			/**
			 * Constructor. The ST7783 takes 10 gamma values. The constructor must be supplied with
			 * those values.
			 */

				ST7783Gamma(
						uint16_t g1,uint16_t g2,uint16_t g3,uint16_t g4,uint16_t g5,
						uint16_t g6,uint16_t g7,uint16_t g8,uint16_t g9,uint16_t g10) : DisplayDeviceGamma(10) {

					_gamma[0]=g1;
					_gamma[1]=g2;
					_gamma[2]=g3;
					_gamma[3]=g4;
					_gamma[4]=g5;
					_gamma[5]=g6;
					_gamma[6]=g7;
					_gamma[7]=g8;
					_gamma[8]=g9;
					_gamma[9]=g10;
				}
		};

	}
}
