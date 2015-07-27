/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Implementation of the basic flash operations. Note that InternalFlashWriteFeature must be in the
   * hierarchy if any of the program/erase operations are called
   */

  class InternalFlashPeripheral {

    public:
      enum {
        E_ERASE_FAILED = 1,          ///< erase operation failed: see extended code for reason
        E_PROGRAM_FAILED = 2         ///< program operation failed: see extended code for reason
      };

    public:
      constexpr uint32_t getPageSize() const;

      bool chipErase() const;
      bool pageErase(uint32_t flashAddress) const;

      bool wordProgram(uint32_t flashAddress,uint32_t data) const;
      bool halfWordProgram(uint32_t flashAddress,uint32_t data) const;
  };


  /**
   * Erase the entire device - not clever if you're running from flash
   * @return true if it worked and the operation has completed.
   */

  inline bool InternalFlashPeripheral::chipErase() const {

    uint32_t err;

    // erase everything

    if((err=FLASH_EraseAllPages())==FLASH_COMPLETE)
      return true;

    return errorProvider.set(ErrorProvider::INTERNAL_FLASH,E_ERASE_FAILED,err);
  }


  /**
   * Erase a page.
   * @param flashAddress. The address to erase - must be a page multiple.
   * @return true if it worked and the operation has completed.
   */

  inline bool InternalFlashPeripheral::pageErase(uint32_t flashAddress) const {

    uint32_t err;

    if((err=FLASH_ErasePage(flashAddress))==FLASH_COMPLETE)
      return true;

    return errorProvider.set(ErrorProvider::INTERNAL_FLASH,E_ERASE_FAILED,err);
  }


  /**
   * Program a 32-bit word
   * @param flashAddress The address to program. Must be a 4-byte boundary.
   * @param data The 32-bit word to program
   * @return true if it worked and the operation has completed.
   */

  inline bool InternalFlashPeripheral::wordProgram(uint32_t flashAddress,uint32_t data) const {

    uint32_t err;

    if((err=FLASH_ProgramWord(flashAddress,data))==FLASH_COMPLETE)
      return true;

    return errorProvider.set(ErrorProvider::INTERNAL_FLASH,E_PROGRAM_FAILED,err);
  }


  /**
   * Program a 16-bit half word
   * @param flashAddress The address to program. Must be a 2-byte boundary.
   * @param data The 16-bit half-word to program
   * @return true if it worked and the operation has completed.
   */

  inline bool InternalFlashPeripheral::halfWordProgram(uint32_t flashAddress,uint16_t data) const {

    uint32_t err;

    if((err=FLASH_ProgramHalfWord(flashAddress,data))==FLASH_COMPLETE)
      return true;

    return errorProvider.set(ErrorProvider::INTERNAL_FLASH,E_PROGRAM_FAILED,err);
  }


  /**
   * Get the page size of this device
   * @return The page size
   */

  inline constexpr uint32_t InternalFlashPeripheral::getPageSize() const {

    #if defined(STM32F072) || defined(STM32F091)
      return 0x800;     // 2Kb
    #else
      return 0x400;     // 1Kb
    #endif
  }
}
