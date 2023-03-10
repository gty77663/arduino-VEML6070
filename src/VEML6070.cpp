/*  
    This library is only a copy-paste from VEML6070 application notes with adaptations for Arduino environment.
    I do not own this code and I didn't write it from scratch!
*/

#include "VEML6070.h"
#include "Wire.h"
#include "Arduino.h"


void VEML6070::begin(uint32_t clock)
{
    Wire.begin();
    Wire.setClock(clock);

    cmd = VEML6070_CMD_DEFAULT;
}

void VEML6070::begin()
{
    Wire.begin();

    cmd = VEML6070_CMD_DEFAULT;
}


void VEML6070::init_device()
{
    // Read ARA to clear interrupt
    uint8_t address;
    VEML6070::read_byte(VEML6070_ADDR_ARA, &address);
    // Initialize command register
    VEML6070::write_byte(VEML6070_ADDR_CMD, cmd);
    delay(200);
}

void VEML6070::enable_sensor(void)
{
    cmd &= ~VEML6070_CMD_SD;
    VEML6070::write_byte(VEML6070_ADDR_CMD, cmd);
}

void VEML6070::disable_sensor(void)
{
    cmd |= VEML6070_CMD_SD;
    VEML6070::write_byte(VEML6070_ADDR_CMD, cmd);
}

uint16_t VEML6070::read_uvs_step(void)
{
    uint8_t lsb, msb;
    uint16_t data;
    VEML6070::read_byte(VEML6070_ADDR_DATA_MSB, &msb);
    VEML6070::read_byte(VEML6070_ADDR_DATA_LSB, &lsb);
    data = ((uint16_t)msb << 8) | (uint16_t)lsb;
    return data;
}

RISK_LEVEL VEML6070::convert_to_risk_level(uint16_t uvs_step)
{
    uint16_t risk_level_mapping_table[4] = {2241, 4482, 5976, 8217};
    uint8_t i;
    for (i = 0; i < 4; i++)
    {
        if (uvs_step <= risk_level_mapping_table[i])
        {
            break;
        }
    }
    return (RISK_LEVEL)i;
}

int VEML6070::read_byte(uint16_t addr, uint8_t *data)
{
    Wire.requestFrom(addr, 1);
    if (Wire.avalible()) 
    {
        *data = Wire.read();
    } else {
        return -1;
    }

    return 1;
}

int VEML6070::write_byte(uint16_t addr, uint8_t data)
{   
    uint8_t err; 

    Wire.beginTransmission(addr);
    err = Wire.write(data);
    Wire.endTransmission();

    return err;
}