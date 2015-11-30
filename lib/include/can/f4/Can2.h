/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


// ensure the MCU series is correct
#if !defined(STM32PLUS_F4)
#error This class can only be used with the STM32F4 series
#endif


namespace stm32plus {


  /**
   * Default pin package:
   * (TX,RX) = (PB13,PB12)
   */

  struct Can2DefaultPinPackage {
    enum{
      Port_RX=GPIOB_BASE,
      Port_TX=GPIOB_BASE,

      Pin_RX=GPIO_Pin_12,
      Pin_TX=GPIO_Pin_13
    };
  };


  /**
   * The default, non-remapped pin package
   */

  template<class... Features>
  struct Can2 : CanPeripheral<Can2DefaultPinPackage,PERIPHERAL_CAN2>,
  	  	  	  	  Features... {
    /**
     * Constructor
     * @param params Initialisation parameters
     */

	  Can2(const Parameters& params)
	    : CanPeripheral<Can2DefaultPinPackage,PERIPHERAL_CAN2>(params),
	      Features(static_cast<Can&>(*this))... {

	    initialisePeripheral();
      RecursiveVoidInit<Can2,Features...>::tinit(this);
	  }
  };


  /**
   * Remap #1:
   * (TX,RX) = (PB6,PB5)
   */

  struct Can2RemapPinPackage {
  	enum{
      Port_RX=GPIOB_BASE,
      Port_TX=GPIOB_BASE,

      Pin_RX=GPIO_Pin_5,
      Pin_TX=GPIO_Pin_6
  	};
  };


  /**
   * The remapped version of CAN2
   */

  template<class... Features>
  struct Can2_Remap : CanPeripheral<Can2RemapPinPackage,PERIPHERAL_CAN2>,
  	  	  	  	  Features...{
    /**
     * Constructor
     * @param params Initialisation parameters
     */

	  Can2_Remap(const Parameters& params)
	      : CanPeripheral<Can2RemapPinPackage,PERIPHERAL_CAN2>(params),
	        Features(static_cast<Can&>(*this))...{

	    initialisePeripheral();
      RecursiveVoidInit<Can2_Remap,Features...>::tinit(this);
		}
  };
    
}

