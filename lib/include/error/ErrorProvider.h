/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include <stdint.h>

namespace stm32plus {

  /**
   * @brief structured error handler.
   *
   * The error handler is a global singleton, much as I dislike them. An error code
   * is a 32 bit value. Bits 00-15 are a provider specific error code. Bits 16-23
   * are the provider identifier.
   */

  class ErrorProvider {
    protected:
      uint32_t _errorCode;      // includes provider code
      uint32_t _errorCause;
      uint32_t _provider;

    public:

      /**
       * Error codes
       */

      enum {
        /// There was no error
        ERROR_NO_ERROR=0
      };

      enum {
        ERROR_PROVIDER_SD_SDIO                                    = 0,
        ERROR_PROVIDER_FILESYSTEM                                 = 1,
        ERROR_PROVIDER_FAT_FILESYSTEM                             = 2,
        ERROR_PROVIDER_FAT16_FILESYSTEM                           = 3,
        ERROR_PROVIDER_FAT32_FILESYSTEM                           = 4,
        ERROR_PROVIDER_FAT16_FILESYSTEM_FORMATTER                 = 5,
        ERROR_PROVIDER_FAT32_FILESYSTEM_FORMATTER                 = 6,
        ERROR_PROVIDER_FAT_DIRECTORY                              = 7,
        ERROR_PROVIDER_ITERATOR                                   = 8,
        ERROR_PROVIDER_TEST                                       = 9,
        ERROR_PROVIDER_DIRECTORY_ITERATOR                         = 10,
        ERROR_PROVIDER_ILI9481_LCD_INTERFACE                      = 11,
        ERROR_PROVIDER_ILI9327_LCD_INTERFACE                      = 12,
        ERROR_PROVIDER_ILI9325_LCD_INTERFACE                      = 13,
        ERROR_PROVIDER_HX8347_LCD_INTERFACE                       = 14,
        ERROR_PROVIDER_REMOVABLE_STORAGE_DEVICE                   = 15,
        ERROR_PROVIDER_BLOCK_DEVICE                               = 16,
        ERROR_PROVIDER_FILE                                       = 17,
        ERROR_PROVIDER_FREE_CLUSTER_FINDER                        = 18,
        ERROR_PROVIDER_LONG_FILENAME_GENERATOR                    = 19,
        ERROR_PROVIDER_USART                                      = 20,
        ERROR_PROVIDER_BUFFERED_IOSTREAM                          = 21,
        ERROR_PROVIDER_SPI                                        = 22,
        ERROR_PROVIDER_I2S                                        = 23,
        ERROR_PROVIDER_DMA                                        = 24,
        ERROR_PROVIDER_BITMAP_CACHE                               = 25,
        ERROR_PROVIDER_SPI_INPUT_STREAM                           = 26,
        ERROR_PROVIDER_USART_INPUT_STREAM                         = 27,
        ERROR_PROVIDER_CIRCULAR_BUFFER_INPUT_STREAM               = 28,
        ERROR_PROVIDER_AT45DB161D                                 = 29,
        ERROR_PROVIDER_SRAM                                       = 30,
        ERROR_PROVIDER_TOUCH_SCREEN                               = 31,
        ERROR_PROVIDER_TOUCH_SCREEN_CALIBRATION                   = 32,
        ERROR_PROVIDER_LZG_DECOMPRESSION_STREAM                   = 33,
        ERROR_PROVIDER_INPUT_STREAM                               = 34,
        ERROR_PROVIDER_OUTPUT_STREAM                              = 35,
        ERROR_PROVIDER_BLOCK_DEVICE_OUTPUT_STREAM                 = 36,
        ERROR_PROVIDER_BLOCK_DEVICE_INPUT_STREAM                  = 37,
        ERROR_PROVIDER_I2C                                        = 38,
        ERROR_PROVIDER_HASH                                       = 39,
        ERROR_PROVIDER_WORK_QUEUE                                 = 40,
        ERROR_PROVIDER_DAC                                        = 41,
        ERROR_PROVIDER_RNG                                        = 42,
        ERROR_PROVIDER_SERIAL_EEPROM                              = 43,
        ERROR_PROVIDER_STL_STRING_INPUT_STREAM                    = 44,
        ERROR_PROVIDER_BYTE_ARRAY_INPUT_STREAM                    = 45,
        ERROR_PROVIDER_READ_AHEAD_INPUT_STREAM                    = 46,
        ERROR_PROVIDER_NET_MAC                                    = 47,
        ERROR_PROVIDER_NET_PHY                                    = 48,
        ERROR_PROVIDER_NET_ARP                                    = 49,
        ERROR_PROVIDER_NET_IP                                     = 50,
        ERROR_PROVIDER_NET_IP_PACKET_REASSEMBLER                  = 51,
        ERROR_PROVIDER_NET_PING                                   = 52,
        ERROR_PROVIDER_NET_UDP                                    = 53,
        ERROR_PROVIDER_NET_TCP                                    = 54,
        ERROR_PROVIDER_NET_TCP_SERVER                             = 55,
        ERROR_PROVIDER_NET_TCP_CONNECTION                         = 56,
        ERROR_PROVIDER_NET_TCP_CONNECTION_ARRAY                   = 57,
        ERROR_PROVIDER_NET_TCP_TEXT_LINE_RECEIVER                 = 58,
        ERROR_PROVIDER_NET_TCP_INPUT_STREAM                       = 59,
        ERROR_PROVIDER_NET_TCP_OUTPUT_STREAM                      = 60,
        ERROR_PROVIDER_NET_DHCP                                   = 61,
        ERROR_PROVIDER_NET_LINK_LOCAL_IP                          = 62,
        ERROR_PROVIDER_NET_DNS                                    = 63,
        ERROR_PROVIDER_NET_IPPORTS                                = 64,
        ERROR_PROVIDER_NET_IP_DISABLED_PACKET_FRAGMENT_FEATURE    = 65,
        ERROR_PROVIDER_NET_IP_DISABLED_PACKET_REASSEMBLER_FEATURE = 66,
        ERROR_PROVIDER_NET_HTTP_CLIENT                            = 67,
        ERROR_PROVIDER_SPI_FLASH                                  = 68,
        ERROR_PROVIDER_SPI_FLASH_INPUT_STREAM                     = 69,
        ERROR_PROVIDER_USB_DEVICE                                 = 70,
        ERROR_PROVIDER_USB_IN_ENDPOINT                            = 71,
        ERROR_PROVIDER_INTERNAL_FLASH                             = 72,
        ERROR_PROVIDER_INTERNAL_FLASH_SETTINGS                    = 73,
        ERROR_PROVIDER_CAN                                        = 74
      };

