/*
 * Support for the Tianma TM032LDH05 display panel
 *
 *  Created on: Apr 22, 2015
 *      Author: Kyle D. Williams
 *
 *  Accepted with thanks into the main stm32plus library source code.
 */

#pragma once


/*
 * Useful typedefs to ease the pain of typing out template parameters
 */

namespace stm32plus {
  namespace display {

    /**
     * Panel traits for the TM032LDH05 display. The panel is 400x240 pixels in size.
     */

    class TM032LDH05 {

      public:
        enum {
          LONG_SIDE = 400,
          SHORT_SIDE = 240
        };

      public:
        template<class TAccessMode>
        static void initialise(TAccessMode& accessMode);

        template<class TAccessMode>
        static void wake(TAccessMode& accessMode);
    };


    /*
     * Post reset initialisation for the panel
     */

    template<class TAccessMode>
    inline void TM032LDH05::initialise(TAccessMode& accessMode) {

      // do the startup sequence

      MillisecondTimer::delay(160);

      accessMode.writeCommand(hx8352a::TEST_MODE_CONTROL,0x02);       // TESTM=1

      accessMode.writeCommand(hx8352a::VDDD_CONTROL,0x03);            // VDC_SEL=011
      accessMode.writeCommand(hx8352a::SOURCE_GAMMA_RESISTOR_1,0x01);
      accessMode.writeCommand(hx8352a::SOURCE_GAMMA_RESISTOR_2,0x93); // STBA[7]=1,STBA[5:4]=01,STBA[1:0]=11

      accessMode.writeCommand(hx8352a::SYNC_FUNCTION,0x01);           // DCDC_SYNC=1

      accessMode.writeCommand(hx8352a::TEST_MODE_CONTROL,0x00);       // TESTM=0

      // Gamma Setting

      accessMode.writeCommand(hx8352a::GAMMA_1+0, 0xB0);
      accessMode.writeCommand(hx8352a::GAMMA_1+1, 0x03);
      accessMode.writeCommand(hx8352a::GAMMA_1+2, 0x10);
      accessMode.writeCommand(hx8352a::GAMMA_1+3, 0x56);
      accessMode.writeCommand(hx8352a::GAMMA_1+4, 0x13);
      accessMode.writeCommand(hx8352a::GAMMA_1+5, 0x46);
      accessMode.writeCommand(hx8352a::GAMMA_1+6, 0x23);
      accessMode.writeCommand(hx8352a::GAMMA_1+7, 0x76);
      accessMode.writeCommand(hx8352a::GAMMA_1+8, 0x00);
      accessMode.writeCommand(hx8352a::GAMMA_1+9, 0x5E);
      accessMode.writeCommand(hx8352a::GAMMA_1+10,0x4F);
      accessMode.writeCommand(hx8352a::GAMMA_1+11,0x40);

      // Power On sequence

      accessMode.writeCommand(hx8352a::OSC_CONTROL_1,0x91);

      accessMode.writeCommand(hx8352a::CYCLE_CONTROL_1,0xF9);
      MillisecondTimer::delay(10);

      accessMode.writeCommand(hx8352a::POWER_CONTROL_3,0x14);

      accessMode.writeCommand(hx8352a::POWER_CONTROL_2,0x11);

      accessMode.writeCommand(hx8352a::POWER_CONTROL_4,0x06);

      accessMode.writeCommand(hx8352a::VCOM_CONTROL,0x42);
      MillisecondTimer::delay(20);

      accessMode.writeCommand(hx8352a::POWER_CONTROL_1 ,0x0A);

      accessMode.writeCommand(hx8352a::POWER_CONTROL_1 ,0x1A);
      MillisecondTimer::delay(40);


      accessMode.writeCommand(hx8352a::POWER_CONTROL_1 ,0x12);
      MillisecondTimer::delay(40);

      accessMode.writeCommand(hx8352a::POWER_CONTROL_6 ,0x27);
      MillisecondTimer::delay(100);


      // Display on setting

      accessMode.writeCommand(hx8352a::DISPLAY_CONTROL_2,0x60);

      accessMode.writeCommand(hx8352a::SOURCE_CONTROL_2,0x40);

      accessMode.writeCommand(hx8352a::CYCLE_CONTROL_10,0x38);

      accessMode.writeCommand(hx8352a::CYCLE_CONTROL_11,0x38);

      accessMode.writeCommand(hx8352a::DISPLAY_CONTROL_2,0x38);
      MillisecondTimer::delay(40);

      accessMode.writeCommand(hx8352a::DISPLAY_CONTROL_2,0x3C);

      accessMode.writeCommand(hx8352a::MEMORY_ACCESS_CONTROL,0x1C);

      accessMode.writeCommand(hx8352a::DISPLAY_MODE_CONTROL,0x06);

      accessMode.writeCommand(hx8352a::PANEL_CONTROL,0x00);

      accessMode.writeCommand(hx8352a::COLUMN_ADDRESS_START_H,0x00);
      accessMode.writeCommand(hx8352a::COLUMN_ADDRESS_START_L,0x00);
      accessMode.writeCommand(hx8352a::COLUMN_ADDRESS_END_H,0x00);
      accessMode.writeCommand(hx8352a::COLUMN_ADDRESS_END_L,0xef);

      accessMode.writeCommand(hx8352a::ROW_ADDRESS_START_H,0x00);
      accessMode.writeCommand(hx8352a::ROW_ADDRESS_START_L,0x00);
      accessMode.writeCommand(hx8352a::ROW_ADDRESS_END_H,0x01);
      accessMode.writeCommand(hx8352a::ROW_ADDRESS_END_L,0x8f);
    }


    /**
     * Wake the panel up from standby mode. Only a subset of the initialisation sequence is actually
     * required but we'll save some code and just call initialise() again to bring it back to life.
     * @param accessMode The access mode
     */

    template<class TAccessMode>
    inline void TM032LDH05::wake(TAccessMode& accessMode) {
      initialise(accessMode);
    }
  }
}
