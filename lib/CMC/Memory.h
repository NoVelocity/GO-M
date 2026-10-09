//
// Created by Robo_Start on 04.08.2026.
//

#ifndef MEMORY_H
#define MEMORY_H
#include <SPI.h>
#include <SD.h>

enum MemoryDataType {
    ID,
    SSID,
    PASSWORD,
    SERVER_IP,
    SERVER_PORT
};

class Memory {
    static const char *getFileName(MemoryDataType type);

public:
    static String readMemory(MemoryDataType type);

    static void writeMemory(const String &data, MemoryDataType type);

    Memory();
    ~Memory();
};


#endif //MEMORY_H
