/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/usart.h"
#include "config/crc.h"
#include "config/string.h"


using namespace stm32plus;


/**
 * CRC demo. This demo will show how to calculate the
 *
 * 32-bit CRC of a stream of bytes using the hardware
 * CRC peripheral on the STM32. The calculated CRC
 * will be output to USART1. USART1 is configured as
 * 57600-8-N-1. The demo CRC value is 4200624485.
 *
 * Usart1 (non-remapped) is not available on the
 * STM32F4DISCOVERY. If this is your target board then
 * choose a remapped pin set or move to another usart,
 * e.g. Usart2.
 *
 * The STM32 CRC peripheral operates on 32-bit words
 * with considerations for the endian-ness of the data
 * left to the user. stm32plus provides implementations
 * for little and big endian calculations with
 * customisable padding bytes for when your data stream
 * isn't a multiple of 32-bits long.
 *
 * Big endian mode:
 *   The words are bit-reversed before going to the CRC
 *   unit and the result is bit-reversed before
 *   returning to you.
 *
 * Little endian mode:
 *   No transformations at all are performed on the data.
 *
 * Compatible MCU:
 *   STM32F0
 *   STM32F1
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F051R8T6
 *   STM32F100RBT6
 *   STM32F103ZET6
 *   STM32F407VGT6
 */

class CrcTest {

  /**
   * The selected CRC mode. We'll use big-endian. Input data is bit-reversed before going in to the
   * calculation and results are bit-reversed before coming back.
   */

  typedef CrcBigEndian MyCrcMode;

  public:

    void run() {

      static const char *testString="Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua";

      /*
       * Declare USART1 to run at 57600 and attach an output stream to it
       */

      Usart1<> usart(57600);
      UsartPollingOutputStream usartOutputStream(usart);

      /*
       * Declare a big-endian CRC implementation with a default padding value
       * of zero and attach it to an output stream
       */

      MyCrcMode::Parameters p;
      MyCrcMode crc(p);

      CrcOutputStream<MyCrcMode> crcOutputStream(crc);

      /*
       * Calculate the CRC of the test string
       */

      crcOutputStream << testString;      // stream in the data to the CRC unit
      crcOutputStream.close();            // close the stream (calls finish() on the CRC)

      /*
       * Write out the CRC value to the USART
       */

      char buf[15];
      StringUtil::modp_uitoa10(crc.currentCrc(),buf);

      usartOutputStream << "CRC is " << buf;

      /*
       * Done
       */

      for(;;);
    }
};


/*
 * Main entry point
 */

int main() {

  CrcTest test;
  test.run();

  // not reached
  return 0;
}
