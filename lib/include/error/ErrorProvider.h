/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
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
        ERROR_PROVIDER_SD_SDIO,
        ERROR_PROVIDER_FILESYSTEM,
        ERROR_PROVIDER_FAT_FILESYSTEM,
        ERROR_PROVIDER_FAT16_FILESYSTEM,
        ERROR_PROVIDER_FAT32_FILESYSTEM,
        ERROR_PROVIDER_FAT16_FILESYSTEM_FORMATTER,
        ERROR_PROVIDER_FAT32_FILESYSTEM_FORMATTER,
        ERROR_PROVIDER_FAT_DIRECTORY,
        ERROR_PROVIDER_ITERATOR,
        ERROR_PROVIDER_TEST,
        ERROR_PROVIDER_DIRECTORY_ITERATOR,
        ERROR_PROVIDER_ILI9481_LCD_INTERFACE,
        ERROR_PROVIDER_ILI9327_LCD_INTERFACE,
        ERROR_PROVIDER_ILI9325_LCD_INTERFACE,
        ERROR_PROVIDER_HX8347_LCD_INTERFACE,
        ERROR_PROVIDER_REMOVABLE_STORAGE_DEVICE,
        ERROR_PROVIDER_BLOCK_DEVICE,
        ERROR_PROVIDER_FILE,
        ERROR_PROVIDER_FREE_CLUSTER_FINDER,
        ERROR_PROVIDER_LONG_FILENAME_GENERATOR,
        ERROR_PROVIDER_USART,
        ERROR_PROVIDER_BUFFERED_IOSTREAM,
        ERROR_PROVIDER_SPI,
        ERROR_PROVIDER_I2S,
        ERROR_PROVIDER_DMA,
        ERROR_PROVIDER_BITMAP_CACHE,
        ERROR_PROVIDER_SPI_INPUT_STREAM,
        ERROR_PROVIDER_USART_INPUT_STREAM,
        ERROR_PROVIDER_CIRCULAR_BUFFER_INPUT_STREAM,
        ERROR_PROVIDER_AT45DB161D,
        ERROR_PROVIDER_SRAM,
        ERROR_PROVIDER_TOUCH_SCREEN,
        ERROR_PROVIDER_TOUCH_SCREEN_CALIBRATION,
        ERROR_PROVIDER_LZG_DECOMPRESSION_STREAM,
        ERROR_PROVIDER_INPUT_STREAM,
        ERROR_PROVIDER_OUTPUT_STREAM,
        ERROR_PROVIDER_BLOCK_DEVICE_OUTPUT_STREAM,
        ERROR_PROVIDER_BLOCK_DEVICE_INPUT_STREAM,
        ERROR_PROVIDER_I2C,
        ERROR_PROVIDER_HASH,
        ERROR_PROVIDER_WORK_QUEUE,
        ERROR_PROVIDER_DAC,
        ERROR_PROVIDER_RNG,
        ERROR_PROVIDER_SERIAL_EEPROM,
        ERROR_PROVIDER_STL_STRING_INPUT_STREAM,
        ERROR_PROVIDER_BYTE_ARRAY_INPUT_STREAM,
        ERROR_PROVIDER_READ_AHEAD_INPUT_STREAM,
        ERROR_PROVIDER_NET_MAC,
        ERROR_PROVIDER_NET_PHY,
        ERROR_PROVIDER_NET_ARP,
        ERROR_PROVIDER_NET_IP,
        ERROR_PROVIDER_NET_IP_PACKET_REASSEMBLER,
        ERROR_PROVIDER_NET_PING,
        ERROR_PROVIDER_NET_UDP,
        ERROR_PROVIDER_NET_TCP,
        ERROR_PROVIDER_NET_TCP_SERVER,
        ERROR_PROVIDER_NET_TCP_CONNECTION,
        ERROR_PROVIDER_NET_TCP_CONNECTION_ARRAY,
        ERROR_PROVIDER_NET_TCP_TEXT_LINE_RECEIVER,
        ERROR_PROVIDER_NET_TCP_INPUT_STREAM,
        ERROR_PROVIDER_NET_TCP_OUTPUT_STREAM,
        ERROR_PROVIDER_NET_DHCP,
        ERROR_PROVIDER_NET_LINK_LOCAL_IP,
        ERROR_PROVIDER_NET_DNS,
        ERROR_PROVIDER_NET_IPPORTS,
        ERROR_PROVIDER_NET_IP_DISABLED_PACKET_FRAGMENT_FEATURE,
        ERROR_PROVIDER_NET_IP_DISABLED_PACKET_REASSEMBLER_FEATURE,
        ERROR_PROVIDER_NET_HTTP_CLIENT,
        ERROR_PROVIDER_SPI_FLASH,
        ERROR_PROVIDER_SPI_FLASH_INPUT_STREAM
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
