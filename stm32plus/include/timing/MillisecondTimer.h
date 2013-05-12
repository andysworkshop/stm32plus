/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


/**
 * The stm32plus namespace is the top-level namespace for all classes in the library.
 */

namespace stm32plus {

	/**
	 * @brief Millisecond delay counter using the SYSTICK core peripheral
	 */

	class MillisecondTimer {

		public:
			static uint32_t _counter;

		public:
			static void initialise();
			static void delay(uint32_t millis_);
			static uint32_t millis();
			static void reset();
	};


	/**
	 * Get the current value of the systick counter.
	 * @return The current value of the counter.
	 */

	inline uint32_t MillisecondTimer::millis() {
		return _counter;
	}


	/**
	 * Delay for given time. Waits for the current value of the systick counter to reach a target.
	 * @param millis The amount of time to wait.
	 */

	inline void MillisecondTimer::delay(uint32_t millis) {

		uint32_t target;

		target=_counter+millis;
		while(_counter<target);
	}


	/**
	 * Reset the counter to zero
	 */

	inline void MillisecondTimer::reset() {
		_counter=0;
	}
}