    public:

      /**
       * Get the last error.
       * @return The last error to occur or ERROR_NO_ERROR if successful.
       */

      uint32_t getLast() const {
        return _errorCode;
      }


      /**
       * Check if we have an error
       * @return true if we have an error
       */

      bool hasError() const {
        return _errorCode!=ERROR_NO_ERROR;
      }


      /**
       * @return just the error code part (no provider)
       */

      uint16_t getCode() const {
        return _errorCode & 0xFFFF;
      }

      /**
       * Compare the parameters to the last error.
       * @param[in] provider The 16-bit provider code.
       * @param[in] error The 16-bit error code.
       * @return True if the parameters match the last error stored here.
       */

      bool isLastError(uint16_t provider,uint16_t error) const {
        return _errorCode == ((static_cast<uint32_t>(provider) << 16) | error);
      }

      /**
       * Get the error cause, a 32-bit provider-specific value that may be
       * associated with the error that occurred.
       *
       * @return The error cause.
       */

      uint32_t getCause() const {
        return _errorCause;
      }

      /**
       * Get the provider code.
       * @return The provider code.
       */

      uint32_t getProvider() const {
        return _provider;
      }

      /**
       * Clear the status. Resets the last error to zero.
       */

      bool clear() {
        _errorCode=0;
        _errorCause=0;
        return true;
      }

      /**
       * Set the last error to the given values
       * @param[in] errorProviderCode The provider code from the enumeration in this class.
       * @param[in] errorCode The error code from the set defined by the provider.
       * @param[in] errorCause Optional 32-bit value associated with this error.
       * @return false, which is the convention for the return value of a method that wants to indicate to its caller that it has failed.
       */

      bool set(uint32_t errorProviderCode,uint32_t errorCode,uint32_t errorCause=0) {
        _provider=errorProviderCode;
        _errorCode=errorCode | (errorProviderCode << 16);
        _errorCause=errorCause;
        return false;
      }
  };

  /**
   * This is the error provider singleton that is visble throughout the library and externally to users.
   */

  extern ErrorProvider errorProvider;
}
