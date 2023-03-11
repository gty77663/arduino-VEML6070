#include "VEML6070.h"

void setup() {
    VEML.begin();
    Serial.println("Test sketch for VEML6070 UV sensor.");
}

void loop() {
    uint16_t uvs = VEML.read_uvs_step();
    int risk_level = VEML.convert_to_risk_level(uvs);
    char* risk = VEML.convert_to_risk_char(risk_level);

    Serial.print("Current UVS is: ");
    Serial.print(uvs);
    Serial.print(", with current risk level: ");
    Serial.println(*risk);

    // Integration time can be up to 1000ms in some pplications, so to be safe delay is set accrodingly
    delay(1000);
}