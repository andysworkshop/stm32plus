/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

namespace stm32plus {

	namespace ObservableEvent {

	/**
	 * The observable events
	 */

		enum E {
			/// A DMA transfer has completed OK
			DMA_TransferComplete,

			/// A DMA transfer is halfway complete
			DMA_HalfTransfer,

			/// There was an error during a DMA transfer
			DMA_TransferError,

			/// USART receive data
			USART_Receive,

			/// USART pre-transmit data
			USART_ReadyToTransmit,

			/// USART parity error
			USART_ParityError,

			/// USART transmit complete
			USART_TransmitComplete,

			/// USART idle line
			USART_Idle,

			/// USART line break detect
			USART_LineBreakDetect,

			/// USART clear to send
			USART_ClearToSend,

			/// USART error
			USART_Error,

			/// SPI receive data
			SPI_Receive,

			/// SPI pre-transmit data
			SPI_ReadyToTransmit,

			/// RTC tick event
			RTC_Tick,

			/// RTC wakeup event
			RTC_Wakeup,

			/// RTC alarm event (F1)
			RTC_Alarm,

			/// RTC alarm A event (F4)
			RTC_AlarmA,

			/// RTC alarm B event (F4)
			RTC_AlarmB,

			/// RTC overflow event
			RTC_Overflow,

			/// SDIO complete
			SDIO_DataEnd,

			// CRC failure
			SDIO_CrcFail,

			// timeout
			SDIO_Timeout,

			// receive fifo overrun
			SDIO_RxOverrun,

			// transmit fifo overrun
			SDIO_TxOverrun,

			// start bit error
			SDIO_StartBitError,

			/// Touch panel event
			TouchPanel_ReadyForSampling,

			/// An ethernet frame received
			Ethernet_ReceiveFrame,

			/// An ethernet reception error
			Ethernet_ReceiveError,

			/// External interrupt has triggered
			EXTI_Triggered,

			/// Timer events
			Timer_Break,
			Timer_Update,
			Timer_Trigger,
			Timer_Com,
			Timer_Compare1,
			Timer_Compare2,
			Timer_Compare3,
			Timer_Compare4,

			/// I2C TXE
			I2C_ReadyToTransmit,

			/// I2C RXNE
			I2C_Receive,

			/// I2C_IT_STOPF
			I2C_StopBitReceived,

			/// I2C_IT_ADD10
			I2C_10BitHeaderSent,

			/// I2C_IT_BTF
			I2C_ByteTransferSent,

			/// I2C_IT_ADDR
			I2C_AddressSent,

			/// I2C_IT_SB
			I2C_StartBitSent,

			/// I2C_IT_SMBALERT
			I2C_SmbAlert,

			/// I2C_IT_TIMEOUT
			I2C_Timeout,

			/// I2C_IT_PECERR
			I2C_PECError,

			/// I2C_IT_OVR
			I2C_Overrun,

			/// I2C_IT_AF
			I2C_AckFailure,

			/// I2C_IT_ARLO
			I2C_ArbitrationLoss,

			/// I2C_IT_BERR
			I2C_BusError
		};
	}
}
