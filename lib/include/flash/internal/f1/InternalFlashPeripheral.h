/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


#ifndef STM32PLUS_F1
#error This class can only be used with the STM32F1 series
#endif


namespace stm32plus {


  /**
   * Basic features for the internal flash peripheral
   */

  class InternalFlashPeripheral {

    public:
      constexpr uint32_t getPageSize(uint32_t address) const;
      constexpr bool isStartOfPage(uint32_t address) const;

      uint32_t getFlashSizeInBytes() const;
  };


  /**
   * Get the page size of this device
   * @return The page size
   */

  inline constexpr uint32_t InternalFlashPeripheral::getPageSize(uint32_t /* address */) const {

    #if defined(STM32PLUS_F1_HD) || defined(STM32PLUS_F1_CL_E)
      return 0x800;     // 2Kb
    #elif defined(STM32PLUS_F1_MD_VL)
      return 0x400;     // 1Kb
    #else
      #error Unsupported F1 variant
    #endif
  }


  /**
   * Check if the given address is at the start of a page
   * @param address The address
   * @return true if at the start of a page
   */

  constexpr bool InternalFlashPeripheral::isStartOfPage(uint32_t address) const {
    return address % getPageSize(address)==0;
  }


  /**
   * Read the total flash size in bytes
   * @return The total flash size
   */

  inline uint32_t InternalFlashPeripheral::getFlashSizeInBytes() const {
    return *reinterpret_cast<volatile uint16_t *>(0x1FFFF7E0)*1024;
  }
}
