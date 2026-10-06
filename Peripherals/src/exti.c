/*
 * exti.c
 *
 *  Created on: Aug 14, 2026
 *      Author: harineshvijay
 */


#include "exti.h"



void exti_set_source(exti_port_e port, gpio_num_e pin){

  SYSCFG->EXTICR[pin >> 2] &= ~(0xF << ((pin % 4)*4));
  SYSCFG->EXTICR[pin >> 2] |= (port << ((pin % 4)*4));

}

void exti_set_trigger_edge(exti_source_e source, exti_trigger_e trigger){

  if (trigger == EXTI_RISING_EDGE){

      EXTI->RTSR |= (1 << source);
      EXTI->FTSR &= ~(1 << source);
  }
  if (trigger == EXTI_FALLING_EDGE){

      EXTI->FTSR |= (1 << source);
      EXTI->RTSR &= ~(1 << source);
  }
   if (trigger == EXTI_RISING_AND_FALLING){

       EXTI->RTSR |= (1 << source);
       EXTI->FTSR |= (1 << source);
   }
}

void exti_enable_irq(exti_source_e source, IRQn_Type irq_num){
  EXTI->IMR |= (1 << source);
  NVIC_EnableIRQ(irq_num);
}

void exti_disable_irq(exti_source_e source, IRQn_Type irq_num)
{

  EXTI->EMR |= (1 << source);
  NVIC_DisableIRQ(irq_num);
}
