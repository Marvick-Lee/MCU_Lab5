/*
 * command_parser_fsm.c
 *
 *  Created on: Nov 24, 2023
 *      Author: Le HUy
 */

#include "global.h"
#include "command_parser_fsm.h"

void command_parser_fsm() {
	switch(parser_status) {
	case START_PARSER:
		if(temp == '!') {
			parser_status = RECEIVE_COMMAND;
			uart_status = WAIT_COMMAND;
		}
		else clear_buffer();
		break;

	case RECEIVE_COMMAND:
		if(temp == '#') {
			check_command();
			parser_status = START_PARSER;
		}
		else if(temp == '!') {
			clear_buffer();
		}
		break;

	default:
		break;
	}

}
