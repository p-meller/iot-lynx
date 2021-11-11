#ifndef WIFIHANDLER_H
#define WIFIHANDLER_H

#include <string>

class FlashHandler;

class WiFiHandler
{

    std::string defaultSsid;
    std::string defaultPassword;
    FlashHandler *flashHandler;

    bool connect(const std::string &ssid, const std::string &password);

public:
    WiFiHandler(const std::string &ssid, const std::string &password, FlashHandler *flashHandler);


    bool connectAndSave(const std::string &ssid, const std::string &password);
    bool connectDefault();
    bool connectFromFlash();
};

#endif /* WIFIHANDLER_H */
