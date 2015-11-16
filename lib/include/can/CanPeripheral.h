/*
 * CanPeripheral.h
 *
 *  Created on: 2015. nov. 10.
 *      Author: elektroman
 */

#pragma once
#include "fwlib/f1/stdperiph/inc/stm32f10x_can.h"

namespace stm32plus {

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


			Parameters(uint32_t baudrate=100000,uint32_t samplepoint=875)
			{
				volatile uint32_t T1 =samplepoint;
				volatile uint32_t T2 =1000-samplepoint;
				uint8_t Tsum;
				bool calculated = false;
				RCC_ClocksTypeDef Clocks;
				RCC_GetClocksFreq(&Clocks);

				while (T2 > 8 || T1 >16 )
				{
					if (T2%11==0 && T1%11==0) {T2/=11;T1/=11;calculated= true; }
					else if (T2%7 ==0 && T1%7 ==0) {T2/=7;T1/=7;calculated= true; }
					else if (T2%5 ==0 && T1%5 ==0) {T2/=5;T1/=5;calculated= true; }
					else if (T2%3 ==0 && T1%3 ==0) {T2/=3;T1/=3;calculated= true; }
					else if (T2%2 ==0 && T1%2 ==0) {T2/=2;T1/=2; calculated= true; }
					else calculated= false;
					if (T2<1 || T1 <2) {calculated= false; break;}

				}
				if(calculated){
					Tsum=T1+T2+1;
					can_Prescaler= Clocks.PCLK1_Frequency / (baudrate*Tsum);
					can_BS1 = T1-1;
					can_BS2 = T2-1;


				}
				else
				{
					//cannot calculate the CAN baudrate, use manual caluclating
					while(1);
				}
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

	protected :
		void initialisePeripheral();

	};



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
	   * Destructor, disable peripheral and disable clocks
	   */

	  template<class TPinPackage,PeripheralName TPeripheralName>
	  inline CanPeripheral<TPinPackage,TPeripheralName>::~CanPeripheral(){

		  // disable the peripheral
		  disablePeripheral();

		  // clocks off

		  ClockControl<TPeripheralName>::Off();
	  }

	  template<class TPinPackage, PeripheralName TPeripheralName>
	  	inline void CanPeripheral<TPinPackage,TPeripheralName>::initialisePeripheral()
		{

			// initialise

			CAN_Init((CAN_TypeDef *)PeripheralTraits<TPeripheralName>::PERIPHERAL_BASE,&_init);

			enablePeripheral();
		}



}  // namespace stm32plus
