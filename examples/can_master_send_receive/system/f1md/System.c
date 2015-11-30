/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#include "config/stdperiph.h"


static volatile ErrorStatus HSEStartUpStatus=SUCCESS;

uint32_t SystemCoreClock=72000000;
uint32_t HSE_Clock = (uint32_t)HSE_VALUE;
uint8_t  multiplier;
uint32_t RCC_PLLMul;

void check_failed(uint8_t *file, uint32_t line){
file++;
line++;
file--;
line--;

	while(1);
}

void SystemInit() {

	// SYSCLK, HCLK, PCLK2 and PCLK1 configuration
	// RCC system reset(for debug purpose) */
	RCC_DeInit();

	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);

	/* Wait till HSE is ready */
	HSEStartUpStatus=RCC_WaitForHSEStartUp();

	if(HSEStartUpStatus == SUCCESS) {
		/* Enable Prefetch Buffer */
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

		/* Flash 2 wait state */
		FLASH_SetLatency(FLASH_Latency_2);

		/* HCLK = SYSCLK */
		RCC_HCLKConfig(RCC_SYSCLK_Div1);

		/* PCLK2 = HCLK */
		RCC_PCLK2Config(RCC_HCLK_Div1);

		/* PCLK1 = HCLK/2 */
		RCC_PCLK1Config(RCC_HCLK_Div2);


		/* Calculate the multiplier value */
		multiplier= SystemCoreClock / HSE_Clock;
		if ( HSE_Clock *multiplier != SystemCoreClock )
			while(1){/* Error, multiplier cannot be calculated */}
		switch (multiplier) {
			case 2:		RCC_PLLMul= RCC_PLLMul_2 ; break;
			case 3: 	RCC_PLLMul= RCC_PLLMul_3 ; break;
			case 4:		RCC_PLLMul=	RCC_PLLMul_4 ; break;
			case 5:		RCC_PLLMul=	RCC_PLLMul_5 ; break;
			case 6:		RCC_PLLMul= RCC_PLLMul_6 ; break;
			case 7:		RCC_PLLMul=	RCC_PLLMul_7 ; break;
			case 8:		RCC_PLLMul=	RCC_PLLMul_8 ; break;
			case 9:		RCC_PLLMul=	RCC_PLLMul_9 ; break;
			case 10:	RCC_PLLMul=	RCC_PLLMul_10; break;
			case 11:	RCC_PLLMul=	RCC_PLLMul_11; break;
			case 12:	RCC_PLLMul=	RCC_PLLMul_12; break;
			case 13:	RCC_PLLMul=	RCC_PLLMul_13; break;
			case 14:	RCC_PLLMul=	RCC_PLLMul_14; break;
			case 15:	RCC_PLLMul=	RCC_PLLMul_15; break;
			case 16:	RCC_PLLMul=	RCC_PLLMul_16; break;
			default: 	while(1){/* Error, multiplier cannot be calculated */}; break;
		}


		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul);

		/* Enable PLL */
		RCC_PLLCmd(ENABLE);

		/* Wait till PLL is ready */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {
		}

		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		/* Wait till PLL is used as system clock source */
		while(RCC_GetSYSCLKSource() != 0x08) {
		}
	}
}

