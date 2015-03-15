/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F0
#error This class can only be used with the STM32F0 series
#endif


namespace stm32plus {

  /**
   * @brief Template implementation for the EXTI lines
   * @tparam Index The index (0..15) of the EXTI line.
   * @tparam Line The EXTI line e.g. EXTI_Line0
   * @tparam Irq The corresponding IRQ for the EXTI line.
   *
   * Exti 0..15 configuration differs only slightly so we define as a template with integral
   * parameters and then declare typedefs for the user usage.
   *
   * The ExtiInterruptEventSender member can be used to subscribe to the
   * interrupts. See the 'exti' example for details of how to do this.
   */

  template<uint32_t Line>
  class ExtiPeripheral : public ExtiPeripheralBase {

    public:
      static ExtiPeripheral *_extiInstance;

    public:
      ExtiPeripheral(EXTIMode_TypeDef mode,EXTITrigger_TypeDef trigger);

      void generateSoftwareInterrupt() const;
      bool isSet() const;
  };


  /**
   * Constructor
   *
   * @param[in] mode EXTI_Mode_Event or EXTI_Mode_Interrupt
   * @param[in] trigger EXTI_Trigger_Falling / EXTI_Trigger_Rising / EXTI_Trigger_Rising_Falling
   * @param[in] pin The GPIO pin to use.
   */

  template<uint32_t Line>
  inline ExtiPeripheral<Line>::ExtiPeripheral(EXTIMode_TypeDef mode,EXTITrigger_TypeDef trigger) {
    _extiInstance=this;
    initialisePeripheral(Line,mode,trigger);
  }


  /**
   * Generate a software interrupt on this line
   */

  template<uint32_t Line>
  inline void ExtiPeripheral<Line>::generateSoftwareInterrupt() const {
    EXTI_GenerateSWInterrupt(Line);
  }

  /**
   * Test if this line is set or not
   * @return true if it is set
   */

  template<uint32_t Line>
  inline bool ExtiPeripheral<Line>::isSet() const {
    return EXTI_GetFlagStatus(Line)==SET;
  }


  /**
   * Specialisations for all the exti lines
   */

  struct Exti0 : public ExtiPeripheral<EXTI_Line0> {
    Exti0(EXTIMode_TypeDef mode,EXTITrigger_TypeDef trigger,const Gpio& pin)
      : ExtiPeripheral<EXTI_Line0>(mode,trigger) {

      pin.enableExti();

      if(mode==EXTI_Mode_Interrupt)
        ExtiInterruptEnabler<0>::enable();
    }
  };

  struct Exti1 : public ExtiPeripheral<EXTI_Line1> {
    Exti1(EXTIMode_TypeDef mode,EXTITrigger_TypeDef trigger,const Gpio& pin)
      : ExtiPeripheral<EXTI_Line1>(mode,trigger) {

      pin.enableExti();

      if(mode==EXTI_Mode_Interrupt)
        ExtiInterruptEnabler<1>::enable();
    }
  };

  struct Exti2 : public ExtiPeripheral<EXTI_Line2> {
    Exti2(EXTIMode_TypeDef mode,EXTITrigger_TypeDef trigger,const Gpio& pin)
      : ExtiPeripheral<EXTI_Line2>(mode,trigger) {

      pin.enableExti();

      if(mode==EXTI_Mode_Interrupt)
        ExtiInterruptEnabler<2>::enable();
    }
  };

  struct Exti3 : public ExtiPeripheral<EXTI_Line3> {
    Exti3(EXTIMode_TypeDef mode,EXTITrigger_TypeDef trigger,const Gpio& pin)
      : ExtiPeripheral<EXTI_Line3>(mode,trigger) {

      pin.enableExti();

      if(mode==EXTI_Mode_Interrupt)
        ExtiInterruptEnabler<3>::enable();
    }
  };

  struct Exti4 : public ExtiPeripheral<EXTI_Line4> {
    Exti4(EXTIMode_TypeDef mode,EXTITrigger_TypeDef trigger,const Gpio& pin)
      : ExtiPeripheral<EXTI_Line4>(mode,trigger) {

      pin.enableExti();

      if(mode==EXTI_Mode_Interrupt)
        ExtiInterruptEnabler<4>::enable();
    }
  };

  struct Exti5 : public ExtiPeripheral<EXTI_Line5> {
    Exti5(EXTIMode_TypeDef mode,EXTITrigger_TypeDef trigger,const Gpio& pin)
      : ExtiPeripheral<EXTI_Line5>(mode,trigger) {

      pin.enableExti();

      if(mode==EXTI_Mode_Interrupt)
        ExtiInterruptEnabler<5>::enable();
    }
  };

  struct Exti6 : public ExtiPeripheral<EXTI_Line6> {
    Exti6(EXTIMode_TypeDef mode,EXTITrigger_TypeDef trigger,const Gpio& pin)
      : ExtiPeripheral<EXTI_Line6>(mode,trigger) {

      pin.enableExti();

      if(mode==EXTI_Mode_Interrupt)
        ExtiInterruptEnabler<6>::enable();
    }
  };

