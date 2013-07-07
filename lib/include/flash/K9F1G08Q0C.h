/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

	/**
	 * @brief Support for the Samsung K9F1G08Q0C 1Gbit (128M x 8bit) NAND flash
	 *
	 * The device is presented as a block device that can be used to create
	 * filesystems on.
	 */

	template<class TFsmc>
	class K9F1G08Q0C : public BlockDevice,
										 public FsmcNand<TFsmc> {

		public:
			K9F1G08Q0C(const FsmcNandTiming& timing);
			virtual ~K9F1G08Q0C() {}

			// overrides from BlockDevice

			virtual uint32_t getTotalBlocksOnDevice() override;
			virtual uint32_t getBlockSizeInBytes() override;
			virtual bool readBlock(void *dest,uint32_t blockIndex) override;
			virtual bool readBlocks(void *dest,uint32_t blockIndex,uint32_t numBlocks) override;
			virtual bool writeBlock(const void *src,uint32_t blockIndex) override;
			virtual bool writeBlocks(const void *src,uint32_t blockIndex,uint32_t numBlocks) override;
			virtual formatType getFormatType() override;
			virtual bool getMbr(Mbr *mbr) override;
	};


	/**
	 * Constructor. Reset the device and read the device ID data. ErrorProvider holds
	 * the error code if this did not work
	 * @param fsmc The FSMC connection for this device.
	 */

	template<class TFsmc>
	inline K9F1G08Q0C<TFsmc>::K9F1G08Q0C(const FsmcNandTiming& timing)
		: FsmcNand<TFsmc>(timing) {
	}


	template<class TFsmc>
	inline uint32_t K9F1G08Q0C<TFsmc>::getTotalBlocksOnDevice() {
		return 262144;
	}

	template<class TFsmc>
	inline uint32_t K9F1G08Q0C<TFsmc>::getBlockSizeInBytes() {
		return 512;
	}

	template<class TFsmc>
	inline bool K9F1G08Q0C<TFsmc>::readBlock(void *dest __attribute__((unused)),uint32_t blockIndex __attribute__((unused)))  {
		return false;
	}

	template<class TFsmc>
	inline bool K9F1G08Q0C<TFsmc>::readBlocks(void *dest __attribute__((unused)),uint32_t blockIndex __attribute__((unused)),uint32_t numBlocks __attribute__((unused))) {
		return false;
	}

	template<class TFsmc>
	inline bool K9F1G08Q0C<TFsmc>::writeBlock(const void *src __attribute__((unused)),uint32_t blockIndex __attribute__((unused))) {
		return false;
	}

	template<class TFsmc>
	inline bool K9F1G08Q0C<TFsmc>::writeBlocks(const void *src __attribute__((unused)),uint32_t blockIndex __attribute__((unused)),uint32_t numBlocks __attribute__((unused))) {
		return false;
	}

	template<class TFsmc>
	inline BlockDevice::formatType K9F1G08Q0C<TFsmc>::getFormatType() {
		return BlockDevice::formatNoMbr;
	}

	template<class TFsmc>
	inline bool K9F1G08Q0C<TFsmc>::getMbr(Mbr *mbr __attribute__((unused))) {
		return false;
	}
}

