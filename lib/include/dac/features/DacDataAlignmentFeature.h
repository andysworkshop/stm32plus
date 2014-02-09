/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Feature class to enable the 3 DAC data alignment modes
   *
   * @tparam TChannel The DAC channel (DAC_Channel_1/2). Only channel 1 is valid on the F0.
   * @tparam TAlignment The data alignment (DAC_Align_12b_R / DAC_Align_12b_L / DAC_Align_8b_R)
   */

  template<uint32_t TChannel,uint32_t TAlignment>
  class DacDataAlignmentFeature : public DacFeatureBase {

    public:
      enum {
        DUAL_CHANNEL_FEATURE = 0
      };

    public:
      DacDataAlignmentFeature(Dac& dac);

      static uint32_t getAlignment();
      static uint32_t getChannel();
      static void write(uint16_t data);
  };


  /**
   * Constructor
   * @param dac
   */

  template<uint32_t TChannel,uint32_t TAlignment>
  inline DacDataAlignmentFeature<TChannel,TAlignment>::DacDataAlignmentFeature(Dac& dac)
    : DacFeatureBase(dac) {
  }


  /**
   * Get the alignment
   */

  template<uint32_t TChannel,uint32_t TAlignment>
  inline uint32_t DacDataAlignmentFeature<TChannel,TAlignment>::getAlignment() {
    return TAlignment;
  }


  /**
   * Get the channel
   */

  template<uint32_t TChannel,uint32_t TAlignment>
  inline uint32_t DacDataAlignmentFeature<TChannel,TAlignment>::getChannel() {
    return TChannel;
  }


  /**
   * Write data
   */

  template<uint32_t TChannel,uint32_t TAlignment>
  inline void DacDataAlignmentFeature<TChannel,TAlignment>::write(uint16_t data) {

    DAC_SetChannel1Data(TAlignment,data);

#if defined(STM32PLUS_F0)
#else

    // This is a comparison of constants - the optimiser will eliminate the comparison completely

    if(TChannel==DAC_Channel_1)
      DAC_SetChannel1Data(TAlignment,data);
    else
      DAC_SetChannel2Data(TAlignment,data);

#endif
  }


  /**
   * Typedefs for all combinations of cases
   */

  typedef DacDataAlignmentFeature<DAC_Channel_1,DAC_Align_12b_R> DacChannel112BitRightAlignmentFeature;
  typedef DacDataAlignmentFeature<DAC_Channel_1,DAC_Align_12b_L> DacChannel112BitLeftAlignmentFeature;
  typedef DacDataAlignmentFeature<DAC_Channel_1,DAC_Align_8b_R> DacChannel18BitRightAlignmentFeature;

#if !defined(STM32PLUS_F0)
  typedef DacDataAlignmentFeature<DAC_Channel_2,DAC_Align_12b_R> DacChannel212BitRightAlignmentFeature;
  typedef DacDataAlignmentFeature<DAC_Channel_2,DAC_Align_12b_L> DacChannel212BitLeftAlignmentFeature;
  typedef DacDataAlignmentFeature<DAC_Channel_2,DAC_Align_8b_R> DacChannel28BitRightAlignmentFeature;
#endif
}
