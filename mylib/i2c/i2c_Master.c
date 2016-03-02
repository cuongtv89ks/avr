#ifndef F_CPU
#define FCPU 16000000UL //16MHz
#endif

#include <avr/io.h>
#include <util/twi.h>

#include "i2c_Master.h"

#define F_SCL 100000UL //SCL = 100MHz
#define Prescaler 1
#define TWBR_Val ((((F_CPU/F_SCL)-16 )/ Prescaler) /2)

void i2c_Init(void){
	TWBR = (uint8_t) TWBR_Val; //set Baud Rate
}

uint8_t i2c_Start(uint8_t slaveAddr){
	// reset TWINT control register
	TWCR = 0;
	// transmit Start Condition
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	// wait for end of transmission
	while (!(TWCR & (1<<TWINT)));
	
	// check if the start condition was successfully transmitted
	if ((TWSR & 0xF8) != TW_START)
	return 1;
	
	// load slave address into data register
	TWDR = slaveAddr;
	// start transmission of address
	TWCR = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	while(!(TWCR & (1<<TWINT)));
	
	//check if the device has ackknowledge the READ/ WRITE mode
	if( ( (TWSR & 0xF8) != TW_MT_SLA_ACK) && ((TWSR & 0xF8) != TW_MR_SLA_ACK) )
	return 1;
	
	return 0;
}

uint8_t i2c_Write(uint8_t data){
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while( !(TWCR & (1<<TWINT)));
	
	if ((TWSR & 0xF8) != TW_MT_DATA_ACK)
	return 1;
	return 0;
}

uint8_t i2c_Transmit(uint8_t slaveAddr, uint8_t* data, uint16_t length){
	if (i2c_Start(slaveAddr | i2cWrite))
	return 1;
	for(uint16_t i = 0; i < length; i++){
		if (i2c_Write(data[i]))
		return 1;
	}
	i2c_Stop();
	return 0;
}

uint8_t i2c_ReadAck(){
	// start TWI module and acknowledge data after reception
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<TWEA);
	// wait for ending of transission
	while ( !(TWCR & (1<<TWINT)));
	// return received data from TWDR
	return TWDR;
}

uint8_t i2c_ReadNack(){
	// start receiving without acknowledging reception
	TWCR = (1<< TWINT) | (1<<TWEN);
	while( !(TWCR & (1<<TWINT)));
	return TWDR;
}

uint8_t i2c_Receive(uint8_t slaveAddr, uint8_t* dataReceive, uint16_t length){
	if (i2c_Start(slaveAddr | i2cRead))
	return 0;
	for(uint8_t i = 0; i<length-1; i++){
		data[i] = i2c_ReadAck();
	}
	data[length-1]  = i2c_ReadNack();
	
	i2c_Stop();
	
	return 0;
}

