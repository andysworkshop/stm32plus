/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
	namespace spiflash {


		/**
		 * @brief Implementation of an input stream for SPI flash devices
		 * @tparam TSpiFlash An object that implements fastRead() and getSize()
		 */

		template<class TSpiFlash>
		class SpiFlashInputStream : public InputStream {

			protected:
				const TSpiFlash& _spiFlash;
				uint32_t _initialOffset;
				uint32_t _offset;

			public:
				SpiFlashInputStream(const TSpiFlash& spiFlash,uint32_t initialOffset);
				virtual ~SpiFlashInputStream() {}

				// overrides from InputStream

				virtual int16_t read() override;
				virtual bool read(void *buffer,uint32_t size,uint32_t& actuallyRead) override;
				virtual bool skip(uint32_t howMuch) override;
				virtual bool available() override;
				virtual bool close() override;
				virtual bool reset() override;
		};


		/**
		 * Constructor
		 */

		template<class TSpiFlash>
		inline SpiFlashInputStream<TSpiFlash>::SpiFlashInputStream(const TSpiFlash& spiFlash,uint32_t initialOffset)
			: _spiFlash(spiFlash),
			  _initialOffset(initialOffset),
			  _offset(initialOffset) {
		}


		/**
		 * Doesn't do anything.
		 * @return always true
		 */

		template<class TSpiFlash>
		inline bool SpiFlashInputStream<TSpiFlash>::close() {
			return true;
		}


		/**
		 * Reset the stream pointer to the beginning
		 * @return true
		 */

		template<class TSpiFlash>
		inline bool SpiFlashInputStream<TSpiFlash>::reset() {
			_offset=_initialOffset;
			return true;
		}


		/**
		 * Read a single byte
		 * @return The byte in the lower 8 bits or E_END_OF_STREAM / E_STREAM ERROR (negative numbers)
		 */

		template<class TSpiFlash>
		inline int16_t SpiFlashInputStream<TSpiFlash>::read() {

			uint8_t c;
			uint32_t actuallyRead;

			// must have at least a byte available

			if(!available())
				return E_END_OF_STREAM;

			// try to read a byte

			if(!read(&c,1,actuallyRead))
				return E_STREAM_ERROR;

			// return the byte in the lower bits

			return c;
		}


		/**
		 * Read a chunk of bytes up to the amount requested.
		 * @param buffer Where to read out the bytes to
		 * @param size The maximum number of bytes to read
		 * @param actuallyRead The actual number of bytes read, which may be less than requested if the end of stream is hit.
		 * @return false if there was an error. end of stream is not an error.
		 */

		template<class TSpiFlash>
		inline bool SpiFlashInputStream<TSpiFlash>::read(void *buffer,uint32_t size,uint32_t& actuallyRead) {

		}


		template<class TSpiFlash>
		inline bool SpiFlashInputStream<TSpiFlash>::skip(uint32_t howMuch) {

		}


		template<class TSpiFlash>
		inline bool SpiFlashInputStream<TSpiFlash>::available() {

		}
	}
}
