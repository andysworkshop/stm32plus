/**
  ******************************************************************************
  * @file    system_stm32f0xx.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   CMSIS Cortex-M0 Device Peripheral Access Layer System Source File.
  *          This file contains the system clock configuration for STM32F0xx devices,
  *          and is customized for use with STM32F0-DISCOVERY Kit.
  *          The STM32F0xx is configured to run at 48 MHz, following the three
  *          configuration below:
  *            - PLL_SOURCE_HSI (default): HSI (~8MHz) used to clock the PLL, and
  *                                        the PLL is used as system clock source.
  *            - PLL_SOURCE_HSE          : HSE (8MHz) used to clock the PLL, and
  *                                        the PLL is used as system clock source.
  *            - PLL_SOURCE_HSE_BYPASS   : HSE bypassed with an external clock
  *                                        (8MHz, coming from ST-Link) used to clock
  *                                        the PLL, and the PLL is used as system
  *                                        clock source.
  *
  *
  * 1.  This file provides two functions and one global variable to be called from
  *     user application:
  *      - SystemInit(): Setups the system clock (System clock source, PLL Multiplier
  *                      and Divider factors, AHB/APBx prescalers and Flash settings),
  *                      depending on the configuration selected (see above).
  *                      This function is called at startup just after reset and
  *                      before branch to main program. This call is made inside
  *                      the "startup_stm32f0xx.s" file.
  *
  *      - SystemCoreClock variable: Contains the core clock (HCLK), it can be used
  *                                  by the user application to setup the SysTick
  *                                  timer or configure other parameters.
  *
  *      - SystemCoreClockUpdate(): Updates the variable SystemCoreClock and must
  *                                 be called whenever the core clock is changed
  *                                 during program execution.
  *
  * 2. After each device reset the HSI (8 MHz Range) is used as system clock source.
  *    Then SystemInit() function is called, in "startup_stm32f0xx.s" file, to
  *    configure the system clock before to branch to main program.
  *
  * 3. If the system clock source selected by user fails to startup, the SystemInit()
  *    function will do nothing and HSI still used as system clock source. User can
  *    add some code to deal with this issue inside the SetSysClock() function.
  *
  * 4. The default value of HSE crystal is set to 8MHz, refer to "HSE_VALUE" define
  *    in "stm32f0xx.h" file. When HSE is used as system clock source, directly or
  *    through PLL, and you are using different crystal you have to adapt the HSE
  *    value to your own configuration.
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32f0xx_system
  * @{
  */

/** @addtogroup STM32F0xx_System_Private_Includes
  * @{
  */

#include "config/stdperiph.h"
#include "usb/f0/UsbDefs.h"

/**
  * @}
  */

/** @addtogroup STM32F0xx_System_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32F0xx_System_Private_Defines
  * @{
  */
/* Select the PLL clock source */

#define PLL_SOURCE_HSI        // HSI (~8MHz) used to clock the PLL, and the PLL is used as system clock source
//#define PLL_SOURCE_HSE        // HSE (8MHz) used to clock the PLL, and the PLL is used as system clock source
//#define PLL_SOURCE_HSE_BYPASS // HSE bypassed with an external clock (8MHz, coming from ST-Link) used to clock
                              // the PLL, and the PLL is used as system clock source

/**
  * @}
  */

/** @addtogroup STM32F0xx_System_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32F0xx_System_Private_Variables
  * @{
  */
uint32_t SystemCoreClock    = 48000000;
__I uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};

/**
  * @}
  */

/** @addtogroup STM32F0xx_System_Private_FunctionPrototypes
  * @{
  */

static void SetSysClock(void);

/**
  * @}
  */

/** @addtogroup STM32F0xx_System_Private_Functions
  * @{
  */

/**
  * @brief  Setup the microcontroller system.
  *         Initialize the Embedded Flash Interface, the PLL and update the
  *         SystemCoreClock variable.
  * @param  None
  * @retval None
  */
