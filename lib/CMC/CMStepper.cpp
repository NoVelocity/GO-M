
#include "CMStepper.h"

CMStepper::CMStepper(
    uint8_t pinSTEP,
    uint8_t pinDIR,
    uint8_t pinENA
)
: motor(AccelStepper::DRIVER, pinSTEP, pinDIR),
pinENA(pinENA) {
}

void CMStepper::begin() {
    motor.setMaxSpeed(maxSpeed);
    motor.setAcceleration(acceleration);
    motor.setMinPulseWidth(5);

    if (pinENA != NO_ENABLE_PIN) {
        pinMode(pinENA, OUTPUT);

        // AccelStepper normally enables a driver with LOW.
        // Inversion is configurable for different TB6600 boards.
        motor.setEnablePin(pinENA);
        motor.setPinsInverted(false, false, enableActiveLow);
    }

    motor.disableOutputs();
    state = false;
    mode = MODE_STOP;
    stepsCount = 0;
}

void CMStepper::updateHardware() {
    if (state) {
        motor.enableOutputs();
    } else {
        motor.disableOutputs();
    }
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

void CMStepper::setState(bool newState) {
    state = newState;
    updateHardware();

    if (!state) {
        stopStepper();
    }
}

void CMStepper::setEnableActiveLow(bool activeLow) {
    enableActiveLow = activeLow;

    // Reapply inversion configuration.
    motor.setPinsInverted(false, false, enableActiveLow);
    updateHardware();
}

void CMStepper::setDirectionInverted(bool inverted) {
    motor.setPinsInverted(
        inverted,
        false,
        enableActiveLow
    );
}

void CMStepper::updateSpeed() {
    motor.setMaxSpeed(maxSpeed);
    motor.setAcceleration(acceleration);
}

void CMStepper::setStepDelay(int newDelay) {
    if (newDelay < 1) {
        newDelay = 1;
    }

    stepDelay = newDelay;
    maxSpeed = 1000.0f / stepDelay;
    updateSpeed();
}

int CMStepper::getStepDelay() const {
    return stepDelay;
}

void CMStepper::resetStepDelay() {
    setStepDelay(DEFAULT_STEP_DELAY);
}

void CMStepper::setMaxSpeed(float speed) {
    if (speed <= 0) {
        return;
    }

    maxSpeed = speed;
    stepDelay = max(1, (int)(1000.0f / maxSpeed));
    updateSpeed();
}

float CMStepper::getMaxSpeed() const {
    return maxSpeed;
}

void CMStepper::setAcceleration(float value) {
    if (value <= 0) {
        return;
    }

    acceleration = value;
    motor.setAcceleration(acceleration);
}

float CMStepper::getAcceleration() const {
    return acceleration;
}

void CMStepper::stepFor(int count) {
    if (mode != MODE_STOP || count == 0) {
        return;
    }

    targetPosition = motor.currentPosition() + (long)count;
    stepsCount = count;
    mode = MODE_FOR;

    motor.moveTo(targetPosition);
}

void CMStepper::stepUntil(bool isBackwardDirection) {
    mode = MODE_UNTIL;
    stepsCount = isBackwardDirection ? 0 : 1;

    motor.setSpeed(isBackwardDirection ? -maxSpeed : maxSpeed);
}

CMStepperMode CMStepper::getStepperMode() const {
    return mode;
}

int CMStepper::getStepsCount() const {
    if (mode == MODE_FOR) {
        long remaining = motor.distanceToGo();

        if (remaining > 2147483647L) return 2147483647;
        if (remaining < -2147483647L) return -2147483647;

        return (int)remaining;
    }

    return stepsCount;
}

int CMStepper::getStepsCountAbsolute() const {
    long remaining = getStepsCount();

    return (int)(remaining < 0 ? -remaining : remaining);
}

void CMStepper::stopStepper() {
    motor.stop();
    mode = MODE_STOP;
    stepsCount = 0;
}

void CMStepper::doStep() {
    if (!state || mode == MODE_STOP) {
        return;
    }

    if (mode == MODE_FOR) {
        motor.run();

        if (motor.distanceToGo() == 0) {
            mode = MODE_STOP;
            stepsCount = 0;
        }
    } else if (mode == MODE_UNTIL) {
        motor.runSpeed();
    }
}

long CMStepper::currentPosition() const {
    return motor.currentPosition();
}

void CMStepper::setCurrentPosition(long position) {
    motor.setCurrentPosition(position);
    targetPosition = position;
    stepsCount = 0;
    mode = MODE_STOP;
}
