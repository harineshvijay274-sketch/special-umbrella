/*
 * FreeRTOSasks.h
 *This contain the definition of the FreeRtos tasks, prioritizing their stack size and executing the priorition
 *This file contain the Task configuation of the ease managment nd calirity in overall system design
 *  Created on: 16.07.2026
 *      Author: harineshvijay
 */

#ifndef FREERTOSTASKS_H_
#define FREERTOSTASKS_H_

#include "FreeRTOS.h"
#include "task.h"


/**
 * The startup task is designed to initialize system components and
 * facilitate the transition to normal operational tasks. The stack size
 * of 512 is chosen for prototyping purposes and should be optimized based
 * on the actual requirements of the initialization routines. It is given
 * high priority (the highest among the tasks defined here) to ensure swift
 * system initialization. However, the task deletes itself upon completion.
 */

#define STARTUP_TASK_STACK_SIZE           (512)
#define STARTUP_TASK_PRIORITY             (configMAX_PRIORITIES - 1)



/**
 * The Handler Task handles system errors.
 * Messages are sent to this task for error handling actions, which can be expanded/customized as needed.
 * The task has a high priority due to the need for timely error detection.
 * Stack size is set at 256 for prototyping, optimize later based on usage.
 */
#define ERROR_HANDLER_TASK_STACK_SIZE     (256)
#define ERROR_HANDLER_TASK_PRIORITY       (configMAX_PRIORITIES - 2)


/*
The system Health Monitor Task oversees the overall health of the system.
Currently, this includes the Monitoring the temperature and
checking the state of critical tasks and resetting the watchdog time they are operational
The tack the low priority to ensure that higher priority task get the CPU time first
preventing them from being starved and causing a system reset if they fail to update
 Stack size is 256 for prototyping optimize later based on needs
*/

#define SYS_HEALTH_MONITOR_TASK_STACK_SIZE      (256)
#define SYS_HEALTH_MONITOR_TASK_PRIORITY        (configMAX_PRIORITIES - 4)


#endif /* FREERTOSTASKS_H_ */






