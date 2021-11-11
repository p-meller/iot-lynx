#include "WiFiHandler.h"
#include "../FlashHandler/FlashHandler.h"
#include <WiFi.h>

WiFiHandler::WiFiHandler(const std::string &ssid, const std::string &password, FlashHandler *flashHandler)
{
    this->defaultSsid = ssid;
    this->defaultPassword = password;
    this->flashHandler = flashHandler;
}

bool WiFiHandler::connect(const std::string &ssid, const std::string &password)
{
    if (ssid.empty())
    {
        return false;
    }

    if (password.empty())
    {
        return false;
    }

    Serial.println("Check if wifi connected");
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("disconecting");

        WiFi.disconnect();
    }

    while (WiFi.status() == WL_CONNECTED)
    {
        Serial.print(".");
        delay(100);
    }
    Serial.println("");

    Serial.println("Wifi init");

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    Serial.println("Connecting");

    while (!(WiFi.status() == WL_CONNECTED || WiFi.status() == WL_CONNECT_FAILED || WiFi.status() == WL_NO_SSID_AVAIL))
    {
        Serial.print(".");

        delay(500);
    }


    return WiFi.status() == WL_CONNECTED;
}

bool WiFiHandler::connectAndSave(const std::string &ssid, const std::string &password)
{
    if (connect(ssid, password))
    {
        char *ssidStr = new char[ssid.length() + 1];
        strcpy(ssidStr, ssid.c_str());
        char *passwordStr = new char[password.length() + 1];
        strcpy(passwordStr, password.c_str());

        flashHandler->writeToStorage(FlashIndex::WIFI_SSID, ssidStr, ssid.length() + 1);
        flashHandler->writeToStorage(FlashIndex::WIFI_SSID, passwordStr, ssid.length() + 1);
        delete[] ssidStr;
        delete[] passwordStr;
        return true;
    }

    return false;
}

bool WiFiHandler::connectDefault()
{
    return this->connect(this->defaultSsid, this->defaultPassword);
}

bool WiFiHandler::connectFromFlash()
{
    FlashStorageData storageDataSsid = flashHandler->readFromStorage(FlashIndex::WIFI_SSID);
    std::string ssid = std::string(static_cast<char *>(storageDataSsid.data));
    FlashStorageData storageDataPassword = flashHandler->readFromStorage(FlashIndex::WIFI_PASSWORD);
    std::string password = std::string(static_cast<char *>(storageDataPassword.data));
    return this->connect(ssid, password);
}
