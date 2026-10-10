/*
 * config the priority level of the various IRQ's  in the system by setting the
 * priority level in the NVIC for specific interrupt
 */

#include "mcu.h"
#include "irq.h"

void irq_set_priorities(void){

  NVIC_SetPriority(ADC_IRQn, IRQ_ADC_PRIORITY);

  NVIC_SetPriority(EXTI15_10_IRQn, IRO_EXTI15_10_PRIORITY);
}
