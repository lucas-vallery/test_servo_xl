/*
 * xl320_driver.c
 *
 *  Created on: Oct 14, 2022
 *      Author: lucas
 */

#include "xl320_driver.h"

void xl320_init(Xl320* xl320, UART_HandleTypeDef* uart, uint8_t id, uint8_t br){
	xl320->uart = uart;
	xl320->id 	= id;
	xl320->br	= br;
}

void xl320_addHeader2Buff(uint8_t* buff){
	buff[0] = 0xFF;
	buff[1] = 0xFF;
	buff[2] = 0xFD;
	buff[3] = 0x00;
}

unsigned short xl320_updateCrc(unsigned short crc_accum, unsigned char *data_blk_ptr, unsigned short data_blk_size) {
	unsigned short i, j;
	unsigned short crc_table[256] = {
			0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
			0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
			0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,
			0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041,
			0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2,
			0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
			0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1,
			0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
			0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,
			0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1,
			0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1,
			0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2,
			0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151,
			0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162,
			0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
			0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101,
			0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312,
			0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
			0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371,
			0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342,
			0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,
			0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
			0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2,
			0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381,
			0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291,
			0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2,
			0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,
			0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1,
			0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
			0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261,
			0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231,
			0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202
	};

	for(j = 0; j < data_blk_size; j++) {
		i = ((unsigned short)(crc_accum >> 8) ^ data_blk_ptr[j]) & 0xFF;
		crc_accum = (crc_accum << 8) ^ crc_table[i];
	}

	return crc_accum;
}

void xl320_copyParams2Buff(uint8_t buffStartIndex, uint8_t* buff, uint16_t nbParams, uint8_t* params){
	for(int i = 0; i < nbParams; i++){
		buff[i + buffStartIndex] = params[i];
	}
}

void xl320_sendCommand(Xl320* xl320, uint8_t inst, uint16_t nbParams, uint8_t* params){
	uint8_t* txBuff = NULL;
	txBuff = (uint8_t*) malloc((MIN_FRAME_SIZE + nbParams)*sizeof(uint8_t));
	uint16_t length = nbParams + 3;

	xl320_addHeader2Buff(txBuff);
	txBuff[4] = xl320->id;
	txBuff[5] = (uint8_t) (length & 0xFF);
	txBuff[6] = (uint8_t) (length >> 8);
	txBuff[7] = inst;
	xl320_copyParams2Buff(8, txBuff, nbParams, params);

	uint16_t crc = xl320_updateCrc(0, txBuff, MIN_FRAME_SIZE + nbParams - CRC_FIELD_SIZE);

	txBuff[(MIN_FRAME_SIZE + nbParams) - 2] = (uint8_t) (crc & 0xFF);
	txBuff[(MIN_FRAME_SIZE + nbParams) - 1] = (uint8_t) (crc >> 8);

	HAL_HalfDuplex_EnableTransmitter(&huart6);
	HAL_UART_Transmit(xl320->uart, txBuff, (MIN_FRAME_SIZE + nbParams)*sizeof(uint8_t), HAL_MAX_DELAY);

	free(txBuff);
}

void xl320_reboot(Xl320* xl320){
	xl320_sendCommand(xl320, INSTR_REBOOT, 0, NULL);
}

void xl320_setLedColor(Xl320* xl320, Color color){
	uint8_t params[3] = {REG_LED, 0, (uint8_t) color};

	xl320_sendCommand(xl320, INSTR_WRITE, 3, (uint8_t*) &params);
}

void xl320_setGoalPosition(Xl320* xl320, float goalPositionInDeg){
	uint16_t position = (uint16_t)(goalPositionInDeg/BIT_RESOLUTION_IN_DEG);
	uint8_t params[3] = {REG_POSITION, (uint8_t)(position & 0xFF) , (uint8_t)(position >> 8)};

	xl320_sendCommand(xl320, INSTR_WRITE, 3, (uint8_t*) &params);
}

void xl320_setSpeed(Xl320* xl320, float rpm){
	uint16_t speedValue = (uint16_t)(rpm/RESOLUTION_SPEED);
	uint8_t highByte = (uint8_t)((speedValue >> 8) & 0xFF);
	uint8_t lowByte = (uint8_t)(speedValue & 0xFF);

	uint8_t params[3] = {LIMIT_SPEED, lowByte, highByte};
	xl320_sendCommand(xl320, INSTR_WRITE, 3, (uint8_t*) &params);
}

void xl320_executeAction(Xl320* xl320){
	xl320_sendCommand(xl320, INSTR_ACTION, 0, NULL);
}

void xl320_torqueEnable(Xl320* xl320){
	uint8_t params[2] = {REG_TORQUE_EN, ENABLE};

	xl320_sendCommand(xl320, INSTR_WRITE, 2, (uint8_t*) &params);
}

void xl320_blinbling(Xl320* xl320){
	xl320_setLedColor(xl320, Off);
	HAL_Delay(100);
	xl320_setLedColor(xl320, Red);
	HAL_Delay(100);
	xl320_setLedColor(xl320, Green);
	HAL_Delay(100);
	xl320_setLedColor(xl320, Yellow);
	HAL_Delay(100);
	xl320_setLedColor(xl320, Blue);
	HAL_Delay(100);
	xl320_setLedColor(xl320, Purple);
	HAL_Delay(100);
	xl320_setLedColor(xl320, Cyan);
	HAL_Delay(100);
	xl320_setLedColor(xl320, White);
	HAL_Delay(100);
}


/*
void xl320_clearReceiveBuffer(uint8_t* buffer) {
	int i;
	for(i = 0; i < 32; i++){
		buffer[i] = 0;
	}
}


void xl320_sendCommand(const uin8_t servoId, uint8_t* packet, uint16_t packetLength) {
}

void xl320_addHeader(){
	uint8_t headerPacket[] = {0xFF, 0xFF, 0xFD};
}

void xl320_sendCommand(const uint8_t servoId, uint8_t* instructionPacket) {

}

void xl320_ping(const uint8_t servoId) {
	uint8_t pingPacket[] = {0xFF, 0xFF, 0xFD, 0x00, servoId, 0x03, 0x00, 0x01, 0x0, 0x0};
	unsigned short crc = xl320_updateCrc(0, pingPacket, 8);
	uint8_t receiveBuffer[32];
	xl320_clearReceiveBuffer(receiveBuffer);

	pingPacket[8] = (uint8_t) (crc & 0xFF);
	pingPacket[9] = (uint8_t) (crc >> 8);

	HAL_HalfDuplex_EnableTransmitter(&huart6);
	HAL_UART_Transmit(&huart6, pingPacket, 10*sizeof(char), 0x1F4);
	printf("sent\r\n");

	HAL_HalfDuplex_EnableReceiver(&huart6);
	HAL_UART_Receive(&huart6, receiveBuffer, 32, 0x1F4);
	int i;
	if(receiveBuffer[0] != 0) {
		for(i= 0; i < 32; i++){
			printf("Received : %d\r\n", receiveBuffer[i]);
		}
	}
}
 */
