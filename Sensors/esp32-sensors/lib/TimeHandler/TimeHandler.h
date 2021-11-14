#ifndef TIMEHANDLER_H
#define TIMEHANDLER_H

#include "time.h"
#include <WString.h>
#include <cstddef>

// const char *ntpServer = "pool.ntp.org";
// const long gmtOffset_sec = 3600;
// const int daylightOffset_sec = 0;

template <size_t length = 20>
class IsoDateTimeString
{
    char datetime[length];

public:
    IsoDateTimeString(const char *datetime)
    {
        strcpy(this->datetime, datetime);
    }

    const char *getDateTime()
    {
        return datetime;
    }

    size_t getStrLength()
    {
        return length;
    }
};

template <size_t urlSize = 64>
class TimeHandler
{
    enum TimeHandlerStatus
    {
        INIT = -1,
        OK,
        GET_TIME_ERROR,
        CONNECTION_ERROR,

    };

    char ntpServer[urlSize];
    long timeOffset;
    long savingOffest;
    TimeHandlerStatus status = TimeHandlerStatus::INIT;

public:
    TimeHandler(const char *ntpServer, int timeZone = 0, bool daylightSaving = false) : timeOffset(timeZone * 3600), savingOffest(daylightSaving * 3600)
    {
        strcpy(this->ntpServer, ntpServer);
    }

    bool connect()
    {
        struct tm timeinfo;
        for (int i = 0; i < 5; ++i)
        {
            configTime(this->timeOffset, this->savingOffest, this->ntpServer);
            if (getLocalTime(&timeinfo))
            {
                this->status = TimeHandlerStatus::OK;
                return true;
            }
            this->status = TimeHandlerStatus::CONNECTION_ERROR;
            delay(50);
        }
        return false;
    }

    IsoDateTimeString<> getIsoTime()
    {
        struct tm timeinfo;
        if (!getLocalTime(&timeinfo))
        {
            //Serial.println("Failed to obtain time");
            status = TimeHandlerStatus::GET_TIME_ERROR;
            return IsoDateTimeString<>("");
        }
        char time[20];
        sprintf(time, "%04d-%02d-%02dT%02d:%02d:%02d", timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
        return IsoDateTimeString<>(time);
    }
};

#endif //TIMEHANDLER_H