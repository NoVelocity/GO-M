//
// Created by Robo_Start on 03.08.2026.
//

#ifndef NETWORKCARD_H
#define NETWORKCARD_H
#include <ArduinoJson.h>

#include "IPAddr.h"
#include "../CPL/JsonUtility.h"
#include "Tasks.h"

class NetworkCard {
    bool wifiConnected = false;

    HardwareSerial &networkCardSerial;

    String ssid;
    String password;
    String robotId;

    IPAddr serverIP;
    uint16_t serverPort;

public:
    NetworkCard(HardwareSerial &network_card_serial) : networkCardSerial(network_card_serial) {};

    void begin(String wifi_ssid, String wifi_pass, String id,
               const IPAddr &server_ip, uint16_t port);

    bool getWifiConnected() const;

    IPAddr getServerIp();

    String getWifiSSID() const;

    void reset();

    void connect() const;

    void connected();

    void send(const JsonDocument &payload) const;

    String getRobotId();
};


#endif //NETWORKCARD_H