void SystemInit (void)
{
  /* Set HSION bit */
  RCC->CR |= (uint32_t)0x00000001;

  /* Reset SW[1:0], HPRE[3:0], PPRE[2:0], ADCPRE and MCOSEL[2:0] bits */
  RCC->CFGR &= (uint32_t)0xF8FFB80C;

  /* Reset HSEON, CSSON and PLLON bits */
  RCC->CR &= (uint32_t)0xFEF6FFFF;

  /* Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /* Reset PLLSRC, PLLXTPRE and PLLMUL[3:0] bits */
  RCC->CFGR &= (uint32_t)0xFFC0FFFF;

  /* Reset PREDIV1[3:0] bits */
  RCC->CFGR2 &= (uint32_t)0xFFFFFFF0;

  /* Reset USARTSW[1:0], I2CSW, CECSW and ADCSW bits */
  RCC->CFGR3 &= (uint32_t)0xFFFFFEAC;

  /* Reset HSI14 bit */
  RCC->CR2 &= (uint32_t)0xFFFFFFFE;

  /* Disable all interrupts */
  RCC->CIR = 0x00000000;

  /* Configure the System clock frequency, AHB/APBx prescalers and Flash settings */
  SetSysClock();
}

/**
  * @brief  Update SystemCoreClock according to Clock Register Values
  *         The SystemCoreClock variable contains the core clock (HCLK), it can
  *         be used by the user application to setup the SysTick timer or configure
  *         other parameters.
  *
  * @note   Each time the core clock (HCLK) changes, this function must be called
  *         to update SystemCoreClock variable value. Otherwise, any configuration
  *         based on this variable will be incorrect.
  *
  * @note   - The system frequency computed by this function is not the real
  *           frequency in the chip. It is calculated based on the predefined
  *           constant and the selected clock source:
  *
  *           - If SYSCLK source is HSI, SystemCoreClock will contain the HSI_VALUE(*)
  *
  *           - If SYSCLK source is HSE, SystemCoreClock will contain the HSE_VALUE(**)
  *
  *           - If SYSCLK source is PLL, SystemCoreClock will contain the HSE_VALUE(**)
  *             or HSI_VALUE(*) multiplied/divided by the PLL factors.
  *
  *         (*) HSI_VALUE is a constant defined in stm32f0xx.h file (default value
  *             8 MHz) but the real value may vary depending on the variations
  *             in voltage and temperature.
  *
  *         (**) HSE_VALUE is a constant defined in stm32f0xx.h file (default value
  *              8 MHz), user has to ensure that HSE_VALUE is same as the real
  *              frequency of the crystal used. Otherwise, this function may
  *              have wrong result.
  *
  *         - The result of this function could be not correct when using fractional
  *           value for HSE crystal.
  * @param  None
  * @retval None
  */
void SystemCoreClockUpdate (void)
{
  uint32_t tmp = 0, pllmull = 0, pllsource = 0, prediv1factor = 0;

  /* Get SYSCLK source -------------------------------------------------------*/
  tmp = RCC->CFGR & RCC_CFGR_SWS;

  switch (tmp)
  {
    case 0x00:  /* HSI used as system clock */
      SystemCoreClock = HSI_VALUE;
      break;
    case 0x04:  /* HSE used as system clock */
      SystemCoreClock = HSE_VALUE;
      break;
    case 0x08:  /* PLL used as system clock */
      /* Get PLL clock source and multiplication factor ----------------------*/
      pllmull = RCC->CFGR & RCC_CFGR_PLLMULL;
      pllsource = RCC->CFGR & RCC_CFGR_PLLSRC;
      pllmull = ( pllmull >> 18) + 2;

      if (pllsource == 0x00)
      {
        /* HSI oscillator clock divided by 2 selected as PLL clock entry */
        SystemCoreClock = (HSI_VALUE >> 1) * pllmull;
      }
      else
      {
        prediv1factor = (RCC->CFGR2 & RCC_CFGR2_PREDIV1) + 1;
        /* HSE oscillator clock selected as PREDIV1 clock entry */
        SystemCoreClock = (HSE_VALUE / prediv1factor) * pllmull;
      }
      break;
    default: /* HSI used as system clock */
      SystemCoreClock = HSI_VALUE;
      break;
  }
  /* Compute HCLK clock frequency ----------------*/
  /* Get HCLK prescaler */
  tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];
  /* HCLK clock frequency */
  SystemCoreClock >>= tmp;
}

