/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


// ensure the MCU series is correct
#if !defined(STM32PLUS_F1)
#error This class can only be used with the STM32F1 series
#endif


namespace stm32plus {


  /**
   * Default pin package:
   * (TX,RX) = (PA12,PA11)
   */

  struct Can1DefaultPinPackage {
    enum{
      Port_TX=GPIOA_BASE,
      Port_RX=GPIOA_BASE,

      Pin_TX=GPIO_Pin_12,
      Pin_RX=GPIO_Pin_11
    };
  };


  /**
   * The default, non-remapped pin package
   */

  template<class... Features>
  struct Can1 : CanPeripheral<Can1DefaultPinPackage,PERIPHERAL_CAN1>,
  	  	  	  	  Features... {
    /**
     * Constructor
     * @param params Initialisation parameters
     */

	  Can1(const Parameters& params)
	    : CanPeripheral<Can1DefaultPinPackage,PERIPHERAL_CAN1>(params),
	      Features(static_cast<Can&>(*this))... {

	    initialisePeripheral();
      RecursiveVoidInit<Can1,Features...>::tinit(this);
	  }
  };


  /**
   * Remap #1:
   * (TX,RX) = (PB9,PB8)
   */

  struct Can1Remap1PinPackage {
  	enum{
  		Port_TX=GPIOB_BASE,
  		Port_RX=GPIOB_BASE,

  		Pin_TX=GPIO_Pin_9,
  		Pin_RX=GPIO_Pin_8
  	};
  };


  /**
   * The remapped version of CAN1
   */

  template<class... Features>
  struct Can1_Remap1 : CanPeripheral<Can1Remap1PinPackage,PERIPHERAL_CAN1>,
  	  	  	  	  Features...{
    /**
     * Constructor
     * @param params Initialisation parameters
     */

	  Can1_Remap1(const Parameters& params)
	      : CanPeripheral<Can1Remap1PinPackage,PERIPHERAL_CAN1>(params),
	        Features(static_cast<Can&>(*this))...{

	    GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE);

	    initialisePeripheral();
      RecursiveVoidInit<Can1_Remap1,Features...>::tinit(this);
		}
  };


  /**
   * Remap #2:
   * (TX,RX) = (PD1,PD0)
   */

  struct Can1Remap2PinPackage {
  	enum{
  		Port_TX=GPIOD_BASE,
  		Port_RX=GPIOD_BASE,

  		Pin_TX=GPIO_Pin_1,
  		Pin_RX=GPIO_Pin_0
  	};
  };


  /**
   * The remapped version of CAN1
   */

  template<class... Features>
  struct Can1_Remap2 : CanPeripheral<Can1Remap2PinPackage,PERIPHERAL_CAN1>,
  	  	  	  	  Features...{
    /**
     * Constructor
     * @param params Initialisation parameters
     */

	  Can1_Remap2(const Parameters& params)
	      : CanPeripheral<Can1Remap2PinPackage,PERIPHERAL_CAN1>(params),
	        Features(static_cast<Can&>(*this))...{

	    GPIO_PinRemapConfig(GPIO_Remap2_CAN1,ENABLE);

	    initialisePeripheral();
      RecursiveVoidInit<Can1_Remap2,Features...>::tinit(this);
		}
  };
}

