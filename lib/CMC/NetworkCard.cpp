//
// Created by Robo_Start on 03.08.2026.
//

#include "NetworkCard.h"


void NetworkCard::begin(String wifi_ssid, String wifi_pass, String id,
                        const IPAddr &server_ip, uint16_t port) {
    ssid = wifi_ssid;
    password = wifi_pass;
    robotId = id;
    serverIP = server_ip;
    serverPort = port;
}

bool NetworkCard::getWifiConnected() const {
    return wifiConnected;
}

IPAddr NetworkCard::getServerIp() {
    return serverIP;
}

String NetworkCard::getWifiSSID() const {
    return ssid;
}

void NetworkCard::reset() {
    wifiConnected = false;
    JsonDocument doc;
    doc["task"] = RESET;
    JsonUtility::jsonToSerial(doc, networkCardSerial, false);
}

void NetworkCard::connect() const {
    if (wifiConnected) return;
    JsonDocument doc;
    doc["task"] = CONNECT;
    doc["ssid"] = ssid;
    doc["password"] = password;
    doc["id"] = robotId;
    doc["ip"] = serverIP.toString();
    doc["port"] = String(serverPort);
    JsonUtility::jsonToSerial(doc, networkCardSerial, false);
}

void NetworkCard::connected() {
    wifiConnected = true;
}

void NetworkCard::send(const JsonDocument &payload) const {
    JsonDocument doc;
    doc["task"] = NetworkTasks::SEND;
    doc["payload"] = payload;
    JsonUtility::jsonToSerial(doc, networkCardSerial, false);
}

String NetworkCard::getRobotId() {
    return robotId;
}
