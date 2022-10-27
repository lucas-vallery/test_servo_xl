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
 * OPTION FOR DEBUG
 */

#define DEBUG_XL320_ENABLE

#ifdef DEBUG_XL320_ENABLE
	#define DEBUG_PRINTF(x) printf(x)
#else
	#define DEBUG_PRINTF
#endif


/*
 * SERVO CONSTANTS
 */
#define BIT_RESOLUTION_IN_DEG 	0.29
#define RESOLUTION_SPEED 		0.111
#define LIMIT_SPEED 			114

#define GATE_CLOSED 			69

#define BUFFER_SIZE				32


/*
 * FRAME SIZE
 */
#define MIN_FRAME_SIZE 		10
#define CRC_FIELD_SIZE		2
#define ERR_FRAME_OFFSET	7
#define LEN_FRAME_OFFSET	5


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
}XL320_Instruction_t;


/*
 * ERRORS
 */
typedef enum error_struct{
	NO_ERROR			=	0x00,
	RESULT_FAIL			=	0x01,
	INSTR_ERROR			=	0x02,
	CRC_ERROR			=	0x03,
	DATA_RANGE_ERROR	=	0x04,
	DATA_LENGTH_ERROR	=	0x05,
	DATA_LIMIT_ERROR	=	0x06,
	ACCESS_ERROR		=	0x07
}XL320_Error_t;

/*
 * REGISTERS
 */
typedef enum register_struct{
	TORQUE_EN 	= 	0x18,
	LED 		= 	0x19,
	POSITION 	= 	0x1E
}XL320_Register_t;


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
}XL320_Color_t;

typedef int (* adxl345_transmit_t)(uint8_t *pData, uint16_t size, uint32_t timeout);
typedef int (* adxl345_receive_t)(uint8_t *pData, uint16_t size, uint32_t timeout);

typedef struct xl320_serial_struct{
	adxl345_transmit_t transmit;
	adxl345_receive_t receive;
}XL320_Serial_t;

/**
 * \struct XL320_t
 *
 * \brief XL320 object
 */
typedef struct XL320_t{
	uint8_t id; /*!< ID of the XL320. The default ID is 1*/
	uint8_t br; /*!< Baude rate of the XL320. The default baude rate is at 1MBaud/s*/

	XL320_Serial_t serial;

}XL320_t;


int xl320_init(XL320_t* xl320,uint8_t id, uint8_t br);

void xl320_addHeader2Buff(uint8_t* buff);

unsigned short xl320_updateCrc(unsigned short crc_accum, unsigned char *data_blk_ptr, unsigned short data_blk_size);

void xl320_copyParams2Buff(uint8_t buffStartIndex, uint8_t* buff, uint16_t nbParams, uint8_t* params);

int xl320_sendCommand(XL320_t* xl320, uint8_t inst, uint16_t nbParams, uint8_t* params);

int xl320_receiveCommand(XL320_t* xl320, uint8_t* rxBuff);

int xl320_checkErrorField(uint8_t errCode);

int xl320_check_crcField(uint8_t* buffer);

int xl320_reboot(XL320_t* xl320);

int xl320_setLedColor(XL320_t* xl320, XL320_Color_t color);

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
