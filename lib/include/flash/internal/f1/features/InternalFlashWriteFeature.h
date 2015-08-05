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
   * Implementation of the basic flash operations. Note that the flash must be write enabled
   * for any of these methods to work.
   */

  class InternalFlashWriteFeature : public InternalFlashFeatureBase {

    public:
      enum {
        E_ERASE_FAILED = 1,          ///< erase operation failed: see extended code for reason
        E_PROGRAM_FAILED = 2         ///< program operation failed: see extended code for reason
      };

    public:
      InternalFlashWriteFeature(InternalFlashPeripheral& flashPeripheral);

      bool chipErase() const;
      bool pageErase(uint32_t flashAddress) const;

      bool wordProgram(uint32_t flashAddress,uint32_t data) const;
      bool halfWordProgram(uint32_t flashAddress,uint16_t data) const;
  };


  /**
   * Constructor
   * @param flashPeripheral reference to the peripheral class
   */

  inline InternalFlashWriteFeature::InternalFlashWriteFeature(InternalFlashPeripheral& flashPeripheral)
    : InternalFlashFeatureBase(flashPeripheral) {
  }


  /**
   * Erase the entire device - not clever if you're running from flash
   * @return true if it worked and the operation has completed.
   */

  inline bool InternalFlashWriteFeature::chipErase() const {

    uint32_t err;

    // erase everything

    if((err=FLASH_EraseAllPages())==FLASH_COMPLETE)
      return true;

    return errorProvider.set(ErrorProvider::ERROR_PROVIDER_INTERNAL_FLASH,E_ERASE_FAILED,err);
  }


  /**
   * Erase a page.
   * @param flashAddress. The address to erase - must be a page multiple.
   * @return true if it worked and the operation has completed.
   */

  inline bool InternalFlashWriteFeature::pageErase(uint32_t flashAddress) const {

    uint32_t err;

    if((err=FLASH_ErasePage(flashAddress))==FLASH_COMPLETE)
      return true;

    return errorProvider.set(ErrorProvider::ERROR_PROVIDER_INTERNAL_FLASH,E_ERASE_FAILED,err);
  }


  /**
   * Program a 32-bit word
   * @param flashAddress The address to program. Must be a 4-byte boundary.
   * @param data The 32-bit word to program
   * @return true if it worked and the operation has completed.
   */

  inline bool InternalFlashWriteFeature::wordProgram(uint32_t flashAddress,uint32_t data) const {

    uint32_t err;

    if((err=FLASH_ProgramWord(flashAddress,data))==FLASH_COMPLETE)
      return true;

    return errorProvider.set(ErrorProvider::ERROR_PROVIDER_INTERNAL_FLASH,E_PROGRAM_FAILED,err);
  }


  /**
   * Program a 16-bit half word
   * @param flashAddress The address to program. Must be a 2-byte boundary.
   * @param data The 16-bit half-word to program
   * @return true if it worked and the operation has completed.
   */

  inline bool InternalFlashWriteFeature::halfWordProgram(uint32_t flashAddress,uint16_t data) const {

    uint32_t err;

    if((err=FLASH_ProgramHalfWord(flashAddress,data))==FLASH_COMPLETE)
      return true;

    return errorProvider.set(ErrorProvider::ERROR_PROVIDER_INTERNAL_FLASH,E_PROGRAM_FAILED,err);
  }
}
