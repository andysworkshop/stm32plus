/** @defgroup GPIO_Alternate_function_selection_define 
  * @{
  */ 

/** 
  * @brief   AF 0 selection  
  * JTCK-SWCLK, JTDI, JTDO/TRACESW0, JTMS-SWDAT, MCO, NJTRST, TRACED, TRACECK
  */ 
//#define GPIO_AF_RTC_50Hz      ((uint8_t)0x00)  /* RTC_50Hz Alternate Function mapping */
#define GPIO_AF_MCO           GPIO_AF_0        /* MCO (MCO1 and MCO2) Alternate Function mapping */
//#define GPIO_AF_TAMPER        ((uint8_t)0x00)  /* TAMPER (TAMPER_1 and TAMPER_2) Alternate Function mapping */
//#define GPIO_AF_SWJ           ((uint8_t)0x00)  /* SWJ (SWD and JTAG) Alternate Function mapping */
//#define GPIO_AF_TRACE         ((uint8_t)0x00)  /* TRACE Alternate Function mapping */

/** 
  * @brief   AF 1 selection  
  * GPIO_AF_1: OUT, TIM2, TIM15, TIM16, TIM17
  */ 
#define GPIO_AF_TIM2          GPIO_AF_1        /* TIM2 Alternate Function mapping */ /* WARNING: duplicate in stm32f30x_gpio.h */
#define GPIO_AF_TIM15         GPIO_AF_1        /* TIM15 Alternate Function mapping */ /* WARNING: duplicate in stm32f30x_gpio.h */
#define GPIO_AF_TIM17         GPIO_AF_1        /* TIM17 Alternate Function mapping */

/** 
  * @brief   AF 2 selection  
  * GPIO_AF_2: COMP1_OUT, TIM1, TIM2, TIM3, TIM4, TIM8, TIM15
  */ 
#define GPIO_AF_COMP1_OUT     GPIO_AF_2        /* COMP1_OUT Alternate Function mapping */
#define GPIO_AF_TIM1          GPIO_AF_2        /* TIM1 Alternate Function mapping */
#define GPIO_AF_TIM3          GPIO_AF_2        /* TIM3 Alternate Function mapping */
#define GPIO_AF_TIM4          GPIO_AF_2        /* TIM4 Alternate Function mapping */
#define GPIO_AF_TIM8          GPIO_AF_2        /* TIM8 Alternate Function mapping */ /* WARNING: duplicate in stm32f30x_gpio.h */

/** 
  * @brief   AF 3 selection  
  * GPIO_AF_3: COMP7_OUT, TIM8, TIM15, Touch
  */ 
#define GPIO_AF_COMP7_OUT     GPIO_AF_3        /* TIM8 Alternate Function mapping */
#define GPIO_AF_TOUCH         GPIO_AF_3        /* Touch Alternate Function mapping */

/** 
  * @brief   AF 4 selection  
  * GPIO_AF_4: I2C1, I2C2, TIM1, TIM8, TIM16, TIM17
  */ 
#define GPIO_AF_I2C1          GPIO_AF_4        /* I2C1 Alternate Function mapping */
#define GPIO_AF_I2C2          GPIO_AF_4        /* I2C2 Alternate Function mapping */
//#define GPIO_AF_I2C3          ((uint8_t)0x04)  /* I2C3 Alternate Function mapping */

/** 
  * @brief   AF 5 selection  
  * GPIO_AF_5: IR_OUT, I2S2, I2S3, SPI1, SPI2, TIM8, USART4, USART5
  */ 
#define GPIO_AF_SPI1          GPIO_AF_5        /* SPI1 Alternate Function mapping */
#define GPIO_AF_SPI2          GPIO_AF_5        /* SPI2/I2S2 Alternate Function mapping */

/** 
  * @brief   AF 6 selection  
  * GPIO_AF_6:  IR_OUT, I2S2, I2S3, SPI2, SPI3, TIM1, TIM8
  */ 
#define GPIO_AF_SPI3          GPIO_AF_6        /* SPI3/I2S3 Alternate Function mapping */

/** 
  * @brief   AF 7 selection  
  * GPIO_AF_7: AOP2_OUT, CAN, COMP3_OUT, COMP5_OUT, COMP6_OUT, USART1, USART2, USART3
  */ 
