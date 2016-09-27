#include "commun.h"


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
