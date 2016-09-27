#ifndef COMMUN_H
#define COMMUN_H

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "GPIO_STM32F4xx.h"
#include "STM32F4xx.h"

#define cmd_length_max 64	// ATTENTION if more than 64, use for the comter uint16_t (or int) insted of uint8_t !!
#define STX 0x02
#define ETX 0x03

//uint8_t IRQ_main_connection(uint8_t from);


#endif
