#define DEBUG_MODE

#include <Arduino.h>
#include <CMC.h>
#include <CMStepper.h>
#include <JsonUtility.h>

static constexpr uint8_t
        STEPPER_1_ENA = OUT1,
        STEPPER_1_STEP = OUT2,
        STEPPER_1_DIR = OUT3,

        STEPPER_2_ENA = IN1,
        STEPPER_2_STEP = IN2,
        STEPPER_2_DIR = IN3;

CMC *cmc;
bool isInit = false;

void setup() {
    cmc = new CMC(
        Serial1, Serial2,
        CMStepper(
            STEPPER_1_ENA, STEPPER_1_STEP, STEPPER_1_DIR
        ),
        CMStepper(
            STEPPER_2_ENA, STEPPER_2_STEP, STEPPER_2_DIR
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
