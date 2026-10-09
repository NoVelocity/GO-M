#define DEBUG_MODE

#include <Arduino.h>
#include <CMC.h>
#include <CMStepper.h>
#include <JsonUtility.h>

static constexpr uint8_t
        STEPPER_1_ENA = OUT2,
        STEPPER_1_IN1 = OUT1,
        STEPPER_1_IN2 = OUT4,
        STEPPER_1_IN3 = OUT3,
        STEPPER_1_IN4 = OUT6,
        STEPPER_1_ENB = OUT5,

        STEPPER_2_ENA = IN2,
        STEPPER_2_IN1 = IN1,
        STEPPER_2_IN2 = IN4,
        STEPPER_2_IN3 = IN3,
        STEPPER_2_IN4 = IN6,
        STEPPER_2_ENB = IN5;

CMC *cmc;
bool isInit = false;

void setup() {
    cmc = new CMC(
        Serial1, Serial2,
        CMStepper(
            STEPPER_1_ENA, STEPPER_1_IN1, STEPPER_1_IN2, STEPPER_1_IN3, STEPPER_1_IN4, STEPPER_1_ENB
        ),
        CMStepper(
            STEPPER_2_ENA, STEPPER_2_IN1, STEPPER_2_IN2, STEPPER_2_IN3, STEPPER_2_IN4, STEPPER_2_ENB
        )
    );
}

void loop() {
    if (!isInit) {
        cmc->setup();
        isInit = true;
    } else {
        cmc->loop();
    }
}
