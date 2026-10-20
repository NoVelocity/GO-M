
#include "CMStepper.h"

CMStepper::CMStepper(
    const uint8_t pinSTEP,
    const uint8_t pinDIR,
    const uint8_t pinENA
) : pinSTEP(pinSTEP),
pinDIR(pinDIR),
pinENA(pinENA) {
}

void CMStepper::begin() {
    pinMode(pinSTEP, OUTPUT);
    pinMode(pinDIR, OUTPUT);

    digitalWrite(pinSTEP, LOW);
    digitalWrite(pinDIR, LOW);

    if (pinENA != NO_ENABLE_PIN) {
        pinMode(pinENA, OUTPUT);
    }

    updateHardware();
}

void CMStepper::enable() {
    setState(true);
}

void CMStepper::disable() {
    setState(false);
}

void CMStepper::switchState() {
    setState(!state);
}

bool CMStepper::getState() const {
    return state;
}

void CMStepper::setState(const bool newState) {
    state = newState;
    updateHardware();
}

void CMStepper::setEnableActiveLow(const bool activeLow) {
    enableActiveLow = activeLow;
    updateHardware();
}

void CMStepper::setDirectionInverted(const bool inverted) {
    directionInverted = inverted;
}

void CMStepper::updateHardware() {
    if (pinENA != NO_ENABLE_PIN) {
        digitalWrite(
            pinENA,
            state
            ? (enableActiveLow ? LOW : HIGH)
            : (enableActiveLow ? HIGH : LOW)
        );
    }

    if (!state) {
        digitalWrite(pinSTEP, LOW);
    }
}

void CMStepper::setStepDelay(const int newDelay) {
    stepDelay = max(0, newDelay);
}

int CMStepper::getStepDelay() const {
    return stepDelay;
}

void CMStepper::resetStepDelay() {
    setStepDelay(DEFAULT_STEP_DELAY);
}

void CMStepper::stepFor(const int count) {
    if (mode != MODE_STOP || count == 0) {
        return;
    }

    stepsCount = count;
    mode = MODE_FOR;
}

void CMStepper::stepUntil(const bool isBackwardDirection) {
    stepsCount = isBackwardDirection ? 0 : 1;
    mode = MODE_UNTIL;
}

CMStepperMode CMStepper::getStepperMode() const {
    return mode;
}

int CMStepper::getStepsCount() const {
    return stepsCount;
}

int CMStepper::getStepsCountAbsolute() const {
    if (stepsCount < 0) {
        return -stepsCount;
    }

    return stepsCount;
}

void CMStepper::stopStepper() {
    mode = MODE_STOP;
    stepsCount = 0;
    digitalWrite(pinSTEP, LOW);
}

void CMStepper::doStep() {
    if (!state) {
        return;
    }

    switch (mode) {
        case MODE_FOR:
            if (stepsCount == 0) {
                mode = MODE_STOP;
                return;
            }

            step(stepsCount > 0);

            if (stepsCount > 0) {
                --stepsCount;
            } else {
                ++stepsCount;
            }
            break;

        case MODE_UNTIL:
            step(stepsCount != 0);
            break;

        case MODE_STOP:
        default:
            return;
    }
}

void CMStepper::step(const bool isForwardDirection) {
    digitalWrite(
        pinDIR,
        (isForwardDirection != directionInverted) ? HIGH : LOW
    );

    // Give the driver time to register the direction.
    delayMicroseconds(5);

    // One rising edge on STEP = one configured microstep.
    digitalWrite(pinSTEP, HIGH);
    delayMicroseconds(5);
    digitalWrite(pinSTEP, LOW);

    if (stepDelay > 0) {
        delay(stepDelay);
    }
}
