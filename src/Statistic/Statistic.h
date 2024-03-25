//
// Created by rmaks on 31-Jan-24.
//

#ifndef EVENT_BUTTON_STATISTIC_H
#define EVENT_BUTTON_STATISTIC_H

#include <Arduino.h>

typedef enum {
    STAT_HTTP_POST,
    STAT_HTTP_GET
} http_stat_method;

typedef enum  {
    STAT_SIMPLE, STAT_EXTENDED
} StatisticLevel;

class Statistic {

public:
    void initStat(String apiUrl, http_stat_method method = STAT_HTTP_POST, unsigned int statisticLevel = 0);
    void sendStat(String data);

private:
    String apiUrl;
    http_stat_method method;
    bool sendExtendedData;
    bool enabled;

    String prepareData(const String &data, bool asUrlParams = false);
    String extendedData(bool asUrlParams = false);

    int httpPost(String &url, const String &payload);
    int httpsPost(String &url, const String &payload);

    int httpGet(String &url, const String &payload);
    int httpsGet(String &url, const String &payload);
};


#endif //EVENT_BUTTON_STATISTIC_H
