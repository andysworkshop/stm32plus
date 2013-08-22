/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
	namespace spiflash {


		/**
		 * Winbond W25Q16DW 1.8V 16Mbit Serial Flash Memory with Dual/Quad SPI & QPI
		 *
		 */

		template<class TSpi>
		struct W25Q16DW : WriteStatus<TSpi>,					// standard commands
											WriteDisable<TSpi>,
											ReadStatusRegister<TSpi>,
											WriteEnable<TSpi>,
											PageProgram<TSpi>,
											Read<TSpi> {

			/**
			 * Constructor
			 * @param spi Reference to the SPI object
			 */

			W25Q16DW(TSpi& spi) {
				this->_spi=&spi;
			}
		};
	}
}
