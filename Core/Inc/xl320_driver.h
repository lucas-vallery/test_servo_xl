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
#include "uart_half_duplex_driver.h"

/*
 * SERVO CONSTANTS
 */
#define BIT_RESOLUTION_IN_DEG 	0.29
#define RESOLUTION_SPEED 		0.111
#define LIMIT_SPEED 			114

#define GATE_CLOSED 			69


/*
 * FRAME SIZE
 */
#define MIN_FRAME_SIZE 	10
#define CRC_FIELD_SIZE	2


/*
 * BAUD RATES
 */
typedef enum baudRate_struct{
	BR_9600 	=	0,
	BR_57600 	=	1,
	BR_115200 	= 	2,
	BR_1M 		=	3
}XL320_BaudRate_t;


/*
 * INSTRUCTIONS
 */
typedef enum instruction_struct{
	WRITE 		= 	0x03,
	REG_WRITE 	= 	0x04,
	ACTION 		= 	0x05,
	REBOOT 		= 	0x08
}Instruction_t;


/*
 * REGISTERS
 */
typedef enum register_struct{
	TORQUE_EN 	= 	0x18,
	LED 		= 	0x19,
	POSITION 	= 	0x1E
}Register_t;


/*
 * LED COLORS
 */
typedef enum color_struct{
	Off,
	Red,
	Green,
	Yellow,
	Blue,
	Purple,
	Cyan,
	White
}Color_t;

typedef int (* adxl345_transmit_t)(uint8_t *pData, uint16_t size, uint32_t timeout);
typedef int (* adxl345_receive_t)(uint8_t *pData, uint16_t size, uint32_t timeout);

typedef struct xl320_serial_struct{
	adxl345_transmit_t transmit;
	adxl345_receive_t receive;
}xl320_serial_t;

typedef struct XL320_struct{
	uint8_t id;
	uint8_t br;

	xl320_serial_t serial;

}XL320_t;


int xl320_init(XL320_t* xl320,uint8_t id, uint8_t br);

void xl320_addHeader2Buff(uint8_t* buff);

unsigned short xl320_updateCrc(unsigned short crc_accum, unsigned char *data_blk_ptr, unsigned short data_blk_size);

void xl320_copyParams2Buff(uint8_t buffStartIndex, uint8_t* buff, uint16_t nbParams, uint8_t* params);

int xl320_sendCommand(XL320_t* xl320, uint8_t inst, uint16_t nbParams, uint8_t* params);

int xl320_reboot(XL320_t* xl320);

int xl320_setLedColor(XL320_t* xl320, Color_t color);

int xl320_setGoalPosition(XL320_t* xl320, float goalPositionInDeg);

int xl320_setSpeed(XL320_t* xl320, float rpm);

int xl320_executeAction(XL320_t* xl320);

int xl320_torqueEnable(XL320_t* xl320);

int xl320_blinbling(XL320_t* xl320);

/*
void xl320_clearReceiveBuffer(uint8_t* buffer);

void xl320_addHeader(void);

void xl320_sendCommand(const uin8_t servoId, uint8_t* packet, uint16_t packetLength);

void xl320_ping(const uint8_t servoId);



//void xl320_led(const uint8_t servoID);

 */
