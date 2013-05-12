/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F1
#error This class can only be used with the STM32F1 series
#endif


namespace stm32plus {

	/**
	 * DMA feature to enable reading via DMA
	 * @tparam TSpi The type of the Spi peripheral (Spi1<...>, Spi2<...> ... )
	 */

	template<class TSpi,uint32_t TPriority=DMA_Priority_High>
	class SpiDmaWriterFeature : public DmaFeatureBase {

		public:
			SpiDmaWriterFeature(Dma& dma);
			void beginWrite(const void *source,uint32_t count);
	};


	/**
	 * Constructor, store the reference to the DMA base class
	 * @param dma the base class reference
	 */

	template<class TSpi,uint32_t TPriority>
	inline SpiDmaWriterFeature<TSpi,TPriority>::SpiDmaWriterFeature(Dma& dma)
		: DmaFeatureBase(dma) {

		SPI_TypeDef *spi;

		spi=(SPI_TypeDef *)TSpi::PERIPHERAL_BASE;

		_init.DMA_PeripheralBaseAddr=reinterpret_cast<uint32_t>(&(spi->DR));
		_init.DMA_DIR=DMA_DIR_PeripheralDST;											// 'peripheral' is destination
		_init.DMA_PeripheralInc=DMA_PeripheralInc_Disable;				// 'peripheral' does not increment
		_init.DMA_MemoryInc=DMA_MemoryInc_Enable;									// memory is incremented
		_init.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;	// transferring bytes
		_init.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;					// user defined word size
		_init.DMA_Mode=DMA_Mode_Normal;														// not a circular buffer
		_init.DMA_Priority=TPriority;															// user-configurable priority
		_init.DMA_M2M=DMA_M2M_Disable;														// memory->peripheral configuration

		SPI_I2S_DMACmd(spi,SPI_I2S_DMAReq_Tx,ENABLE);
	}


	/**
	 * Start a transfer of data to the Spi
	 *
	 * @param[in] source memory address of the source data bytes.
	 * @param[in] count The number of bytes to transfer.
	 */

	template<class TSpi,uint32_t TPriority>
	inline void SpiDmaWriterFeature<TSpi,TPriority>::beginWrite(const void *source,uint32_t count) {

		DMA_Channel_TypeDef *peripheralAddress;

		// set up the parameters for this transfer

		_init.DMA_MemoryBaseAddr=reinterpret_cast<uint32_t>(source);
		_init.DMA_BufferSize=count;

		// this class is always in a hierarchy with DmaPeripheral

		peripheralAddress=_dma;

			// disable and then re-enable

    DMA_Cmd(peripheralAddress,DISABLE);
    DMA_Init(peripheralAddress,&_init);
    DMA_Cmd(peripheralAddress,ENABLE);
	}
}
