/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Can peripheral base template. The specific instances of the CAN peripheral
   * inherit from this
   */

    template<class TPinPackage, PeripheralName TPeripheralName>
    class CanPeripheral : public Can {

      public:

        struct Parameters {

        uint16_t can_Prescaler;   /*!< Specifies the length of a time quantum.
                                         It ranges from 1 to 1024. */
        uint8_t can_Mode;         /*!< Specifies the CAN operating mode.
                                         This parameter can be a value of
                                        @ref CAN_operating_mode */
        uint8_t can_SJW;          /*!< Specifies the maximum number of time quanta
                                         the CAN hardware is allowed to lengthen or
                                         shorten a bit to perform resynchronization.
                                         This parameter can be a value of
                                         @ref CAN_synchronisation_jump_width */
        uint8_t can_BS1;          /*!< Specifies the number of time quanta in Bit
                                         Segment 1. This parameter can be a value of
                                         @ref CAN_time_quantum_in_bit_segment_1 */
        uint8_t can_BS2;          /*!< Specifies the number of time quanta in Bit
                                         Segment 2.
                                         This parameter can be a value of
                                         @ref CAN_time_quantum_in_bit_segment_2 */
        FunctionalState can_TTCM; /*!< Enable or disable the time triggered
                                         communication mode. This parameter can be set
                                         either to ENABLE or DISABLE. */

        FunctionalState can_ABOM;  /*!< Enable or disable the automatic bus-off
                                          management. This parameter can be set either
                                          to ENABLE or DISABLE. */

        FunctionalState can_AWUM;  /*!< Enable or disable the automatic wake-up mode.
                                          This parameter can be set either to ENABLE or
                                          DISABLE. */

        FunctionalState can_NART;  /*!< Enable or disable the no-automatic
                                          retransmission mode. This parameter can be
                                          set either to ENABLE or DISABLE. */

        FunctionalState can_RFLM;  /*!< Enable or disable the Receive FIFO Locked mode.
                                          This parameter can be set either to ENABLE
                                          or DISABLE. */

        FunctionalState can_TXFP;  /*!< Enable or disable the transmit FIFO priority.
                                          This parameter can be set either to ENABLE
                                                or DISABLE. */

        Parameters(uint32_t baudrate=100000,uint32_t samplepoint=500) {

          uint32_t T1;
          uint32_t T2;

          uint32_t Tsum;
          RCC_ClocksTypeDef Clocks;
          RCC_GetClocksFreq(&Clocks);

          if( Clocks.PCLK1_Frequency%baudrate != 0 ){
        	//Configuration failed
        	return;
          }

          Tsum = Clocks.PCLK1_Frequency/baudrate;

          while( Tsum > 20 ){
            if( Tsum % 11 == 0 ) Tsum/=11;
            else if( Tsum % 7 == 0 ) Tsum/=7;
            else if( Tsum % 5 == 0 ) Tsum/=5;
            else if( Tsum % 3 == 0 ) Tsum/=3;
            else if( Tsum % 2 == 0 ) Tsum/=2;
            else{
              if( Tsum == 23 ) break;
              //Configuration failed
              return;
            }
          }

          Tsum -= 1;

          T1 = (uint32_t)(Tsum*samplepoint+499)/1000;
          T2 = Tsum - T1;

          if( T1 == 0 ){
            T1 += 1;
            T2 -= 1;
          }
          else if( T2 == 0 ){
            T1 -= 1;
            T2 += 1;
          }

          if( T1 > 16){
            T1 = 16;
            T2 = Tsum-16;
          }
          else if( T2 > 8){
            T2 = 8;
            T1 = Tsum-8;
          }

          Tsum=T1+T2+1;
          can_Prescaler=Clocks.PCLK1_Frequency/(baudrate*Tsum);
          can_BS1=T1-1;
          can_BS2=T2-1;
          can_Mode = CAN_Mode_Normal;
          can_SJW = CAN_SJW_1tq;
          can_TTCM = DISABLE;
          can_ABOM = DISABLE;
          can_AWUM = ENABLE;
          can_NART = DISABLE;
          can_RFLM = DISABLE;
          can_TXFP = ENABLE;
        }
      };

      CanPeripheral(const Parameters& params);
      ~CanPeripheral();

      protected:
        void initialisePeripheral();
    };


    /**
     * Constructor
     * @param params
     */

    template<class TPinPackage, PeripheralName TPeripheralName>
    inline CanPeripheral<TPinPackage,TPeripheralName>::CanPeripheral(const Parameters& params)
        : Can((CAN_TypeDef * )PeripheralTraits<TPeripheralName>::PERIPHERAL_BASE) {

      // clocks on first before any features get initialised

      ClockControl<TPeripheralName>::On();
      CanPinInitialiser<TPinPackage,TPeripheralName>::initialise();

      // initialise the peripheral

      _init.CAN_ABOM=params.can_ABOM;
      _init.CAN_AWUM=params.can_AWUM;
      _init.CAN_BS1=params.can_BS1;
      _init.CAN_BS2=params.can_BS2;
      _init.CAN_Mode=params.can_Mode;
      _init.CAN_NART=params.can_NART;
      _init.CAN_Prescaler=params.can_Prescaler;
      _init.CAN_RFLM=params.can_RFLM;
      _init.CAN_SJW=params.can_SJW;
      _init.CAN_TTCM=params.can_TTCM;
      _init.CAN_TXFP=params.can_TXFP;
    }


    /**
   * Destructor, disable clocks
   */

  template<class TPinPackage,PeripheralName TPeripheralName>
  inline CanPeripheral<TPinPackage,TPeripheralName>::~CanPeripheral(){

    // clocks off

    ClockControl<TPeripheralName>::Off();
  }


  /*
   * Initialise the peripheral
   */

  template<class TPinPackage, PeripheralName TPeripheralName>
    inline void CanPeripheral<TPinPackage,TPeripheralName>::initialisePeripheral() {

    // initialise

    CAN_Init((CAN_TypeDef *)PeripheralTraits<TPeripheralName>::PERIPHERAL_BASE,&_init);
  }
}
