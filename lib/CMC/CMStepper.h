
#ifndef CMSTEPPER_H
#define CMSTEPPER_H

#include <Arduino.h>
#include <AccelStepper.h>

enum CMStepperMode {
    MODE_STOP = 0,
    MODE_FOR = 1,
    MODE_UNTIL = 2
};

class CMStepper {
public:
    static constexpr int DEFAULT_STEP_DELAY = 5;
    static constexpr uint8_t NO_ENABLE_PIN = 255;

private:
    AccelStepper motor;

    const uint8_t pinENA;
    bool state = false;
    bool enableActiveLow = true;

    CMStepperMode mode = MODE_STOP;
    long targetPosition = 0;
    int stepsCount = 0;
    int stepDelay = DEFAULT_STEP_DELAY;

    float maxSpeed = 1000.0f / DEFAULT_STEP_DELAY;
    float acceleration = 500.0f;

    void updateHardware();
    void updateSpeed();

public:
    // TB6600: STEP, DIR, optional ENA
    CMStepper(uint8_t pinSTEP,
              uint8_t pinDIR,
              uint8_t pinENA = NO_ENABLE_PIN);

    void begin();

    void enable();
    void disable();
    void switchState();
    bool getState() const;
    void setState(bool newState);

    void setEnableActiveLow(bool activeLow);
    void setDirectionInverted(bool inverted);

    void setStepDelay(int newDelay);
    int getStepDelay() const;
    void resetStepDelay();

    void setMaxSpeed(float speed);
    float getMaxSpeed() const;

    void setAcceleration(float value);
    float getAcceleration() const;

    void stepFor(int stepCount);
    void stepUntil(bool isBackwardDirection);

    CMStepperMode getStepperMode() const;
    int getStepsCount() const;
    int getStepsCountAbsolute() const;

    void stopStepper();
    void doStep();

    long currentPosition() const;
    void setCurrentPosition(long position);
};

#endif // CMSTEPPER_H
