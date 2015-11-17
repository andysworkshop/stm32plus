/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Base class for CAN features
   */

  class CanFeatureBase {

    protected:
			Can& _can;

		public:
			CanFeatureBase(Can& can);
			operator Can&();
	};


  /**
   * Constructor
   * @param can reference to the CAN peripheral
   */

	inline CanFeatureBase::CanFeatureBase(Can& can)
		: _can(can){
	}


	/**
	 * operator to return the CAN peripheral reference
	 */

	inline CanFeatureBase::operator stm32plus::Can &() {
	  return _can;
	}
}
