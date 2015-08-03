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

  template<InternalFlashVoltageRange TVoltageRange,class TSectorMap>
  class InternalFlashWriteFeature {

    public:
      enum {
        E_ERASE_FAILED = 1,          ///< erase operation failed: see extended code for reason
        E_PROGRAM_FAILED = 2         ///< program operation failed: see extended code for reason
      };

    public:
      uint32_t getPageSize(uint32_t flashAddress) const;
      uint8_t getPageFromAddress(uint32_t flashAddress) const;

      bool chipErase() const;
      bool pageErase(uint32_t flashAddress) const;

      bool wordProgram(uint32_t flashAddress,uint32_t data) const;
      bool halfWordProgram(uint32_t flashAddress,uint16_t data) const;
  };


  /*
   * Some typedefs for common MCU configurations
   */

  typedef InternalFlashWriteFeature<InternalFlashVoltageRange::VR_2_7_TO_3_6,InternalFlashSectorMapF40xF41x> DefaultF407InternalFlashWriteFeature;


  /**
   * Erase the entire device - not clever if you're running from flash
   * @return true if it worked and the operation has completed.
   */

  template<InternalFlashVoltageRange TVoltageRange,class TSectorMap>
  inline bool InternalFlashWriteFeature<TVoltageRange,TSectorMap>::chipErase() const {

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

  template<InternalFlashVoltageRange TVoltageRange,class TSectorMap>
  inline bool InternalFlashWriteFeature<TVoltageRange,TSectorMap>::pageErase(uint32_t flashAddress) const {

    uint32_t err;

    if((err=FLASH_EraseSector(getPageFromAddress(flashAddress),static_cast<uint8_t>(TVoltageRange)))==FLASH_COMPLETE)
      return true;

    return errorProvider.set(ErrorProvider::ERROR_PROVIDER_INTERNAL_FLASH,E_ERASE_FAILED,err);
  }


  /**
   * Program a 32-bit word
   * @param flashAddress The address to program. Must be a 4-byte boundary.
   * @param data The 32-bit word to program
   * @return true if it worked and the operation has completed.
   */

  template<InternalFlashVoltageRange TVoltageRange,class TSectorMap>
  inline bool InternalFlashWriteFeature<TVoltageRange,TSectorMap>::wordProgram(uint32_t flashAddress,uint32_t data) const {

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

  template<InternalFlashVoltageRange TVoltageRange,class TSectorMap>
  inline bool InternalFlashWriteFeature<TVoltageRange,TSectorMap>::halfWordProgram(uint32_t flashAddress,uint16_t data) const {

    uint32_t err;

    if((err=FLASH_ProgramHalfWord(flashAddress,data))==FLASH_COMPLETE)
      return true;

    return errorProvider.set(ErrorProvider::ERROR_PROVIDER_INTERNAL_FLASH,E_PROGRAM_FAILED,err);
  }


  /**
   * Get the page number from the flash address
   * @param flashAddress
   * @return the zero-based page number
   */

  template<InternalFlashVoltageRange TVoltageRange,class TSectorMap>
  uint8_t InternalFlashWriteFeature<TVoltageRange,TSectorMap>::getPageFromAddress(uint32_t flashAddress) const {

    uint8_t pageNumber;
    uint32_t address,pageSize;

    address=FLASH_BASE;
    pageNumber=0;

    for(;;) {

      pageSize=getPageSize(flashAddress);

      if(flashAddress<address+pageSize)
        return pageNumber;

      pageNumber++;
      address+=pageSize;
    }
  }


  /**
   * Get the page size of this device
   * @return The page size
   */

  template<InternalFlashVoltageRange TVoltageRange,class TSectorMap>
  inline uint32_t InternalFlashWriteFeature<TVoltageRange,TSectorMap>::getPageSize(uint32_t flashAddress) const {
    return TSectorMap::sectorMap[getPageFromAddress(flashAddress)]*1024;
  }
}
