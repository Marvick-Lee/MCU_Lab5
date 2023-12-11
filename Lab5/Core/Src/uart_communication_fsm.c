/*
 * uart_communication_fsm.c
 *
 *  Created on: Nov 24, 2023
 *      Author: Le HUy
 */

#include "global.h"
#include "command_parser_fsm.h"
#include "uart_communication_fsm.h"

void uart_communication_fsm () {
	switch(uart_status) {
	case WAIT_COMMAND:
		if(print_status == 1) {
			uart_status = SEND_DATA;
			setTimer2(1500);
			print_status = 0;
		}
		break;

	case SEND_DATA:
		if(timer2_flag == 1) {
			setTimer2(1500);
			send_data_status = 1;
			ADC_value = HAL_ADC_GetValue(&hadc1);
			HAL_UART_Transmit(&huart2, (uint8_t *)str, sprintf(str , "!ADC=%lu#\r\n" ,ADC_value), 1000);
		}
		break;

	default:
			break;
	}

}
