/*
 * scheduler.h
 *
 *  Created on: Dec 7, 2023
 *      Author: Le HUy
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include <stdint.h>
#include "global.h"

#define SCH_MAX_TASKS 40
#define TIMER_CYCLE 10
typedef struct{
	void (*pTask)(void);
	uint32_t Delay;
	uint32_t Period;
	uint8_t RunMe; //timerflag

	uint32_t TaskID;
} sTasks;

typedef struct {
	sTasks TASK_LIST[SCH_MAX_TASKS];
	uint8_t current_index_task;
} taskList;

taskList SCH_TASK_LIST;

void SCH_Init();
void SCH_ListPush(sTasks task);
void SCH_Add_Task(	void (*pFunction)(),
					uint32_t DELAY,
					uint32_t PERIOD);
void SCH_Dispatch_Tasks(void);
void SCH_Update(void);
void SCH_Delete_Task(void);

#endif /* INC_SCHEDULER_H_ */
