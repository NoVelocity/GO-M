//
// Created by Robo_Start on 24.07.2026.
//

#include "CMStepper.h"

CMStepper::CMStepper(const uint8_t pinENA, const uint8_t pinIN1, const uint8_t pinIN2, const uint8_t pinIN3,
                     const uint8_t pinIN4, const uint8_t pinENB) : pinENA(pinENA),
                                                                   pinIN1(pinIN1),
                                                                   pinIN2(pinIN2),
                                                                   pinIN3(pinIN3),
                                                                   pinIN4(pinIN4),
                                                                   pinENB(pinENB) {
}

void CMStepper::begin() const {
    pinMode(pinENA, OUTPUT);
    pinMode(pinIN1, OUTPUT);
    pinMode(pinIN2, OUTPUT);
    pinMode(pinIN3, OUTPUT);
    pinMode(pinIN4, OUTPUT);
    pinMode(pinENB, OUTPUT);
}

void CMStepper::enable() {
    setState(true);
}

void CMStepper::disable() {
    setState(false);
}

void CMStepper::switchState() {
    setState(!this->getState());
}

bool CMStepper::getState() const {
    return state;
}

void CMStepper::setState(const bool newState) {
    state = newState;
    updateHardware();
}

void CMStepper::setStepDelay(const int newDelay) {
    stepDelay = newDelay;
}

int CMStepper::getStepDelay() const {
    return stepDelay;
}

void CMStepper::resetStepDelay() {
    setStepDelay(DEFAULT_STEP_DELAY);
}

void CMStepper::stepFor(const int stepCount) {
    if (mode != MODE_STOP) {
        return;
    }
    mode = MODE_FOR;
    stepsCount = stepCount;
}

void CMStepper::stepUntil(const bool isBackwardDirection) {
    mode = MODE_UNTIL;
    stepsCount = isBackwardDirection ? 0 : 1;
}

CMStepperMode CMStepper::getStepperMode() const {
    return mode;
}

int CMStepper::getStepsCount() const {
    return stepsCount;
}

int CMStepper::getStepsCountAbsolute() const {
    return abs(getStepsCount());
}

void CMStepper::stopStepper() {
    mode = MODE_STOP;
    stepsCount = 0;
}

void CMStepper::doStep() {
    if (!state) return;

    switch (mode) {
        case MODE_FOR: {
            if (stepsCount != 0) {
                step(stepsCount > 0);
                if (stepsCount > 0) {
                    stepsCount--;
                } else {
                    stepsCount++;
                }
            } else {
                mode = MODE_STOP;
            }
            break;
        }
        case MODE_UNTIL: {
            step(stepsCount);
            break;
        }
    }
}

void CMStepper::updateHardware() const {
    digitalWrite(pinENA, state);
    digitalWrite(pinENB, state);
    if (!state) {
        digitalWrite(pinIN1, false);
        digitalWrite(pinIN2, false);
        digitalWrite(pinIN3, false);
        digitalWrite(pinIN4, false);
    }
}

void CMStepper::step(const bool isForwardDirection) {
    switch (phase) {
        case STEP_1: {
            digitalWrite(pinIN1, true);
            digitalWrite(pinIN2, false);
            digitalWrite(pinIN3, isForwardDirection);
            digitalWrite(pinIN4, !isForwardDirection);
            phase = STEP_2;
            break;
        }
        case STEP_2: {
            digitalWrite(pinIN1, false);
            digitalWrite(pinIN2, true);
            digitalWrite(pinIN3, isForwardDirection);
            digitalWrite(pinIN4, !isForwardDirection);
            phase = STEP_3;
            break;
        }
        case STEP_3: {
            digitalWrite(pinIN1, false);
            digitalWrite(pinIN2, true);
            digitalWrite(pinIN3, !isForwardDirection);
            digitalWrite(pinIN4, isForwardDirection);
            phase = STEP_4;
            break;
        }
        case STEP_4: {
            digitalWrite(pinIN1, true);
            digitalWrite(pinIN2, false);
            digitalWrite(pinIN3, !isForwardDirection);
            digitalWrite(pinIN4, isForwardDirection);
            phase = STEP_1;
            break;
        }
    }
    delay(stepDelay);
}
