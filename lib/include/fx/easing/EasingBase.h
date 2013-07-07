/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
	namespace fx {

		/**
		 * Base class for easing functions
		 */

		class EasingBase {

			protected:
				double _change;
				double _duration;

			public:
				/// default constructor
				EasingBase();
				virtual ~EasingBase() {}

				/**
				 * Ease a transition in
				 * @param[in] time_ The time to do the transition.
				 */

				virtual double easeIn(double time_) const=0;

				/**
				 * Ease a transition out.
				 * @param[in] time_ The time to do the transition.
				 */

				virtual double easeOut(double time_) const=0;

				/**
				 * Ease a transition in and out.
				 * @param[in] time_ The time to do the transition.
				 */

				virtual double easeInOut(double time_) const=0;

				void setDuration(double duration_);
				void setTotalChangeInPosition(double totalChangeInPosition_);
		};
	}
}
