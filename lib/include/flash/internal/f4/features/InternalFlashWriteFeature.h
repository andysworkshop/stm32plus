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
   * Implementation of the basic flash operations. Note that the flash must be write enabled
   * for any of these methods to work.
   *
   * @tparam TVoltageRange a value from the VoltageRange
   */

  template<InternalFlashVoltageRange TVoltageRange>
  class InternalFlashWriteFeature  : public InternalFlashFeatureBase {

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


  /*
   * Some typedefs for common MCU configurations
   */

  typedef InternalFlashWriteFeature<InternalFlashVoltageRange::VR_2_7_TO_3_6> DefaultF407InternalFlashWriteFeature;


  /**
   * Constructor
   * @param flashPeripheral reference to the peripheral class
   */

  template<InternalFlashVoltageRange TVoltageRange>
  inline InternalFlashWriteFeature<TVoltageRange>::InternalFlashWriteFeature(InternalFlashPeripheral& flashPeripheral)
    : InternalFlashFeatureBase(flashPeripheral) {
  }


  /**
   * Erase the entire device - not clever if you're running from flash
   * @return true if it worked and the operation has completed.
   */

  template<InternalFlashVoltageRange TVoltageRange>
  inline bool InternalFlashWriteFeature<TVoltageRange>::chipErase() const {

    uint32_t err;

    // erase everything

    if((err=FLASH_EraseAllSectors(static_cast<uint8_t>(TVoltageRange)))==FLASH_COMPLETE)
      return true;

    return errorProvider.set(ErrorProvider::ERROR_PROVIDER_INTERNAL_FLASH,E_ERASE_FAILED,err);
  }


  /**
   * Erase a page.
   * @param flashAddress. The address to erase - must be a page multiple.
   * @return true if it worked and the operation has completed.
   */

  template<InternalFlashVoltageRange TVoltageRange>
  inline bool InternalFlashWriteFeature<TVoltageRange>::pageErase(uint32_t flashAddress) const {

    uint32_t err;
    uint8_t pageNumber;

    // FLASH_EraseSector takes a coded sector number

    pageNumber=_flashPeripheral.getPageFromAddress(flashAddress);
    if(pageNumber<12)
      pageNumber*=8;                        // the low range (1Mb devices)
    else
      pageNumber=0x80+((pageNumber-12)*8);  // the high rage (2Mb devices)

    if((err=FLASH_EraseSector(pageNumber,static_cast<uint8_t>(TVoltageRange)))==FLASH_COMPLETE)
      return true;

    return errorProvider.set(ErrorProvider::ERROR_PROVIDER_INTERNAL_FLASH,E_ERASE_FAILED,err);
  }


  /**
   * Program a 32-bit word
   * @param flashAddress The address to program. Must be a 4-byte boundary.
   * @param data The 32-bit word to program
   * @return true if it worked and the operation has completed.
   */

  template<InternalFlashVoltageRange TVoltageRange>
  inline bool InternalFlashWriteFeature<TVoltageRange>::wordProgram(uint32_t flashAddress,uint32_t data) const {

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

  template<InternalFlashVoltageRange TVoltageRange>
  inline bool InternalFlashWriteFeature<TVoltageRange>::halfWordProgram(uint32_t flashAddress,uint16_t data) const {

    uint32_t err;

    if((err=FLASH_ProgramHalfWord(flashAddress,data))==FLASH_COMPLETE)
      return true;

    return errorProvider.set(ErrorProvider::ERROR_PROVIDER_INTERNAL_FLASH,E_PROGRAM_FAILED,err);
  }
}
