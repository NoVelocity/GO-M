//
// Created by Robo_Start on 24.07.2026.
//

#ifndef CMSTEPPER_H
#define CMSTEPPER_H
#include <Arduino.h>

enum CMStepperMode {
    MODE_STOP = 0,
    MODE_FOR = 1,
    MODE_UNTIL = 2
};

enum CMStepperPhase {
    STEP_1 = 0,
    STEP_2 = 1,
    STEP_3 = 2,
    STEP_4 = 3
};

class CMStepper {
public:
    static constexpr int DEFAULT_STEP_DELAY = 5;

private:
    const uint8_t
            pinENA,
            pinIN1,
            pinIN2,
            pinIN3,
            pinIN4,
            pinENB;

    bool state = false;

    CMStepperMode mode = MODE_STOP;
    CMStepperPhase phase = STEP_1;
    int stepsCount = 0;

    int stepDelay = DEFAULT_STEP_DELAY;

    void updateHardware() const;

    void step(bool isForwardDirection);

public:
    CMStepper(uint8_t pinENA, uint8_t pinIN1, uint8_t pinIN2, uint8_t pinIN3, uint8_t pinIN4, uint8_t pinENB);

    void begin() const;

    // State
    void enable();

    void disable();

    void switchState();

    bool getState() const;

    void setState(bool newState);

    // Step delay
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


#endif //CMSTEPPER_H
