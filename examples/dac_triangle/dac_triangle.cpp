/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/dac.h"
#include "config/timer.h"


using namespace stm32plus;


/**
 * Demonstration of DAC channel 1 producing a triangular
 * wave superimposed upon a base output of 1/16 Vref.
 * Timer 6 is used as the trigger for each conversion.
 *
 * The output can be seen on PA4. Attach an oscilloscope
 * to see the triangular wave or take a look at
 * waveform_f1.png in the example subdirectory to see
 * how it looks on the STM32F103ZET6.
 *
 * Compatible MCU:
 * 	 STM32F1
 * 	 STM32F4
 *
 * Tested on devices:
 *   STM32F103ZET6
 *   STM32F407VGT6
 */

class DacTriangleTest {

	public:

		void run() {

			/*
			 * Set up Timer6 with an internal clock source and configured to be
			 * in master mode with update as the trigger
			 */

			Timer6<Timer6InternalClockFeature,TimerUpdateMasterFeature> timer;

			/*
			 * Configure the timer with a reload value of 255 and no prescaler.
			 */

			timer.initialiseTimeBase(255,0,TIM_CKD_DIV1,TIM_CounterMode_Up);

			/*
			 * Declare a DAC type with 12 bit right-aligned data.
			 */

			typedef Dac1<DacChannel112BitRightAlignmentFeature> MyDac;

			/*
			 * Create the DAC parameters. The trigger for conversion will be timer 6's
			 * trigger output and the wave generation mode is triangular.
			 */

			MyDac::Parameters params;
			params.dac_trigger=DAC_Trigger_T6_TRGO;
			params.dac_waveGeneration=DAC_WaveGeneration_Triangle;
			params.dac_lfsrMaskOrTriangleAmplitude=DAC_TriangleAmplitude_1023;

			/*
			 * Declare the DAC object
			 */

			MyDac dac(params);

			/*
			 * Set a base level of Vref/16. 4096 is the full 12 bit conversion value and that equals
			 * an output of Vref. Therefore 4096/16 = 256 for a base level of Vref/16. The triangle
			 * wave is added to this base value.
			 */

			dac.write(256);

			/*
			 * Enable the trigger output and then Start the timer. Each time the update event is
			 * generated a DAC conversion is triggered.
			 */

			timer.enableMasterFeature();
			timer.enablePeripheral();

			/*
			 * Finished. It's all running in the background now.
			 */

			for(;;);
		}
};


/*
 * Main entry point
 */

int main() {

	DacTriangleTest test;
	test.run();

	// not reached
	return 0;
}
