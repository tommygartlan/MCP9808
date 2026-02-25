/* * File: mcp9808.h
 * Device: MCP9808 Digital Temperature Sensor
 * Datasheet: DS20005095B
 */

#ifndef MCP9808_H
#define	MCP9808_H

//#include "mcc_generated_files/system/system.h"
#include <xc.h>
// I2C Address (User specified: 0x1C)
#define MCP9808_ADDR 0x1C 

// Register Pointers (Source: Datasheet Page 16, Register 5-1) 
#define REG_CONFIG      0x01
#define REG_UPPER_TEMP  0x02
#define REG_LOWER_TEMP  0x03
#define REG_CRIT_TEMP   0x04
#define REG_AMBIENT_TEMP 0x05
#define REG_MANUF_ID    0x06
#define REG_DEVICE_ID   0x07
#define REG_RESOLUTION  0x08

// Function Prototypes
void MCP9808_Init(void);
float MCP9808_ReadTemp(void);
uint16_t MCP9808_GetManufID(void);
void MCP9808_SetResolution(uint8_t resolution);

#endif	/* MCP9808_H */