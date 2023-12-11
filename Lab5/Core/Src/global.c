/*
 * global.c
 *
 *  Created on: Oct 30, 2023
 *      Author: Le HUy
 */

#include "global.h"
uint8_t temp = 0;
uint32_t ADC_value = 0;

uint8_t buffer [MAX_BUFFER_SIZE]={0};
uint8_t index_buffer=0;
uint8_t buffer_flag=0;

char str[MAX_BUFFER_SIZE] = {0};

int parser_status = 0;
int uart_status = 0;

int print_status = 0;
int send_data_status = 0;
//int check_command_status = 0;

void clear_buffer() {
	memset(buffer, 0, sizeof(buffer));
	index_buffer = 0;
}

void check_command() {
	if(buffer[0] == '!' && buffer[1] == 'R' && buffer[2] == 'S' && buffer[3] == 'T') {
		HAL_UART_Transmit(&huart2, (void*)str, sprintf(str, "\r\n"), 1000);
		print_status = 1;
		clear_buffer();
	}
	else if(buffer[0] == '!' && buffer[1] == 'O' && buffer[2] == 'K') {
		HAL_UART_Transmit(&huart2, (void*)str, sprintf(str, "\r\n"), 1000);
		send_data_status = 0;
		print_status = 0;
		clear_buffer();
	}
	else {
		print_status = 0;
		clear_buffer();
	}

	if(send_data_status == 1) {
		uart_status = SEND_DATA;
	}
}
