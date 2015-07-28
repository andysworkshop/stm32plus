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
   * For example, your settings class is 14 bytes long. The 8-byte overhead makes that 22 bytes. That is rounded up
   * to a storage size of 32 bytes. Your device has 1K pages and you have allocated 2 pages to the settings. To fill
   * up both pages requires 2048/32 = 64 writes before wrapping back to the first page. ST guarantee a minimum of 10K
   * erase/program cycles per page. Therefore you are safe to write settings 10000*64 = 640000 times before the flash
   * is in danger of wearing out.
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

        uint32_t firstLocation;
        uint32_t pageCount;

        /**
         * Constructor
         * @param location The first location in flash (must be a multiple of the page size)
         * @param count The number of pages to dedicate to settings storage
         */

        Parameters(uint32_t location,uint32_t count)
          : firstLocation(location),
            pageCount(count) {
        }
      };

      /**
       * Errors
       */

      enum {
        E_NOT_FOUND = 1     ///< No settings found in flash
      };

    protected:
      const TFlash& _flashPeripheral;
      const Parameters& _parameters;
      CrcLittleEndian _crc;

      uint32_t _lastLocation;
      uint32_t _lastVersion;

    protected:
      bool findSettings();
      uint32_t getSettingsSize() const;

    public:
      InternalFlashSettingsStorage(const TFlash& flashPeripheral,const Parameters& params);

      bool read(TSettings& settings);
      bool write(const TSettings& settings);
  };


  /**
   * Constructor
   */

  template<class TSettings,class TFlash>
  inline InternalFlashSettingsStorage<TSettings,TFlash>::InternalFlashSettingsStorage(const TFlash& flashPeripheral,const Parameters& params)
    : _flashPeripheral(flashPeripheral),
      _parameters(params),
      _crc(CrcLittleEndian::Parameters()) {

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

    // if this is the first time then the settings need to be found

    if(_lastLocation==0xFFFFFFFF && !findSettings())
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_INTERNAL_FLASH_SETTINGS,E_NOT_FOUND);

    // copy out the settings and return OK

    memcpy(&settings,_lastLocation+1,sizeof(TSettings));
    return true;
  }


  /**
   * Write settings
   * @param settings The settings class to write
   * @return true if it worked
   */

  template<class TSettings,class TFlash>
  inline bool InternalFlashSettingsStorage<TSettings,TFlash>::write(const TSettings& settings) {
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
    location=reinterpret_cast<uint32_t *>(_parameters.firstLocation);
    settingsSize=getSettingsSize();
    entryCount=(_flashPeripheral.getPageSize()*_parameters.pageCount)/settingsSize;
    settingsSize/=4;

    // all possible locations must be checked

    for(i=0;i<entryCount;i++) {

      // the top 8 bits have a magic number: BE

      thisVersion=*location;
      if(thisVersion & 0xFF000000==0xBE000000) {

        if(thisVersion & 0x00FFFFFF>maxVersion) {

          // possible match, run the CRC check

          _crc.reset();
          crc=_crc.calculateWordBuffer(location,settingsSize-1);    // don't include the CRC itself

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
      _lastLocation=maxLocation;

      return true;
    }

    // not found

    return false;
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
