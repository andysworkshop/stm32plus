/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {

/**
 * @brief Driver for ATMEL AT45DB161D SPI DataFlash device.
 *
 * This driver interacts with the SPI peripheral in polling mode.
 */

	class AT45DB161D : public BlockDevice {

		protected:
			uint32_t _numPages;

			static constexpr uint8_t COMMAND_DEVICE_ID_READ = 0x9F;
			static constexpr uint8_t COMMAND_READ_STATUS_REGISTER = 0xD7;

		protected:

			/**
			 * Write an 8-bit command to the device and sequentially receive reponseSize bytes of data.
			 * @param[in] command The command to send.
			 * @return false if it fails.
			 */

			bool writeCommand(uint8_t command,uint8_t *response,uint32_t responseSize) {

				bool retval=true;
				uint8_t dummy;

				// chip select LOW
				_spi.setNss(false);

				// send the byte
				if(!_spi.send(&command,1))
					retval=false;
				else {
					// receive the response

					dummy=0;
					while(responseSize--) {
						if(!_spi.send(&dummy,1,response++)) {
							retval=false;
							break;
						}
					}
				}

				// chip select HIGH
				_spi.setNss(true);
				return retval;
			}


			/**
			 * Read the 8-bit status register from the device.
			 * @return false if it fails.
			 */

			bool readStatusRegister(uint8_t& sr) {
				return writeCommand(COMMAND_READ_STATUS_REGISTER,&sr,1);
			}

			bool readDeviceId();

		public:
			Spi& _spi;

			enum {
				E_MBR_NOT_SUPPORTED=1,
				E_INVALID_DENSITY,
				E_INVALID_MANUFACTURER_ID,
				E_INVALID_PRODUCT_ID
			};

		public:

			/*
			 * Constructor
			 */

			AT45DB161D(Spi& spi);

			/**
			 * Virtual destructor, does nothing
			 */

			virtual ~AT45DB161D() {
			}

			bool enable512BytePages();

			// overrides from BlockDevice

			virtual uint32_t getTotalBlocksOnDevice() override;
			virtual uint32_t getBlockSizeInBytes() override;
			virtual bool readBlock(void *dest,uint32_t blockIndex) override;
			virtual bool readBlocks(void *dest,uint32_t blockIndex,uint32_t numBlocks) override;
			virtual bool writeBlock(const void *src,uint32_t blockIndex) override;
			virtual bool writeBlocks(const void *src,uint32_t blockIndex,uint32_t numBlocks) override;
			virtual formatType getFormatType() override;
			virtual bool getMbr(Mbr *mbr);
	};
}

