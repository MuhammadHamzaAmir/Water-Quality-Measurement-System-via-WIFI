/*
 * ds18b20.h
 *
 * Created: 6/9/2020 10:01:47 PM
 *  Author: Hamza-Murtaza
 */ 


#ifndef DS18B20_H_
#define DS18B20_H_
#include <avr/io.h>

//setup connection
#define DS18B20_PORT PORTE
#define DS18B20_DDR DDRE
#define DS18B20_PIN PINE
#define DS18B20_DQ PE6

//commands
#define DS18B20_CMD_CONVERTTEMP 0x44
#define DS18B20_CMD_RSCRATCHPAD 0xbe
#define DS18B20_CMD_WSCRATCHPAD 0x4e
#define DS18B20_CMD_CPYSCRATCHPAD 0x48
#define DS18B20_CMD_RECEEPROM 0xb8
#define DS18B20_CMD_RPWRSUPPLY 0xb4
#define DS18B20_CMD_SEARCHROM 0xf0
#define DS18B20_CMD_READROM 0x33
#define DS18B20_CMD_MATCHROM 0x55
#define DS18B20_CMD_SKIPROM 0xcc
#define DS18B20_CMD_ALARMSEARCH 0xec

//stop any interrupt on read
#define DS18B20_STOPINTERRUPTONREAD 1

//functions
extern double ds18b20_gettemp();




#endif /* DS18B20_H_ */