  struct Exti7 : public ExtiPeripheral<EXTI_Line7> {
    Exti7(EXTIMode_TypeDef mode,EXTITrigger_TypeDef trigger,const Gpio& pin)
      : ExtiPeripheral<EXTI_Line7>(mode,trigger) {

      pin.enableExti();

      if(mode==EXTI_Mode_Interrupt)
        ExtiInterruptEnabler<7>::enable();
    }
  };

  struct Exti8 : public ExtiPeripheral<EXTI_Line8> {
    Exti8(EXTIMode_TypeDef mode,EXTITrigger_TypeDef trigger,const Gpio& pin)
      : ExtiPeripheral<EXTI_Line8>(mode,trigger) {

      pin.enableExti();

      if(mode==EXTI_Mode_Interrupt)
        ExtiInterruptEnabler<8>::enable();
    }
  };

  struct Exti9 : public ExtiPeripheral<EXTI_Line9> {
    Exti9(EXTIMode_TypeDef mode,EXTITrigger_TypeDef trigger,const Gpio& pin)
      : ExtiPeripheral<EXTI_Line9>(mode,trigger) {

      pin.enableExti();

      if(mode==EXTI_Mode_Interrupt)
        ExtiInterruptEnabler<9>::enable();
    }
  };

  struct Exti10 : public ExtiPeripheral<EXTI_Line10> {
    Exti10(EXTIMode_TypeDef mode,EXTITrigger_TypeDef trigger,const Gpio& pin)
      : ExtiPeripheral<EXTI_Line10>(mode,trigger) {

      pin.enableExti();

      if(mode==EXTI_Mode_Interrupt)
        ExtiInterruptEnabler<10>::enable();     // shared interrupt 10..15
    }
  };

  struct Exti11 : public ExtiPeripheral<EXTI_Line11> {
    Exti11(EXTIMode_TypeDef mode,EXTITrigger_TypeDef trigger,const Gpio& pin)
      : ExtiPeripheral<EXTI_Line11>(mode,trigger) {

      pin.enableExti();

      if(mode==EXTI_Mode_Interrupt)
        ExtiInterruptEnabler<11>::enable();     // shared interrupt 10..15
    }
  };

  struct Exti12 : public ExtiPeripheral<EXTI_Line12> {
    Exti12(EXTIMode_TypeDef mode,EXTITrigger_TypeDef trigger,const Gpio& pin)
      : ExtiPeripheral<EXTI_Line12>(mode,trigger) {

      pin.enableExti();

      if(mode==EXTI_Mode_Interrupt)
        ExtiInterruptEnabler<12>::enable();     // shared interrupt 10..15
    }
  };

  struct Exti13 : public ExtiPeripheral<EXTI_Line13> {
    Exti13(EXTIMode_TypeDef mode,EXTITrigger_TypeDef trigger,const Gpio& pin)
      : ExtiPeripheral<EXTI_Line13>(mode,trigger) {

      pin.enableExti();

      if(mode==EXTI_Mode_Interrupt)
        ExtiInterruptEnabler<13>::enable();     // shared interrupt 10..15
    }
  };

  struct Exti14 : public ExtiPeripheral<EXTI_Line14> {
    Exti14(EXTIMode_TypeDef mode,EXTITrigger_TypeDef trigger,const Gpio& pin)
      : ExtiPeripheral<EXTI_Line14>(mode,trigger) {

      pin.enableExti();

      if(mode==EXTI_Mode_Interrupt)
        ExtiInterruptEnabler<14>::enable();     // shared interrupt 10..15
    }
  };

  struct Exti15 : public ExtiPeripheral<EXTI_Line15> {
    Exti15(EXTIMode_TypeDef mode,EXTITrigger_TypeDef trigger,const Gpio& pin)
      : ExtiPeripheral<EXTI_Line15>(mode,trigger) {

      pin.enableExti();

      if(mode==EXTI_Mode_Interrupt)
        ExtiInterruptEnabler<15>::enable();     // shared interrupt 10..15
    }
  };

  struct ExtiPvd : public ExtiPeripheral<EXTI_Line16> {
    ExtiPvd(EXTIMode_TypeDef mode,EXTITrigger_TypeDef trigger)
      : ExtiPeripheral<EXTI_Line16>(mode,trigger) {

      if(mode==EXTI_Mode_Interrupt)
        ExtiInterruptEnabler<16>::enable();
    }
  };

  struct ExtiRtcAlarm : public ExtiPeripheral<EXTI_Line17> {
    ExtiRtcAlarm(EXTIMode_TypeDef mode,EXTITrigger_TypeDef trigger)
      : ExtiPeripheral<EXTI_Line17>(mode,trigger) {

      if(mode==EXTI_Mode_Interrupt)
        ExtiInterruptEnabler<17>::enable();
    }
  };
}
