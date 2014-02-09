/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#if !defined(STM32PLUS_F1) && !defined(STM32PLUS_F4)
#error This class can only be used with the STM32F1 and STM32F4 series
#endif


namespace stm32plus {


  /**
   * Feature class to enable the DAC in dual channel mode
   *
   * @tparam TChannel The DAC channel (DAC_Channel_1/2)
   * @tparam TAlignment The data alignment (DAC_Align_12b_R / DAC_Align_12b_L / DAC_Align_8b_R)
   * @tparam whether this channel represents a dual mode (not a user parameter, leave the default as false)
   */

  template<uint32_t TAlignment>
  class DacDualChannelFeature : public DacFeatureBase {

    public:
      enum {
        DUAL_CHANNEL_FEATURE = 1
      };

    public:
      DacDualChannelFeature(Dac& dac);

      static uint32_t getAlignment();
      static void write(uint16_t data1,uint16_t data2);

      void triggerOn() const;
      void triggerOff() const;
  };


  /**
   * Constructor
   * @param dac
   */

  template<uint32_t TAlignment>
  inline DacDualChannelFeature<TAlignment>::DacDualChannelFeature(Dac& dac)
    : DacFeatureBase(dac) {
  }


  /**
   * Get the alignment
   */

  template<uint32_t TAlignment>
  inline uint32_t DacDualChannelFeature<TAlignment>::getAlignment() {
    return TAlignment;
  }


  /**
   * Write data
   */

  template<uint32_t TAlignment>
  inline void DacDualChannelFeature<TAlignment>::write(uint16_t data1,uint16_t data2) {

    DAC_SetDualChannelData(TAlignment,data1,data2);
  }


  /**
   * Typedefs for all combinations of cases
   */

  typedef DacDualChannelFeature<DAC_Align_12b_R> DacDualChannel12BitRightAlignmentFeature;
  typedef DacDualChannelFeature<DAC_Align_12b_L> DacDualChannel12BitLeftAlignmentFeature;
  typedef DacDualChannelFeature<DAC_Align_8b_R> DacDualChannel8BitRightAlignmentFeature;
}
