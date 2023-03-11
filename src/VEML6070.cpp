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
    
    VEML6070::clear_ara();

    // Initialize command register
    this->cmd = VEML6070_CMD_DEFAULT;
    VEML6070::write_byte(VEML6070_ADDR_CMD, this->cmd);
    delay(200);
}

void VEML6070::begin()
{
    Wire.begin();

    VEML6070::clear_ara();

    // Initialize command register
    this->cmd = VEML6070_CMD_DEFAULT;
    VEML6070::write_byte(VEML6070_ADDR_CMD, this->cmd);
    delay(200);
}

void VEML6070::clear_ara()
{
    // Read ARA to clear interrupt
    uint8_t *address = new uint8_t;
    VEML6070::read_byte(VEML6070_ADDR_ARA, address);
}

void VEML6070::enable_sensor(void)
{
    this->cmd &= ~VEML6070_CMD_SD;
    VEML6070::write_byte(VEML6070_ADDR_CMD, this->cmd);
}

void VEML6070::disable_sensor(void)
{
    this->cmd |= VEML6070_CMD_SD;
    VEML6070::write_byte(VEML6070_ADDR_CMD, this->cmd);
}

void VEML6070::enable_ack()
{
    this->cmd |= VEML6070_CMD_ACK;
    VEML6070::write_byte(VEML6070_ADDR_CMD, this->cmd);
}

void VEML6070::disable_ack()
{
    this->cmd &= ~VEML6070_CMD_ACK;
    VEML6070::write_byte(VEML6070_ADDR_CMD, this->cmd);
}

void VEML6070::set_ack_thd(uint8_t ack_thd)
{
    if (ack_thd == 1) 
    {
       this->cmd |= VEML6070_CMD_ACK_THD;
    } else
    {
        this->cmd &= ~VEML6070_CMD_ACK_THD;
    }
}

int8_t VEML6070::set_ack_callback(uint8_t gpio, void (*user_func)(void))
{
    noInterrupts();
    this->ack_callback = user_func;
    uint8_t interrupt_number = digitalPinToInterrupt(gpio);
    if (interrupt_number == -1)
    {
        return -1;
    }
    attachInterrupt(interrupt_number, VEML6070::ara_routine, FALLING);
    interrupts();
    return interrupt_number;
}

void VEML6070::ara_routine()
{
    VEML6070::ack_callback();
    VEML6070::clear_ara();
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

VEML6070::RISK_LEVEL VEML6070::convert_to_risk_level(uint16_t uvs_step)
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

char *VEML6070::convert_to_risk_char(uint8_t risk_index)
{
    char *ch = new char;
    ch = this->risk_level_char[risk_index];
    return ch;
}

int VEML6070::read_byte(int addr, uint8_t *data)
{
    Wire.requestFrom(addr, 1);
    if (Wire.available())
    {
        *data = Wire.read();
    }
    else
    {
        return -1;
    }

    return 1;
}

int VEML6070::write_byte(int addr, uint8_t data)
{
    uint8_t err;

    Wire.beginTransmission(addr);
    err = Wire.write(data);
    Wire.endTransmission();

    return err;
}

VEML6070 VEML;