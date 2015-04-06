#include <stdint.h>
#include "fwlib/f4/cmsis/Device/ST/STM32F4xx/Include/stm32f4xx.h"


void __attribute__((naked)) HardFault_Handler(void) {
  __asm volatile
  (
      " tst lr, #4                                                \n"
      " ite eq                                                    \n"
      " mrseq r0, msp                                             \n"
      " mrsne r0, psp                                             \n"
      " ldr r1, [r0, #24]                                         \n"
      " ldr r2, handler2_address_const                            \n"
      " bx r2                                                     \n"
      " handler2_address_const: .word prvGetRegistersFromStack    \n"
  );
}

void prvGetRegistersFromStack(uint32_t *pulFaultStackAddress) {

  volatile uint32_t r0 __attribute__((unused));
  volatile uint32_t r1 __attribute__((unused));
  volatile uint32_t r2 __attribute__((unused));
  volatile uint32_t r3 __attribute__((unused));
  volatile uint32_t r12 __attribute__((unused));
  volatile uint32_t lr __attribute__((unused));     /* Link register. */
  volatile uint32_t pc __attribute__((unused));     /* Program counter. */
  volatile uint32_t psr __attribute__((unused));    /* Program status register. */

  r0=pulFaultStackAddress[0];
  r1=pulFaultStackAddress[1];
  r2=pulFaultStackAddress[2];
  r3=pulFaultStackAddress[3];

  r12=pulFaultStackAddress[4];
  lr=pulFaultStackAddress[5];
  pc=pulFaultStackAddress[6];
  psr=pulFaultStackAddress[7];

  /* When the following line is hit, the variables contain the register values. */
  for(;;)
    ;
}
