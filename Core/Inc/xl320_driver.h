/*
 * xl320_driver.h
 *
 *  Created on: Oct 14, 2022
 *      Author: lucas
 */
#pragma once

#include <stdio.h>

#include "main.h"
#include "usart.h"


unsigned short xl320_updateCrc(unsigned short crc_accum, unsigned char *data_blk_ptr, unsigned short data_blk_size);

void clearReceiveBuffer(uint8_t* buffer);

void xls320_addHeader(void);

void xl320_sendCommand(const uin8_t servoId, uint8_t* packet, uint16_t packetLength);

void xl320_ping(const uint8_t servoId);



//void xl320_led(const uint8_t servoID);
