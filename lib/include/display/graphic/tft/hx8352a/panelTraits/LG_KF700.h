/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


/*
 * Useful typedefs to ease the pain of typing out template parameters
 */

namespace stm32plus {
  namespace display {


    /**
     * Panel traits for the LG KF700 display. The panel is 480x240 pixels in size.
     */

    class LG_KF700 {

      public:
        enum {
          LONG_SIDE = 480,
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
    inline void LG_KF700::initialise(TAccessMode& accessMode) {

      // do the startup sequence

      MillisecondTimer::delay(160);
      accessMode.writeCommand(hx8352a::TEST_MODE_CONTROL,0x02);         // TESTM=1
      accessMode.writeCommand(hx8352a::VDDD_CONTROL,0x02);              // VDC_SEL=010.
      accessMode.writeCommand(hx8352a::SOURCE_GAMMA_RESISTOR_1,0x00);   // STBA[15:8]=0x00
      accessMode.writeCommand(hx8352a::SOURCE_GAMMA_RESISTOR_2,0xB3);   // STBA[7]=1, STBA[5:4]=11, STBA[1:0]=11
      accessMode.writeCommand(hx8352a::TEST_MODE_CONTROL,0x00);         // TESTM=0

      // Power Supply Setting

      accessMode.writeCommand(hx8352a::OSC_CONTROL_1,0x91);           // RADJ=0, OSC_EN=1 (100%)
      accessMode.writeCommand(hx8352a::DISPLAY_CONTROL_1,0x01);       // TE ON
      accessMode.writeCommand(hx8352a::CYCLE_CONTROL_1,0x14);         // N_DCDC=0x14

      MillisecondTimer::delay(20);

      accessMode.writeCommand(hx8352a::POWER_CONTROL_3,0x13);        // BT=0001, AP=100
      accessMode.writeCommand(hx8352a::POWER_CONTROL_2,0x11);        // VC3=001, VC1=001 (VLCD/DDVDH)=6.45V)
      accessMode.writeCommand(hx8352a::POWER_CONTROL_4,0x0);         // gamma x2.8
      accessMode.writeCommand(hx8352a::POWER_CONTROL_5,0x08);        // VBGP=1111
      accessMode.writeCommand(hx8352a::VCOM_CONTROL,0x3B);           // VCM=011_1011

      MillisecondTimer::delay(30);

      accessMode.writeCommand(hx8352a::POWER_CONTROL_1,0x0A);        // GASENB=0, PON=0, DK=1, XDK=0, VLCD_TRI=1, STB=0
      accessMode.writeCommand(hx8352a::POWER_CONTROL_1,0x1A);        // GASENB=0, PON=1, DK=1, XDK=0, VLCD_TRI=1, STB=0

      MillisecondTimer::delay(50);

      accessMode.writeCommand(hx8352a::POWER_CONTROL_1,0x12);       // GASENB=0, PON=1, DK=0, XDK=0, VLCD_TRI=1, STB=0,

      // VLCD=2XVCI by 2 CAPs

      MillisecondTimer::delay(50);
      accessMode.writeCommand(hx8352a::POWER_CONTROL_6,0x2E);       // VCOMG=1, VDV=0_1110

      //VCOMG NEW LOCATION
      accessMode.writeCommand(hx8352a::TEST_MODE_CONTROL,0x02);     // TESTM=1
      accessMode.writeCommand(0x93,0x10);                           // R93[4]=1, VCOMG=1 ...undocumented!!!
      accessMode.writeCommand(hx8352a::TEST_MODE_CONTROL,0x00);

      // DGC Function disabled
      accessMode.writeCommand(0x5A,0x00);

      // Display ON Setting
      accessMode.writeCommand(hx8352a::SOURCE_CONTROL_1,0xFF);      // N_SAP=0111 1111
      accessMode.writeCommand(hx8352a::SOURCE_CONTROL_2,0x0E);      // I_SAP=0000 1110
      accessMode.writeCommand(hx8352a::CYCLE_CONTROL_10,0x38);      // EQS=1000 0111
      accessMode.writeCommand(hx8352a::CYCLE_CONTROL_11,0x38);      // EQP=0011 1000
      accessMode.writeCommand(hx8352a::DISPLAY_CONTROL_2,0x38);     // GON=1, DTE=1, D=10

      MillisecondTimer::delay(50);
      accessMode.writeCommand(hx8352a::DISPLAY_CONTROL_2,0x3C);         // GON=1, DTE=1, D=11
      accessMode.writeCommand(hx8352a::DISPLAY_MODE_CONTROL,0x02);      // INVON=0, NORNO=1
    }


    /**
     * Wake the panel up from standby mode. Only a subset of the initialisation sequence is actually
     * required but we'll save some code and just call initialise() again to bring it back to life.
     * @param accessMode The access mode
     */

    template<class TAccessMode>
    inline void LG_KF700::wake(TAccessMode& accessMode) {
      initialise(accessMode);
    }
  }
}
