/* * File: mcp9808.c
 * implementation based on Datasheet DS20005095B Appendix A 
 */

#include "MCP9808.h"
//#include "I2C_Func_Simple_v2.h"
#include "../Curioisty_I2C_Library-main/I2C_Func_Simple_v2.h"

// ==========================================
// MCP9808 Application Functions
// ==========================================

/**
 * @brief  Initializes and checks presence of the MCP9808 temperature sensor on the I2C bus.
 * @return bool - true if device acknowledged (ACK), false if device failed to respond (NACK).
 */
bool MCP9808_Acknowledge(void) {
    bool device_acknowledged = false;

    // 1. Generate I2C START condition
    i2c_start();

    // 2. Transmit 7-bit slave address + Write bit (0)
    // i2c_write returns true on ACK (ACKSTAT == 0) or false on NACK (ACKSTAT == 1)
    device_acknowledged = i2c_write((unsigned char)((MCP9808_ADDR << 1) | 0x00));

    // 3. Generate I2C STOP condition to free the bus
    i2c_stop();

    // 4. Return presence verification status
    return device_acknowledged;
}

// Logic from "Example 5-1: Sample Instruction Code" 
float MCP9808_ReadTemp(void) {
    unsigned char UpperByte, LowerByte;
    float Temperature;

    i2c_start();                    // Send START
    i2c_write((MCP9808_ADDR << 1) & 0xFE); // Write Command (Address + Write) 
    i2c_write(REG_AMBIENT_TEMP);    // Write TA Register Pointer [cite: 593]
    
    i2c_repStart();                 // Repeat START
    i2c_write((MCP9808_ADDR << 1) | 0x01); // Read Command (Address + Read) [cite: 594]
    
    UpperByte = i2c_read(1);        // Read 8 bits and Send ACK [cite: 595]
    LowerByte = i2c_read(0);        // Read 8 bits and Send NAK [cite: 596]
    i2c_stop();                     // Send STOP [cite: 597]

    // Convert the temperature data (Logic from Example 5-1)
    UpperByte = UpperByte & 0x1F;   // Clear flag bits [cite: 598]
    
    if ((UpperByte & 0x10) == 0x10) { // TA < 0°C [cite: 599]
        UpperByte = UpperByte & 0x0F; // Clear SIGN
        Temperature = 256.0 - ((float)UpperByte * 16.0 + (float)LowerByte / 16.0);
        Temperature = Temperature * -1; // Make negative (implied by eq)
    } else {                          // TA >= 0°C [cite: 600]
        Temperature = ((float)UpperByte * 16.0 + (float)LowerByte / 16.0);
    }
    
    return Temperature;
}

uint16_t MCP9808_GetManufID(void) {
    uint8_t upper, lower;
    
    i2c_start();
    i2c_write((MCP9808_ADDR << 1) | 0);
    i2c_write(REG_MANUF_ID); 
    
    i2c_repStart();
    i2c_write((MCP9808_ADDR << 1) | 1);
    
    upper = i2c_read(true);
    lower = i2c_read(false);
    i2c_stop();
    
    return ((uint16_t)upper << 8) | lower;
}


void MCP9808_SetResolution(uint8_t resolution) {
    // Write to Resolution Register
    i2c_start();
    i2c_write((MCP9808_ADDR << 1) | 0);
    i2c_write(REG_RESOLUTION);
    i2c_write(resolution & 0x03);
    i2c_stop();
}
