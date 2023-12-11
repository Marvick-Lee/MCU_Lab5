/*
 * scheduler.c
 *
 *  Created on: Dec 7, 2023
 *      Author: Le HUy
 */

#include "main.h"
#include "scheduler.h"

void SCH_Init(){
	SCH_TASK_LIST.current_index_task = 0;
	for (int i = 0; i < SCH_MAX_TASKS; i++){
		SCH_TASK_LIST.TASK_LIST[i].Delay = 0;
		SCH_TASK_LIST.TASK_LIST[i].Period = 0;
		SCH_TASK_LIST.TASK_LIST[i].RunMe = 0;
		SCH_TASK_LIST.TASK_LIST[i].pTask = 0;
	}
}

void SCH_Add_Task(	void (*pFunction)(),
					uint32_t DELAY,
					uint32_t PERIOD){
	if(SCH_TASK_LIST.current_index_task < SCH_MAX_TASKS){
	sTasks temp;
	temp.pTask = pFunction;
	temp.Delay = DELAY / TIMER_CYCLE;
	temp.Period =  PERIOD / TIMER_CYCLE;
	temp.RunMe = 0;
	SCH_ListPush(temp);
	} else return;
}

void SCH_Dispatch_Tasks(void){
	for(int i = 0; i < SCH_MAX_TASKS; i++){
		if(SCH_TASK_LIST.TASK_LIST[i].RunMe > 0){
			(*SCH_TASK_LIST.TASK_LIST[i].pTask)();
			SCH_TASK_LIST.TASK_LIST[i].RunMe -=1;
			SCH_Delete_Task();
		}
	}
	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}

void SCH_Update(void){
	if (SCH_TASK_LIST.TASK_LIST[0].pTask == 0) return;
	else {
		if (SCH_TASK_LIST.TASK_LIST[0].Delay == 0) {
			SCH_TASK_LIST.TASK_LIST[0].RunMe +=1;
			if (SCH_TASK_LIST.TASK_LIST[0].Period) {
				SCH_TASK_LIST.TASK_LIST[0].Delay = SCH_TASK_LIST.TASK_LIST[0].Period;
			}
		}
		else SCH_TASK_LIST.TASK_LIST[0].Delay -=1;
	}
}

void SCH_Delete_Task () {
	int id = 0;
	int push_back = 0;
	sTasks temp;
	if (SCH_TASK_LIST.TASK_LIST[id].Period != 0) {
		push_back = 1;
		temp = SCH_TASK_LIST.TASK_LIST[id];
	}
	for (; id < SCH_TASK_LIST.current_index_task - 1; id++) {
		SCH_TASK_LIST.TASK_LIST[id] = SCH_TASK_LIST.TASK_LIST[id + 1];
	}

	SCH_TASK_LIST.TASK_LIST[id].pTask = 0x0000;
	SCH_TASK_LIST.TASK_LIST[id].Delay = 0;
	SCH_TASK_LIST.TASK_LIST[id].Period = 0;
	SCH_TASK_LIST.TASK_LIST[id].RunMe = 0;
	SCH_TASK_LIST.current_index_task -=1;

	if (push_back == 1) {
		SCH_ListPush(temp);
	}
}

void SCH_ListPush(sTasks task){
	if (SCH_TASK_LIST.current_index_task == 0) {
		SCH_TASK_LIST.TASK_LIST[0] = task;
		SCH_TASK_LIST.current_index_task += 1;
		return;
	} else {
		int id = 0;
		int prev = SCH_TASK_LIST.TASK_LIST[0].Delay;
		int timerDelay = SCH_TASK_LIST.TASK_LIST[0].Delay;

		while (id < SCH_TASK_LIST.current_index_task && task.Delay > timerDelay) {
			id++;
			if (id < SCH_TASK_LIST.current_index_task) {
				prev = timerDelay;
				timerDelay += SCH_TASK_LIST.TASK_LIST[id].Delay;
			}
		}

		if (id == SCH_TASK_LIST.current_index_task) {
			task.Delay -= timerDelay;
			SCH_TASK_LIST.TASK_LIST[id] = task;
			SCH_TASK_LIST.current_index_task +=1;
			return;
		} else {
			for (int i = SCH_TASK_LIST.current_index_task; i > id; i--) {
				SCH_TASK_LIST.TASK_LIST[i] = SCH_TASK_LIST.TASK_LIST[i - 1];
			}
			if (id == 0) {
				SCH_TASK_LIST.TASK_LIST[id] = task;
				timerDelay = 0;
				prev = task.Delay;
			} else {
				task.Delay -= prev;
				timerDelay = prev;
				prev += task.Delay;
				SCH_TASK_LIST.TASK_LIST[id] = task;
			}
			timerDelay += SCH_TASK_LIST.TASK_LIST[id + 1].Delay;
			SCH_TASK_LIST.TASK_LIST[id + 1].Delay = timerDelay - prev;
			SCH_TASK_LIST.current_index_task +=1;
		}
	}
}
