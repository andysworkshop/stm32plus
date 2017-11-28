/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * High-level class to manage the storage of program settings (configuration) in flash memory. Configuration data
   * is stored in the upper-most flash pages in your device. At least two pages are required for a system to be
   * resiliant in the face of unexpected power loss. Wear levelling is achieved by using up as much of the page as
   * possible before rolling over to the next page and eventually back to the first, and so on. There is an overhead
   * of 8 bytes on top of your settings class size (the template TSettings parameter) and the size is rounded up to
   * the nearest power of 2.
   *
   * For example, your settings class is 12 bytes long. The 8-byte overhead makes that 20 bytes. That is rounded up
   * to a storage size of 32 bytes. Your device has 1K pages and you have allocated 2 pages to the settings. To fill
   * up both pages requires 2048/32 = 64 writes before wrapping back to the first page. ST guarantee a minimum of 10K
   * erase/program cycles per page. Therefore you are safe to write settings 10000*64 = 640000 times before the flash
   * is in danger of wearing out.
   *
   * The size of your settings class must be a multiple of 4 bytes. This is automatically done for you by the compiler
   * unless you apply an attribute that prevents it. There is a static_assert in this class's constructor to catch
   * the case where a settings class is not a multiple of 4 bytes.
   *
   * The memory layout is:
   *   0     : 0xBE
   *   1-3   : version number
   *   4-n   : your settings structure
   *   a+1-b : random byte padding up to nearest power of 2 less one 32-bit word
   *   b-b+3 : CRC of all preceding data
   *
   * Your settings class must be a POD type (simple types only). We store it prefixed by a version number and
   * postfixed by its 32-bit CRC. The 24-bit version number is incremented each time the settings are written giving
   * you a maximum number of 16m versions.
   */

  template<class TSettings,class TFlash>
  class InternalFlashSettingsStorage {

    public:

      /**
       * Parameters class: must be set up by the user
       */

      struct Parameters {

        uint32_t firstLocation;   ///< The first location to use in flash. Must be on a page boundary.
        uint32_t memorySize;      ///< The amount of memory to use. Must be a multiple of the page sizes you want to use.

        /**
         * Constructor
         * @param location The first location in flash (must be a multiple of the page size)
         * @param size The total amount of flash memory that you want to use
         */

        Parameters(uint32_t location,uint32_t size)
          : firstLocation(location),
            memorySize(size) {
        }


        /**
         * Default constructor. Parameters must be set later.
         */

        Parameters() {
        }
      };


      /**
       * Error codes
       */

      enum {
        E_NOT_FOUND = 1     ///< No settings found in flash
      };

    protected:
      const TFlash& _flashPeripheral;
      const Parameters& _parameters;

      uint32_t _lastLocation;
      uint32_t _lastVersion;

    protected:
      bool findSettings();
      uint32_t getSettingsSize() const;

    public:
      InternalFlashSettingsStorage(const TFlash& flashPeripheral,const Parameters& params);

      bool read(TSettings& settings);
      bool write(const TSettings& settings);

      bool erase() const;
      void reset();
  };


  /**
   * Constructor
   */

  template<class TSettings,class TFlash>
  inline InternalFlashSettingsStorage<TSettings,TFlash>::InternalFlashSettingsStorage(const TFlash& flashPeripheral,const Parameters& params)
    : _flashPeripheral(flashPeripheral),
      _parameters(params) {

    static_assert(sizeof(TSettings) % 4==0,"Settings class must be multiple of 4 bytes. Please pad it out with some dummy fields to make it so");

    ClockControl<PERIPHERAL_CRC>::On();
    reset();
  }


  /**
   * Reset internal state
   */

  template<class TSettings,class TFlash>
  inline void InternalFlashSettingsStorage<TSettings,TFlash>::reset() {
    _lastLocation=0xFFFFFFFF;
    _lastVersion=0xFFFFFFFF;
  }


  /**
   * Read settings
   * @param settings The settings class
   * @return true if it worked
   */

  template<class TSettings,class TFlash>
  inline bool InternalFlashSettingsStorage<TSettings,TFlash>::read(TSettings& settings) {

    uint32_t i,*src,*dest;

    // if this is the first time then the settings need to be found

    if(_lastLocation==0xFFFFFFFF && !findSettings())
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_INTERNAL_FLASH_SETTINGS,E_NOT_FOUND);

    // copy out the settings. these need to come out as words to maintain the endian-ness of
    // the data that programmed word-by-word. In short, don't memcpy them out because it
    // won't work

    src=reinterpret_cast<uint32_t *>(_lastLocation)+1;
    dest=reinterpret_cast<uint32_t *>(&settings);

    for(i=0;i<sizeof(TSettings)/4;i++)
      *dest++=*src++;

    return true;
  }


  /**
   * Write settings
   * @param settings The settings class to write
   * @return true if it worked
   */

  template<class TSettings,class TFlash>
  inline bool InternalFlashSettingsStorage<TSettings,TFlash>::write(const TSettings& settings) {

    uint32_t newVersion,newLocation,settingsWords,i,r,randomWords;
    const uint32_t *srcPtr;

    // if this is the first time then the settings need to be found

    settingsWords=getSettingsSize()/4;

    if(_lastLocation==0xFFFFFFFF && !findSettings()) {

      // if this is the first time then try to find the last version

      _lastLocation=newLocation=_parameters.firstLocation;
      _lastVersion=0;     // invalid version, will be incremented in next instruction
    }
    else
      newLocation=_lastLocation+settingsWords*4;

    // get the new location and new version

    newVersion=_lastVersion+1;

    // if reached the end of all pages then wrap back to the start

    if(newLocation>=_parameters.firstLocation+_parameters.memorySize)
      newLocation=_parameters.firstLocation;

    // declare a lock manager to keep the flash write-enabled while we work on it

    InternalFlashLockFeature::LockManager lm;

    // is this new location the beginning of a page?

    if(_flashPeripheral.isStartOfPage(newLocation)) {

      // erase the page that we are about to occupy

      if(!_flashPeripheral.pageErase(newLocation))
        return false;
    }

    InternalFlashWordWriter<TFlash> writer(_flashPeripheral,newLocation);
    CRC_ResetDR();

    // write the version number

    *writer++=newVersion | 0xBE000000;
    CRC_CalcCRC(newVersion | 0xBE000000);

    srcPtr=reinterpret_cast<const uint32_t *>(&settings);

    // write the settings

    for(i=0;i<sizeof(TSettings)/4;i++) {

      // program the word

      *writer++=*srcPtr;

      // update CRC, src and dest

      CRC_CalcCRC(*srcPtr);
      srcPtr++;
    }

    // write random words from the end of the settings to where the CRC will be

    randomWords=settingsWords-(sizeof(TSettings)/4)-2;

    for(i=0;i<randomWords;i++) {

      // get a new random word

      r=rand();

      // program it

      *writer++=r;

      // update CRC and position

      CRC_CalcCRC(r);
    }

    // write the CRC into the last location - only after this is this settings block valid

    *writer=CRC_GetCRC();

    if(writer.hasError())
      return false;

    // update internal state

    _lastLocation=newLocation;
    _lastVersion=newVersion;

    return true;
  }


  /**
   * Search for the settings in flash
   * @return true if found, false if not
   */

  template<class TSettings,class TFlash>
  inline bool InternalFlashSettingsStorage<TSettings,TFlash>::findSettings() {

    uint32_t crc,maxVersion,thisVersion,settingsSize,i,entryCount;
    uint32_t *location,*maxLocation;

    maxVersion=0;
    maxLocation=nullptr;
    location=reinterpret_cast<uint32_t *>(_parameters.firstLocation);
    settingsSize=getSettingsSize();
    entryCount=_parameters.memorySize/settingsSize;
    settingsSize/=4;

    // all possible locations must be checked

    for(i=0;i<entryCount;i++) {

      // the top 8 bits have a magic number: BE

      thisVersion=*location;
      if((thisVersion & 0xFF000000)==0xBE000000) {

        if((thisVersion & 0x00FFFFFF)>maxVersion) {

          // possible match, run the CRC check

          CRC_ResetDR();
          crc=CRC_CalcBlockCRC(location,settingsSize-1);    // don't include the CRC itself

          if(crc==location[settingsSize-1]) {

            // CRC matches, we have a new high version

            maxVersion=thisVersion & 0x00FFFFFF;
            maxLocation=location;
          }
        }
      }

      location+=settingsSize;
    }

    if(maxVersion>0) {

      // found it

      _lastVersion=maxVersion;
      _lastLocation=reinterpret_cast<uint32_t>(maxLocation);

      return true;
    }

    // not found

    return false;
  }


  /**
   * Erase all pages defined as belonging to these settings
   * @return true if it worked
   */

  template<class TSettings,class TFlash>
  inline bool InternalFlashSettingsStorage<TSettings,TFlash>::erase() const {

    uint32_t address;

    // the flash device needs to be unlocked

    InternalFlashLockFeature::LockManager lm;

    for(address=_parameters.firstLocation;address<_parameters.firstLocation+_parameters.memorySize;address+=_flashPeripheral.getPageSize(address)) {

      // erase this page

      if(!_flashPeripheral.pageErase(address))
        return false;
    }

    return true;
  }


  /**
   * Bit twiddling hack to get the nearest power of 2 above the settings size
   * @return The settings size
   */

  template<class TSettings,class TFlash>
  inline uint32_t InternalFlashSettingsStorage<TSettings,TFlash>::getSettingsSize() const {

    uint32_t size;

    size=sizeof(TSettings)+8;

    size--;
    size |= size >> 1;
    size |= size >> 2;
    size |= size >> 4;
    size |= size >> 8;
    size |= size >> 16;
    size++;

    return size;
  }
}
