/*  
    This library is only a copy-paste from VEML6070 application notes with adaptations for Arduino environment.
    I do not own this code and I didn't write it from scratch!
*/

#ifndef __VEML6070_h
#define	__VEML6070_h

// Veml6070 addreses
#define VEML6070_ADDR_ARA (0x18 >> 1)
#define VEML6070_ADDR_CMD (0x70 >> 1)
#define VEML6070_ADDR_DATA_LSB (0x71 >> 1)
#define VEML6070_ADDR_DATA_MSB (0x73 >> 1)
// VEML6070 command register bits
#define VEML6070_CMD_SD 0x01
#define VEML6070_CMD_IT_0_5T 0x00
#define VEML6070_CMD_IT_1T 0x04
#define VEML6070_CMD_IT_2T 0x08
#define VEML6070_CMD_IT_4T 0x0C
#define VEML6070_CMD_DEFAULT (VEML6070_CMD_WDM | VEML6070_CMD_IT_1T)


class VEML6070 {
    private:
        void init_device();
        int read_byte(uint16_t addr, uint8_t *data);
        int write_byte(uint16_t addr, uint8_t data);

        uint8_t cmd;

    public:
        void begin(uint32_t clock);
        void begin();
        void enable_sensor();
        void disable_sensor();
        uint16_t read_uvs_step();
        RISK_LEVEL convert_to_risk_level(uint16_t uvs_step);

        type enum {LOW, MODERATE, HIGH, VERY_HIGH, EXTREME} RISK_LEVEL;
}

#endif