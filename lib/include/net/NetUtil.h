/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
	namespace net {
		namespace NetUtil {


			/**
			 * Reverse the bytes in the parameter. We can do this in a single instruction
			 * @param data
			 * @return The parameter, with bits reversed
			 */

			inline static uint16_t ntohs(register uint16_t data) {
				asm volatile( "rev16 %0, %0" :: "g" (data) );
				return data;
			}


			/**
			 * Reverse the bytes in the parameter. We can do this in a single instruction
			 * @param data
			 * @return The parameter, with bits reversed
			 */

			inline static uint32_t ntohl(register uint32_t data) {
				asm volatile( "rev %0, %0" :: "g" (data) );
				return data;
			}


			/**
			 * Reverse the bytes in the parameter. We can do this in a single instruction
			 * @param data
			 * @return The parameter, with bits reversed
			 */

			inline static uint16_t htons(register uint16_t data) {
				asm volatile( "rev16 %0, %0" :: "g" (data) );
				return data;
			}


			/**
			 * Reverse the bytes in the parameter. We can do this in a single instruction
			 * @param data
			 * @return The parameter, with bits reversed
			 */

			inline static uint32_t htonl(register uint32_t data) {
				asm volatile( "rev %0, %0" :: "g" (data) );
				return data;
			}
		}
	}
}
