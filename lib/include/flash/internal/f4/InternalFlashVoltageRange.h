/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


#ifndef STM32PLUS_F4
#error This class can only be used with the STM32F4 series
#endif


namespace stm32plus {

  /**
   * Program/erase operations on the F4 need a voltage range parameter from this enumeration.
   */

  enum class InternalFlashVoltageRange : uint8_t {
    VR_1_8_TO_2_1 = VoltageRange_1,         ///< operating range: 1.8V to 2.1V
    VR_2_1_TO_2_7 = VoltageRange_2,         ///< operating range: 2.1V to 2.7V
    VR_2_7_TO_3_6 = VoltageRange_3,         ///< operating range: 2.7V to 3.6V
    VR_2_7_TO_3_6_EXT_VPP = VoltageRange_4  ///< operating range: 2.7V to 3.6V with ext Vpp
  };
}

