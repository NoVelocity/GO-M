//
// Created by Robo_Start on 21.07.2026.
//
#include "CMC.h"

void CMC::initSerials() {
    Serial.begin(115200);
    network.begin(9600);
    module.begin(9600);

    delay(100);

    JsonDocument doc;
    doc["task"] = static_cast<int>(INIT);

    JsonUtility::jsonToSerial(doc, Serial, false);
    JsonUtility::jsonToSerial(doc, network, false);
    JsonUtility::jsonToSerial(doc, module, false);
}

void CMC::setup() {
    rightWheel.begin();
    leftWheel.begin();

    initSerials();
}


void CMC::parseMain(const String &string, HardwareSerial &serialToSend) {
    JsonDocument doc;
    JsonUtility::jsonFromString(string, doc);

    if (doc["task"]) {
        const int task = doc["task"].as<int>();
        switch (task) {
            case ROBOT_STATUS: {
                JsonDocument docToSend;

                docToSend["task"] = ROBOT_STATUS;
                docToSend["debuggerConnected"] = debuggerConnected;
                docToSend["serialRelay"] = serialRelay;

                docToSend["networkSerialConnected"] = networkSerialConnected;
                if (networkSerialConnected) {
                    docToSend["wifiConnected"] = networkCard.getWifiConnected();
                } else {
                    docToSend["wifiConnected"] = nullptr;
                }
                if (networkCard.getWifiConnected()) {
                    docToSend["wifiSSID"] = networkCard.getWifiSSID();
                    docToSend["serverIp"] = networkCard.getServerIp().toString();
                    docToSend["robotId"] = networkCard.getRobotId();
                } else {
                    docToSend["wifiSSID"] = nullptr;
                    docToSend["serverIp"] = nullptr;
                    docToSend["robotId"] = nullptr;
                }
                docToSend["moduleSerialConnected"] = moduleSerialConnected;

                docToSend["rightWheelState"] = rightWheel.getState();
                docToSend["rightWheelMode"] = rightWheel.getStepperMode();
                docToSend["rightWheelDelay"] = rightWheel.getStepDelay();
                docToSend["rightWheelSteps"] = rightWheel.getStepsCount();

                docToSend["leftWheelState"] = leftWheel.getState();
                docToSend["leftWheelMode"] = leftWheel.getStepperMode();
                docToSend["leftWheelDelay"] = leftWheel.getStepDelay();
                docToSend["leftWheelSteps"] = leftWheel.getStepsCount();

                JsonUtility::jsonToSerial(docToSend, serialToSend, &serialToSend == &network);
                break;
            }
            case
            STOP: {
                if (doc["rightWheel"].as<bool>()) {
                    rightWheel.stopStepper();
                } else {
                    leftWheel.stopStepper();
                }
                break;
            }
            case FOR: {
                if (doc["rightWheel"].as<bool>()) {
                    rightWheel.stepFor(doc["stepsCount"].as<int>());
                } else {
                    leftWheel.stepFor(doc["stepsCount"].as<int>());
                }
                break;
            }
            case UNTIL: {
                if (doc["rightWheel"].as<bool>()) {
                    rightWheel.stepUntil(doc["backward"].as<bool>());
                } else {
                    leftWheel.stepUntil(doc["backward"].as<bool>());
                }
                break;
            }
            case STATE: {
                if (doc["rightWheel"].as<bool>()) {
                    rightWheel.setState(doc["state"].as<bool>());
                } else {
                    leftWheel.setState(doc["state"].as<bool>());
                }
                break;
            }
            case ENABLE: {
                if (doc["rightWheel"].as<bool>()) {
                    rightWheel.enable();
                } else {
                    leftWheel.enable();
                }
                break;
            }
            case DISABLE: {
                if (doc["rightWheel"].as<bool>()) {
                    rightWheel.disable();
                } else {
                    leftWheel.disable();
                }
                break;
            }
            default: {
                Serial.print("Unknown command: ");
                Serial.println(string);
                break;
            }
        }
    }
}

void CMC::parseDebugger(const String &string) {
    JsonDocument doc;
    JsonUtility::jsonFromString(string, doc);

    JsonDocument dbgDoc;
    dbgDoc["task"] = DEBUG;
    dbgDoc["from"] = "DEBUGGER";
    dbgDoc["payload"] = doc;
    delay(50);
    serializeJson(dbgDoc, Serial);
    Serial.println();
    delay(50);

    if (doc["task"]) {
        const int task = doc["task"].as<int>();
        switch (task) {
            case INIT: {
                debuggerConnected = true;
                break;
            }
            case MEMORY_READ: {
                JsonDocument result;
                result["task"] = MEMORY_READ;
                result["content"] = Memory::readMemory(static_cast<MemoryDataType>(doc["file"].as<int>()));
                JsonUtility::jsonToSerial(result, Serial, false);
                break;
            }
            case SEND_DEBUG_FROM_M: {
                parseModule(doc["payload"].as<String>());
                break;
            }
            case SEND_DEBUG_FROM_NC: {
                parseNetwork(doc["payload"].as<String>());
                break;
            }
            case SEND_DEBUG_TO_M: {
                JsonUtility::jsonToSerial(doc["payload"], module, false);
                break;
            }
            case SEND_DEBUG_TO_NC: {
                JsonUtility::jsonToSerial(doc["payload"], network, false);
                break;
            }
            default: {
                parseMain(string, Serial);
                break;
            }
        }
    }
}

void CMC::parseNetwork(const String &string) {
    JsonDocument doc;
    JsonUtility::jsonFromString(string, doc);

    JsonDocument dbgDoc;
    dbgDoc["task"] = DEBUG;
    dbgDoc["from"] = "NETWORK";
    dbgDoc["payload"] = doc;
    delay(50);
    serializeJson(dbgDoc, Serial);
    Serial.println();
    delay(50);

    if (doc["task"]) {
        const int task = doc["task"].as<int>();
        switch (task) {
            case INIT: {
                networkSerialConnected = true;
                networkCard.begin(
                    Memory::readMemory(SSID),
                    Memory::readMemory(PASSWORD),
                    Memory::readMemory(ID),
                    IPAddr(Memory::readMemory(SERVER_IP)),
                    static_cast<uint16_t>(Memory::readMemory(SERVER_PORT).toInt())
                );
                networkCard.reset();
                break;
            }
            case RESET: {
                networkCard.connect();
                break;
            }
            case CONNECT: {
                networkCard.connected();
                break;
            }
            case RECEIVE: {
                parseNetwork(doc["payload"].as<String>());
                break;
            }
            default: {
                parseMain(string, network);
                break;
            }
        }
    }
}

void CMC::parseModule(const String &string) {
    JsonDocument doc;
    JsonUtility::jsonFromString(string, doc);

    JsonDocument dbgDoc;
    dbgDoc["task"] = DEBUG;
    dbgDoc["from"] = "MODULE";
    dbgDoc["payload"] = doc;
    delay(50);
    serializeJson(dbgDoc, Serial);
    Serial.println();
    delay(50);

    if (doc["task"]) {
        const int task = doc["task"].as<int>();
        switch (task) {
            case INIT: {
                moduleSerialConnected = true;
                break;
            }
            default: {
                parseMain(string, module);
                break;
            }
        }
    }
}


void CMC::listenSerials() {
    if (Serial.available() > 0) {
        parseDebugger(Serial.readString());
    }
    if (network.available() > 0) {
        parseNetwork(network.readString());
    }
    if (module.available() > 0) {
        parseModule(module.readString());
    }
}

void CMC::loop() {
    listenSerials();

    rightWheel.doStep();
    leftWheel.doStep();
}
