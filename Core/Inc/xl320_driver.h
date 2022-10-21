/*
 * xl320_driver.h
 *
 *  Created on: Oct 14, 2022
 *      Author: lucas
 */
#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "usart.h"

#define BIT_RESOLUTION_IN_DEG 0.29
#define RESOLUTION_SPEED 0.111
#define LIMIT_SPEED 114

/*
 * BAUD RATES
 */
#define BR_9600			0
#define BR_57600		1
#define BR_115200		2
#define BR_1M			3

/*
 * FRAME SIZE
 */
#define MIN_FRAME_SIZE 	10
#define CRC_FIELD_SIZE	2

/*
 * INSTRUCTIONS
 */
#define INSTR_WRITE 	0x03
#define INSTR_ACTION	0x05
#define INSTR_REBOOT 	0x08

/*
 * REGISTERS
 */
#define REG_LED			0x19
#define REG_POSITION 	0x1E


/*
 * LED COLORS
 */
typedef enum Color{
	Off,
	Red,
	Green,
	Yellow,
	Blue,
	Purple,
	Cyan,
	White
}Color;


typedef struct Xl320{
	UART_HandleTypeDef* uart;
	uint8_t id;
	uint8_t br;
} Xl320;

void xl320_init(Xl320* xl320, UART_HandleTypeDef* uart, uint8_t id, uint8_t br);

void xl320_addHeader2Buff(uint8_t* buff);

unsigned short xl320_updateCrc(unsigned short crc_accum, unsigned char *data_blk_ptr, unsigned short data_blk_size);

void xl320_copyParams2Buff(uint8_t buffStartIndex, uint8_t* buff, uint16_t nbParams, uint8_t* params);

void xl320_sendCommand(Xl320* xl320, uint8_t inst, uint16_t nbParams, uint8_t* params);

void xl320_reboot(Xl320* xl320);

void xl320_setLedColor(Xl320* xl320, Color color);

void xl320_setGoalPosition(Xl320* xl320, float goalPositionInDeg);

void xl320_setSpeed(Xl320* xl320, float rpm);

void xl320_blinbling(Xl320* xl320);

/*
void xl320_clearReceiveBuffer(uint8_t* buffer);

void xl320_addHeader(void);

void xl320_sendCommand(const uin8_t servoId, uint8_t* packet, uint16_t packetLength);

void xl320_ping(const uint8_t servoId);



//void xl320_led(const uint8_t servoID);

 */
