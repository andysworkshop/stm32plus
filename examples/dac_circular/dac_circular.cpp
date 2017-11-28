/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/dac.h"

using namespace stm32plus;


/**
 * Demonstration of DAC channel 1 playing back a sampled waveform.
 *
 * Compatible MCU:
 *   STM32F0
 *   STM32F1
 *   STM32F4
 *
 * Tested on devices
 *   STM32F051R8T6
 *   STM32F103ZET6
 *   STM32F407VGT6
 */

// A sine wave centered around 2048

static uint16_t samples[] = { 2048, 2128, 2208, 2288, 2367, 2446, 2523,
    2599, 2674, 2748, 2820, 2890, 2958, 3023, 3087, 3148, 3206, 3261, 3314,
    3363, 3410, 3453, 3492, 3529, 3561, 3590, 3615, 3637, 3654, 3668, 3678,
    3684, 3686, 3684, 3678, 3668, 3654, 3637, 3615, 3590, 3561, 3529, 3492,
    3453, 3410, 3363, 3314, 3261, 3206, 3148, 3087, 3023, 2958, 2890, 2820,
    2748, 2674, 2599, 2523, 2446, 2367, 2288, 2208, 2128, 2048, 1967, 1887,
    1807, 1728, 1649, 1572, 1496, 1421, 1347, 1275, 1205, 1137, 1072, 1008, 947,
    889, 834, 781, 732, 685, 642, 603, 566, 534, 505, 480, 458, 441, 427, 417,
    411, 409, 411, 417, 427, 441, 458, 480, 505, 534, 566, 603, 642, 685, 732,
    781, 834, 889, 947, 1008, 1072, 1137, 1205, 1275, 1347, 1421, 1496, 1572,
    1649, 1728, 1807, 1887, 1967
};

enum {
  NUM_SAMPLES = sizeof(samples) / sizeof(samples[0])
};


#if defined(STM32PLUS_F0)

  typedef Dac1UpdateDmaChannel<Dac1UpdateDmaChannelInterruptFeature,
                               DacDmaWriterFeature<DacChannel112BitRightAlignmentFeature,
                                                   DMA_Priority_High,
                                                   DMA_Mode_Circular> > MyDma;

  typedef Dac1UpdateDmaChannelInterruptFeature InterruptFeature;

#elif defined(STM32PLUS_F1)

  typedef Dac1DmaChannel<DacChannel1DmaChannelInterruptFeature,
                         DacDmaWriterFeature<DacChannel112BitRightAlignmentFeature,
                                             DMA_Priority_High,
                                             DMA_Mode_Circular> > MyDma;

  typedef DacChannel1DmaChannelInterruptFeature InterruptFeature;

#elif defined(STM32PLUS_F4)

  typedef Dac1DmaChannel<Dac1DmaChannelInterruptFeature,
                         DacDmaWriterFeature<DacChannel112BitRightAlignmentFeature,
                                             DMA_Priority_High,
                                             DMA_FIFOMode_Disable,
                                             DMA_Mode_Circular> > MyDma;

  typedef Dac1DmaChannelInterruptFeature InterruptFeature;

#endif

class DacCircularTest {

public:
  void run() {

    Dac1<>::Parameters params;
    params.dac_trigger = DAC_Trigger_T6_TRGO; // Output only changes on trigger
    params.dac_waveGeneration = DAC_WaveGeneration_None;
    params.dac_outputBuffer = DAC_OutputBuffer_Enable;

    Dac1<DacChannel112BitLeftAlignmentFeature> dac(params);

    MyDma dma;

    dma.DmaInterruptEventSender.insertSubscriber(
        DmaInterruptEventSourceSlot::bind(this, &DacCircularTest::onComplete));

    // Must enable all desired interrupts in one call

    dma.enableInterrupts(InterruptFeature::HALF_COMPLETE | InterruptFeature::COMPLETE);

    // Samples output on every second tick of the time base

    Timer6<Timer6InternalClockFeature, TimerUpdateMasterFeature> timer;
    timer.setTimeBaseByFrequency(NUM_SAMPLES * 440 * 2, 1, TIM_CounterMode_Up);
    timer.enableMasterFeature();

    dac.enablePeripheral();
    timer.enablePeripheral();

    dma.beginWrite(samples, NUM_SAMPLES);

    // it's running in the background now

    for(;;);
  }

  /*
   * Subscriber callback is fired when the DMA transfer is (half)complete.
   * We could refresh the half that was just completed or just let it loop.s
   */

  void onComplete(DmaEventType det) {
    if (det == DmaEventType::EVENT_COMPLETE) {
      // refill second half of buffer (or just leave it alone)
    }
    if (det == DmaEventType::EVENT_HALF_COMPLETE) {
      // refill first half of buffer (or just leave it alone)
    }
  }
};


/*
 * Main entry point
 */

int main() {

  DacCircularTest test;
  test.run();

  // not reached
  return 0;
}
