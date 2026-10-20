
#ifndef CMSTEPPER_H
#define CMSTEPPER_H

#include <Arduino.h>

enum CMStepperMode {
    MODE_STOP = 0,
    MODE_FOR = 1,
    MODE_UNTIL = 2
};

class CMStepper {
public:
    static constexpr int DEFAULT_STEP_DELAY = 5;

private:
    static constexpr uint8_t NO_ENABLE_PIN = 255;

    const uint8_t pinSTEP;
    const uint8_t pinDIR;
    const uint8_t pinENA;

    bool state = false;
    bool enableActiveLow = true;
    bool directionInverted = false;

    CMStepperMode mode = MODE_STOP;
    int stepsCount = 0;
    int stepDelay = DEFAULT_STEP_DELAY;

    void updateHardware();
    void step(bool isForwardDirection);

public:
    // ENA необязателен: можно оставить неподключённым.
    CMStepper(uint8_t pinSTEP,
              uint8_t pinDIR,
              uint8_t pinENA = NO_ENABLE_PIN);

    void begin();

    // State
    void enable();
    void disable();
    void switchState();
    bool getState() const;
    void setState(bool newState);

    // Driver configuration
    void setEnableActiveLow(bool activeLow);
    void setDirectionInverted(bool inverted);

    // Step delay, milliseconds between pulses
    void setStepDelay(int newDelay);
    int getStepDelay() const;
    void resetStepDelay();

    // Steps
    void stepFor(int stepCount);
    void stepUntil(bool isBackwardDirection);

    CMStepperMode getStepperMode() const;
    int getStepsCount() const;
    int getStepsCountAbsolute() const;

    void stopStepper();
    void doStep();
};

#endif // CMSTEPPER_H
