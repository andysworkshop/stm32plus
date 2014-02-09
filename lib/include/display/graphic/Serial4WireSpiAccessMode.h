/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include "gpio/Gpio.h"
#include "display/graphic/DmaFsmcLcdMemoryCopy.h"
#include "spi/SpiWriter.h"


namespace stm32plus {
  namespace display {

    /**
     * @brief 4-wire SPI access to a panel - a dedicated GPIO line is used to do the register select.
     *
     * 4-wire mode operates in 8-bit transfers which makes it
     * compatible with the hardware SPI on the STM32, which is why we will use it here. The
     * alternative 3-wire SPI interface requires a 9th bit per transfer which makes it
     * incompatible with the hardware in the STM32.
     */

    class Serial4WireSpiAccessMode {

      protected:
        Gpio& _dataSelect;
        Gpio& _resetPin;
        SpiWriter& _writer;

      public:
        Serial4WireSpiAccessMode(
            SpiWriter& writer,
            Gpio& resetPin,
            Gpio& dataSelect
          );

        // overrides from SSD1306DisplayDevice

        bool writeCommand(uint8_t command);
        bool writeData(uint8_t *data,uint16_t dataSize);
        bool writeData(uint8_t data);

        void reset();
    };


    /**
     * Constructor
     * @param writer The SPI writer ckass
     * @param resetPin The pin for resetting the device
     * @param dataSelect The data-select line
     */

    inline Serial4WireSpiAccessMode::Serial4WireSpiAccessMode(SpiWriter& writer,Gpio& resetPin,Gpio& dataSelect)
      : _dataSelect(dataSelect),
        _resetPin(resetPin),
        _writer(writer) {
    }


    /**
     * Reset the device
     */

    inline void Serial4WireSpiAccessMode::reset() {
      _resetPin.set();
      MillisecondTimer::delay(5);
      _resetPin.reset();
      MillisecondTimer::delay(50);
      _resetPin.set();
      MillisecondTimer::delay(50);
    }


    /**
     * Send a command over the data line
     * @param command
     * @return false if it failed
     */

    inline bool Serial4WireSpiAccessMode::writeCommand(uint8_t command) {

      bool retval;

      _dataSelect.reset();

      _writer.getPeripheral().setNss(false);
      retval=_writer.write(&command,1);
      _writer.getPeripheral().setNss(true);

      return retval;
    }


    /**
     * Write data to the device over the bus
     * @param data
     * @param dataSize Number of bytes to send
     * @return false if it failed
     */

    inline bool Serial4WireSpiAccessMode::writeData(uint8_t *data,uint16_t dataSize) {

      bool retval;

      _dataSelect.set();
      _writer.getPeripheral().setNss(false);

      retval=_writer.write(data,dataSize);

      _writer.getPeripheral().setNss(true);
      return retval;
    }


    /**
     * Write a single byte
     * @param data
     * @return false if it failed
     */

    inline bool Serial4WireSpiAccessMode::writeData(uint8_t data) {
      return writeData(&data,1);
    }
  }
}
