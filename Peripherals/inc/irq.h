/*
 * irq.h
 *
 *  Created on: Aug 6, 2026
 *      Author: harineshvijay
 *
 *      it contain the IRQ priority definition and function prototype
 *
 */

#ifndef INC_IRQ_H_
#define INC_IRQ_H_

#include "mcu.h"
#include "FreeRTOSConfig.h"


//ADC intrrupt priority critical for temperature sensor
#define IRQ_ADC_PRIORITY   ((uint32_t) (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 0))

//External interrupt priority for the user button
#define IRO_EXTI15_10_PRIORITY  ((uint32_t) (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 0))

/*
 * config the priority level of the various IRQ's  in the system by setting the
 * priority level in the NVIC for specific interrupt
 */

void irq_set_priorities(void);



#endif /* INC_IRQ_H_ */
