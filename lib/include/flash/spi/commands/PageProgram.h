/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
	namespace spiflash {


		/**
		 * Page program command implementation
		 */

		template<uint8_t TOpCode=Command::CMD_PAGE_PROGRAM,uint8_t TAddressBytes=3,class TSpi>
		struct WriteEnable : Command<TSpi> {

			/**
			 * Provide a pageProgram() function for the derived class
			 */

			bool pageProgram(uint32_t address,const uint8_t *data,uint32_t dataSize) const {
				return this->writeCommand(TOpCode,address,TAddressBytes,0,data,dataSize);
			}
		};
	}
}
