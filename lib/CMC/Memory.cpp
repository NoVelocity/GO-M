//
// Created by Robo_Start on 04.08.2026.
//

#include "Memory.h"

const char *Memory::getFileName(const MemoryDataType type) {
    switch (type) {
        case ID: return "ID";
        case SSID: return "SSID";
        case PASSWORD: return "PASSWORD";
        case SERVER_IP: return "SRVIP";
        case SERVER_PORT: return "SRVPORT";
        default: return "NO_FILE_FOUND";
    }
}

String Memory::readMemory(const MemoryDataType type) {
    File myFile = SD.open(getFileName(type));
    if (myFile) {
        String content = myFile.readString();
        myFile.close();
        content.trim();
        return content;
    }
    Serial.print("Error reading file: ");
    Serial.println(getFileName(type));
    return "NO_DATA";
}

void Memory::writeMemory(const String &data, const MemoryDataType type) {
    File myFile = SD.open(getFileName(type), O_WRITE | O_TRUNC);
    if (myFile) {
        myFile.print(data);
        myFile.close();
    } else {
        Serial.print("Error writing file: ");
        Serial.println(getFileName(type));
    }
}



Memory::Memory() {
    SD.begin(SD_CS);
}

Memory::~Memory() {
    SD.end();
}
