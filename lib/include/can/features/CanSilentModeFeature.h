/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Feature class to enable silent mode
   */

  struct CanSilentModeFeature : CanFeatureBase {

    /**
	   * Constructor
	   * @param can reference to the CAN peripheral
	   */

    CanSilentModeFeature(Can& can)
	    : CanFeatureBase(can) {
      ((CAN_InitTypeDef *)can)->CAN_Mode |= CAN_Mode_Silent;
	  }


    /**
	   * Post CAN_Init() initialisation
	   */

	  void initialise() {
	  }
  };
}
