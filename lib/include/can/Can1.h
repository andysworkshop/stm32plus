/*
 * Can1.h
 *
 *  Created on: 2015. nov. 10.
 *      Author: elektroman
 */
#pragma once
#include "fwlib/f1/stdperiph/inc/stm32f10x_gpio.h"

namespace stm32plus {



template<class TPinPackage, class... Features>
struct Can1_Custom : CanPeripheral<TPinPackage,PERIPHERAL_CAN1>,
					Features...{

   /**
	 * Constructor
	 * @param params Initialisation parameters
	 */
	Can1_Custom(const typename CanPeripheral<TPinPackage,PERIPHERAL_CAN1>::Parameters& params)
			: CanPeripheral<TPinPackage,PERIPHERAL_CAN1>(params),
			  Features(static_cast<Can&>(*this))...{
		}
	};



/**
 * Default pin package:
 * (TX,RX) = (PA12,PA11)
 */

struct Can1DefaultPinPackage {
	enum{
		Port_TX = GPIOA_BASE,
		Port_RX=GPIOA_BASE,

		Pin_TX=GPIO_Pin_12,
		Pin_RX=GPIO_Pin_11
	};
};

/**
   * Convenience class to match the F1 pin for pin.
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
   * Convenience class to match the F1 pin for pin.
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
		  	  }
  };


}  // namespace stm32plus
