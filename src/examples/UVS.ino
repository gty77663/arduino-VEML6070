#include "VEML6070.h"

VEML6070 veml;

void setup() {
    veml.begin();
    Serial.prinln("Test sketch for VEML6070 UV sensor.");
}

void loop() {
    uint16_t uvs = veml.read_uvs_step();
    RISK_LEVEL risk_level = veml.convert_to_risk_level(uvs);

    Serial.print("Current UVS is :")
    Serial.print(uvs);
    Serial.print(", and is equal to ")
    Serial.println(risk_level);
}