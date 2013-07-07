/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"

#if !defined(STM32PLUS_F1_CL)

#include "config/flash.h"


namespace stm32plus {


	/**
	 * Constructor. The device density will be obtained via a call to read the status register. The caller
	 * should check the error status after this constructor has returned. The device is assumed to be operating
	 * in SPI mode 3 (CPOL=1, CPHA=1), the default mode after reset. Bit ordering is MSB first.
	 *
	 * @param[in] spi_ The SPI polling writer that we will use to communicate with the chip.
	 */

	AT45DB161D::AT45DB161D(Spi& spi_)
		: _spi(spi_) {

		errorProvider.clear();

	// the data sheet specifies that we wait 20ms after power has been applied
	// assume that we're in here really early after power on and delay the max.

		MillisecondTimer::delay(20);
		readDeviceId();
	}


	/**
	 * Read the device density from the status register and set the corresponding number of blocks.
	 * @return false if it fails.
	 */

	bool AT45DB161D::readDeviceId() {

		uint8_t codes[4],density;

		if(!writeCommand(COMMAND_DEVICE_ID_READ,codes,sizeof(codes)))
			return false;

		if(codes[0]!=0x1f)
			return errorProvider.set(ErrorProvider::ERROR_PROVIDER_AT45DB161D,E_INVALID_MANUFACTURER_ID);

		if((codes[1] & 0x20)!=0x20)
			return errorProvider.set(ErrorProvider::ERROR_PROVIDER_AT45DB161D,E_INVALID_PRODUCT_ID);

		density=codes[1] & 0x1f;
		if(density==6)
			_numPages=4096;
		else
			return errorProvider.set(ErrorProvider::ERROR_PROVIDER_AT45DB161D,E_INVALID_DENSITY,density);

		return true;
	}


	/**
	 * Get the total number of blocks on the device.
	 *
	 */

	uint32_t AT45DB161D::getTotalBlocksOnDevice() {
		return _numPages;
	}


	/**
	 * Set the device to use 512 byte pages. Some of the AT45DB161D devices ship with 528 byte pages. I'm sure that someone inside
	 * Atmel had a good reason to deviate from the huge majority of real world use cases but thankfully they included a switch to
	 * put the device into 512 byte mode.
	 *
	 * EXTREMELY IMPORTANT NOTE: This is a one way switch. Once in 512 byte mode the device cannot
	 * be switched back to 528 byte mode.
	 *
	 * If this device is to be used as a BlockDevice (e.g. for FAT16/FAT32 access) then 512 byte mode MUST be enabled.
	 */

	bool AT45DB161D::enable512BytePages() {
		return false;
	}


	/**
	 * Return the block size, this is 512 bytes and therefore this device must be running with a page size of 512.
	 * @see enable512BytePages
	 * @return 512
	 */

	uint32_t AT45DB161D::getBlockSizeInBytes() {
		return 512;
	}


	/*
	 * Read a block
	 */

	bool AT45DB161D::readBlock(void *dest_  __attribute((unused)),uint32_t blockIndex_  __attribute((unused))) {
		return false;
	}


	/*
	 * Read multiple blocks
	 */

	bool AT45DB161D::readBlocks(void *dest_  __attribute((unused)),uint32_t blockIndex_  __attribute((unused)),uint32_t numBlocks_  __attribute((unused))) {
		return false;
	}


	/*
	 * Write a block
	 */

	bool AT45DB161D::writeBlock(const void */* src */,uint32_t blockIndex_  __attribute((unused))) {
		return false;
	}


	/*
	 * Write multiple blocks
	 */

	bool AT45DB161D::writeBlocks(const void */* src */,uint32_t blockIndex_  __attribute((unused)),uint32_t numBlocks_  __attribute((unused))) {
		return false;
	}


	/**
	 * Return the format type, which is formatNoMbr because we're going to operate this device in "super-floppy" mode.
	 */

	BlockDevice::formatType AT45DB161D::getFormatType() {
		return formatNoMbr;
	}


	/**
	 * Get the MBR, or not as in our case.
	 * @return false because we haven't got one.
	 */

	bool AT45DB161D::getMbr(Mbr *mbr_  __attribute((unused))) {
		return errorProvider.set(ErrorProvider::ERROR_PROVIDER_AT45DB161D,E_MBR_NOT_SUPPORTED);
	}
}


#endif
