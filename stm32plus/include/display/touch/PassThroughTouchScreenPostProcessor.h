/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
	namespace display {

		/**
		 * Touch screen postprocessor that passes values through without doing anything
		 */

		class PassThroughTouchScreenPostProcessor : public TouchScreenPostProcessor {
			public:
				virtual ~PassThroughTouchScreenPostProcessor() {}

				/**
				 * Pass through post processor, does nothing and always returns Completed.
				 * @return Completed.
				 */

				virtual PostProcessAction postProcess(Point& /* point */,int /* sequenceNumber */) override {
					return Completed;
				}
		};
	}
}
