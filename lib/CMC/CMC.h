//
// Created by Robo_Start on 21.07.2026.
//

#ifndef CMC_H
#define CMC_H
#include "CMStepper.h"
#include "JsonUtility.h"
#include "Memory.h"
#include "NetworkCard.h"
#include "Tasks.h"

class CMC {
    CMStepper
            rightWheel,
            leftWheel;

    HardwareSerial
            &network,
            &module;

    NetworkCard networkCard;
    Memory memory;

    bool
            debuggerConnected,
            networkSerialConnected,
            moduleSerialConnected,

            serialRelay;

public:
    CMC(HardwareSerial &network_serial, HardwareSerial &module_serial, const CMStepper &right_wheel,
        const CMStepper &left_wheel)
        : rightWheel(right_wheel),
          leftWheel(left_wheel), network(network_serial), module(module_serial), networkCard(network) {
    }

    void initSerials();

    void setup();

    void parseMain(const String &string, HardwareSerial &serialToSend);

    void parseDebugger(const String &string);

    void parseNetwork(const String &string);

    void parseModule(const String &string);

    void listenSerials();

    void loop();
};

#endif //CMC_H