/**
  * @brief  Configures the System clock frequency, AHB/APBx prescalers and Flash
  *         settings.
  * @note   This function should be called only once the RCC clock configuration
  *         is reset to the default reset state (done in SystemInit() function).
  * @param  None
  * @retval None
  */
static void SetSysClock(void)
{
  // enable flash prefetch buffer

  FLASH->ACR |= FLASH_ACR_PRFTBE;

  // enable HSI48

  RCC->CR2 |= RCC_CR2_HSI48ON;
  while((RCC->CR2 & RCC_CR2_HSI48RDY)==0);

  // disable the PLL

  RCC->CR &=~ RCC_CR_PLLON;
  while((RCC->CR & RCC_CR_PLLRDY)!=0);

  // select HSI48 as the USB clock source

  RCC->CFGR3 = (RCC->CFGR3 &~ RCC_CFGR3_USBSW) | RCC_CFGR3_USBSW_HSI48;

  // set flash latency = 1

  FLASH->ACR = (FLASH->ACR &~FLASH_ACR_LATENCY) | FLASH_Latency_1;

  // AHB

  RCC->CFGR = (RCC->CFGR &~ RCC_CFGR_HPRE) | RCC_CFGR_HPRE_DIV1;

  // HCLK source = HSI48

  RCC->CFGR = (RCC->CFGR &~ RCC_CFGR_SW) | RCC_CFGR_SW_HSI48;
  while((RCC->CFGR & RCC_CFGR_SWS)!=RCC_CFGR_SWS_HSI48);

  // PCLK1

  RCC->CFGR = (RCC->CFGR &~ RCC_CFGR_PPRE) | RCC_CFGR_PPRE_DIV1;

  // enable clock recovery system from USB SOF frames

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CRS,ENABLE);

  // Before configuration, reset CRS registers to their default values

  RCC->APB1RSTR |= RCC_APB1RSTR_CRSRST;
  RCC->APB1RSTR &=~ RCC_APB1RSTR_CRSRST;

  // Configure Synchronization input */
  // Clear SYNCDIV[2:0], SYNCSRC[1:0] & SYNCSPOL bits */

  CRS->CFGR &= ~(CRS_CFGR_SYNCDIV | CRS_CFGR_SYNCSRC | CRS_CFGR_SYNCPOL);

  // Set the CRS_CFGR_SYNCDIV[2:0] bits according to Prescaler value
  // CRS->CFGR |= 0;

  // Set the SYNCSRC[1:0] bits according to Source value

  CRS->CFGR |= CRS_CFGR_SYNCSRC_1;

  // Set the SYNCSPOL bits according to Polarity value
  // CRS->CFGR |= 0;

  // Configure Frequency Error Measurement
  // Clear RELOAD[15:0] & FELIM[7:0] bits

  CRS->CFGR &= ~(CRS_CFGR_RELOAD | CRS_CFGR_FELIM);

  // Set the RELOAD[15:0] bits according to ReloadValue value

  CRS->CFGR |= 47999;     // (48MHz/1000) -1

  // Set the FELIM[7:0] bits according to ErrorLimitValue value

  CRS->CFGR |= (0x22 << 16);

  // Adjust HSI48 oscillator smooth trimming
  // Clear TRIM[5:0] bits

  CRS->CR &= ~CRS_CR_TRIM;

  // Set the TRIM[5:0] bits according to RCC_CRS_HSI48CalibrationValue value

  CRS->CR |= (0x20 << 8);

  // Enable Automatic trimming

  CRS->CR |= CRS_CR_AUTOTRIMEN;

  // Enable Frequency error counter

  CRS->CR |= CRS_CR_CEN;

  // Remap PA11-12 to PA9-10 for USB

  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;
  SYSCFG->CFGR1 |= SYSCFG_CFGR1_PA11_PA12_RMP;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
