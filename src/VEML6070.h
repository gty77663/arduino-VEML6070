/*  
    This library is only a copy-paste from VEML6070 application notes with adaptations for Arduino environment.
    I do not own this code and I didn't write it from scratch!
*/

#ifndef __VEML6070_h
#define	__VEML6070_h

#include "stdint.h"

// Veml6070 addreses
#define VEML6070_ADDR_ARA (0x18 >> 1)
#define VEML6070_ADDR_CMD (0x70 >> 1)
#define VEML6070_ADDR_DATA_LSB (0x71 >> 1)
#define VEML6070_ADDR_DATA_MSB (0x73 >> 1)
// VEML6070 command register bits
#define VEML6070_CMD_WDM     0x02
#define VEML6070_CMD_SD      0x01
#define VEML6070_CMD_IT_0_5T 0x00
#define VEML6070_CMD_IT_1T   0x04
#define VEML6070_CMD_IT_2T   0x08
#define VEML6070_CMD_IT_4T   0x0C
#define VEML6070_CMD_ACK     0x20 
#define VEML6070_CMD_ACK_THD 0x10
#define VEML6070_CMD_DEFAULT (VEML6070_CMD_WDM | VEML6070_CMD_IT_1T)

class VEML6070 {
    private:
        // Clear ARA interrupt flag on addres 0x18
        static void clear_ara();
        static int read_byte(int addr, uint8_t *data);
        static int write_byte(int addr, uint8_t data);

        // Private routine that executes user-defined callback and clears ARA flag
        static void ara_routine();
        // Pointer to user-defined callback called on ACK interrupt
        static void (*ack_callback)(void);

        uint8_t cmd;
        // Enumerator for risk levels
        // LOW and HIGH were replaced by MINIMAL and ELEVATED, because of Arduino LOW and HIGH defenitions
        enum RISK_LEVEL {MINIMAL, MODERATE, ELEVATED, VERY_HIGH, EXTREME};
        // Char array that corresponds to RISK_LEVEL enum 
        char* risk_level_char[5] = {"LOW", "MODERATE", "HIGH", "VERY HIGH", "EXTREME"};

    public:
        void begin(uint32_t clock);
        void begin();
        void enable_sensor();
        void disable_sensor();
        void enable_ack();
        void disable_ack();
        // Set ack trashhold, 0 - 102 steps, 1 - 145 steps
        void set_ack_thd(uint8_t ack_thd);
        // Set ACK callback function, (uint8_t gpio, void (*user_func)())
        int8_t set_ack_callback(uint8_t gpio, void (*user_func)(void));
        uint16_t read_uvs_step();
        char* convert_to_risk_char(uint8_t risk_index);
        VEML6070::RISK_LEVEL convert_to_risk_level(uint16_t uvs_step);
};

extern VEML6070 VEML;

#endif