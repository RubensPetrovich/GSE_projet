#include "commun.h"

extern char commande[cmd_length_max];
extern uint8_t command_sent;

/*
uint8_t IRQ_main_connection(uint8_t from) {
	if (from == 0) { // IRQ Call 
		command_sent = 1;
	} else {	// main call
		if (command_sent) {
			command_sent = 0;
			return 0;
		}
	}
	return 1;
}
*/
