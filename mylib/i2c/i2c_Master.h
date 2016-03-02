#ifndef i2cMaster_H
#define i2cMaster_H

#define i2cRead 0x01
#define i2cWrite 0x00


void i2c_Init(void);
uint8_t i2c_Start(uint8_t slaveAddr);
uint8_t i2c_Stop();
uint8_t i2c_Write(uint8_t data);
uint8_t i2c_Transmit(uint8_t slaveAddr, uint8_t* data, uint16_t length);
uint8_t i2c_ReadAck(void);
uint8_t i2c_ReadNack(void);
uint8_t i2c_Receive(uint8_t slaveAddr, uint8_t* dataReceive, uint16_t length);

#endif 
			