#define GPIO_AF_USART1        GPIO_AF_7        /* USART1 Alternate Function mapping */
#define GPIO_AF_USART2        GPIO_AF_7        /* USART2 Alternate Function mapping */
#define GPIO_AF_USART3        GPIO_AF_7        /* USART3 Alternate Function mapping */
//#define GPIO_AF_I2S3ext       ((uint8_t)0x07)  /* I2S3ext Alternate Function mapping */

/** 
  * @brief   AF 8 selection  
  * GPIO_AF_8: COMP1_OUT, COMP2_OUT, COMP3_OUT, COMP4_OUT, COMP5_OUT, COMP6_OUT
  */ 
//#define GPIO_AF_UART4         ((uint8_t)0x08)  /* UART4 Alternate Function mapping */
//#define GPIO_AF_UART5         ((uint8_t)0x08)  /* UART5 Alternate Function mapping */
//#define GPIO_AF_USART6        ((uint8_t)0x08)  /* USART6 Alternate Function mapping */

/** 
  * @brief   AF 9 selection 
  * GPIO_AF_9: AOP4_OUT, CAN, TIM1, TIM8, TIM15
  */ 
#define GPIO_AF_CAN1          GPIO_AF_9        /* CAN1 Alternate Function mapping */
//#define GPIO_AF_CAN2          ((uint8_t)0x09)  /* CAN2 Alternate Function mapping */
//#define GPIO_AF_TIM12         ((uint8_t)0x09)  /* TIM12 Alternate Function mapping */
//#define GPIO_AF_TIM13         ((uint8_t)0x09)  /* TIM13 Alternate Function mapping */
//#define GPIO_AF_TIM14         ((uint8_t)0x09)  /* TIM14 Alternate Function mapping */

/** 
  * @brief   AF 10 selection  
  * GPIO_AF_10: AOP1_OUT, AOP3_OUT, TIM2, TIM3, TIM4, TIM8, TIM17
  */ 
//#define GPIO_AF_OTG_FS         ((uint8_t)0xA)  /* OTG_FS Alternate Function mapping */
//#define GPIO_AF_OTG_HS         ((uint8_t)0xA)  /* OTG_HS Alternate Function mapping */

/** 
  * @brief   AF 11 selection  
  * GPIO_AF_11: TIM1, TIM8
  */ 
//#define GPIO_AF_ETH             ((uint8_t)0x0B)  /* ETHERNET Alternate Function mapping */

/** 
  * @brief   AF 12 selection  
  * GPIO_AF_12: TIM1
  */ 
//#define GPIO_AF_FSMC            ((uint8_t)0xC)  /* FSMC Alternate Function mapping */
//#define GPIO_AF_OTG_HS_FS       ((uint8_t)0xC)  /* OTG HS configured in FS, Alternate Function mapping */
//#define GPIO_AF_SDIO            ((uint8_t)0xC)  /* SDIO Alternate Function mapping */

/** 
  * @brief   AF 13 selection  
  */ 
//#define GPIO_AF_DCMI          ((uint8_t)0x0D)  /* DCMI Alternate Function mapping */

/** 
  * @brief   AF 14 selection  
  * GPIO_AF_14: USBDM, USBDP
  */ 

/** 
  * @brief   AF 15 selection  
  * GPIO_AF_15: OUT
  */ 
//#define GPIO_AF_EVENTOUT      ((uint8_t)0x0F)  /* EVENTOUT Alternate Function mapping */

#define IS_GPIO_AF(AF)   (((AF) == GPIO_AF_0)||((AF) == GPIO_AF_1)||\
                          ((AF) == GPIO_AF_2)||((AF) == GPIO_AF_3)||\
                          ((AF) == GPIO_AF_4)||((AF) == GPIO_AF_5)||\
                          ((AF) == GPIO_AF_6)||((AF) == GPIO_AF_7)||\
                          ((AF) == GPIO_AF_8)||((AF) == GPIO_AF_9)||\
                          ((AF) == GPIO_AF_10)||((AF) == GPIO_AF_11)||\
                          ((AF) == GPIO_AF_14)||((AF) == GPIO_AF_15))

/**
  * @}
  */ 

