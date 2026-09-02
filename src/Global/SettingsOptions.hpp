//
// Created for Event Button Settings Options
//

#ifndef EVENT_BUTTON_SETTINGSOPTIONS_HPP
#define EVENT_BUTTON_SETTINGSOPTIONS_HPP

#include <Arduino.h>
#include <ArduinoJson.h>
#include <TZ.h>
#include "Global.hpp"
#include "Statistic/Statistic.h"

static const TimeZoneEntry TIMEZONES[] = {
        {TimeZoneId::London,     "(UTC+00:00) London, Dublin (GMT/BST)",            "GMT0BST,M3.5.0/1,M10.5.0"},
        {TimeZoneId::Berlin,     "(UTC+01:00) Berlin, Warsaw, Paris (CET/CEST)",    "CET-1CEST,M3.5.0,M10.5.0/3"},
        {TimeZoneId::Kyiv,       "(UTC+02:00) Kyiv, Athens, Bucharest (EET/EEST)",  "EET-2EEST,M3.5.0/3,M10.5.0/4"},
        {TimeZoneId::Istanbul,   "(UTC+03:00) Istanbul, Riyadh",                    "+03-3"},
        {TimeZoneId::Dubai,      "(UTC+04:00) Dubai, Baku (GST/AZT)",               "+04-4"},
        {TimeZoneId::Kolkata,    "(UTC+05:30) Delhi, Mumbai, Kolkata (IST)",        "IST-5:30"},
        {TimeZoneId::Shanghai,   "(UTC+08:00) Beijing, Singapore, Hong Kong (CST)", "CST-8"},
        {TimeZoneId::Tokyo,      "(UTC+09:00) Tokyo, Seoul (JST/KST)",              "JST-9"},
        {TimeZoneId::NewYork,    "(UTC-05:00) New York, Toronto (EST/EDT)",         "EST5EDT,M3.2.0,M11.1.0"},
        {TimeZoneId::Chicago,    "(UTC-06:00) Chicago, Mexico City (CST/CDT)",      "CST6CDT,M3.2.0,M11.1.0"},
        {TimeZoneId::LosAngeles, "(UTC-08:00) Los Angeles, Vancouver (PST/PDT)",    "PST8PDT,M3.2.0,M11.1.0"},
        {TimeZoneId::Utc,        "(UTC+00:00) Fixed UTC",                           "UTC0"}
};
const size_t TIMEZONE_COUNT = sizeof(TIMEZONES) / sizeof(TIMEZONES[0]);

inline String GET_TIMEZONE(TimeZoneId timezone) {
    auto index = static_cast<uint8_t>(timezone);
    if (index >= static_cast<uint8_t>(TimeZoneId::COUNT)) {
        index = static_cast<uint8_t>(TimeZoneId::Kyiv);
    }
    return TIMEZONES[index].tzRule;
}

template<typename TEnum>
struct OptionItem {
    TEnum value;
    const char *label;
};

static const OptionItem<BUTTON_WIFI_MODE> WIFI_MODE_OPTIONS[] = {
        {BUTTON_WIFI_MODE::AUTO, "Auto"},
        {BUTTON_WIFI_MODE::MODE_11B, "11B"},
        {BUTTON_WIFI_MODE::MODE_11G, "11G"},
        {BUTTON_WIFI_MODE::MODE_11N, "11N"}
};
const size_t WIFI_MODE_OPTIONS_COUNT = sizeof(WIFI_MODE_OPTIONS) / sizeof(WIFI_MODE_OPTIONS[0]);

static const OptionItem<LoggerLevel> LOGGER_LEVEL_OPTIONS[] = {
        {LoggerLevel::SERIAL_AND_LOCAL, "Serial & Local"},
        {LoggerLevel::LOGGER_SERIAL, "Serial (115200 8-N-1)"},
        {LoggerLevel::LOGGER_LOCAL, "Local log (/logs)"}
};
const size_t LOGGER_LEVEL_OPTIONS_COUNT = sizeof(LOGGER_LEVEL_OPTIONS) / sizeof(LOGGER_LEVEL_OPTIONS[0]);

static const OptionItem<StatisticLevel> STATISTIC_LEVEL_OPTIONS[] = {
        {StatisticLevel::STAT_SIMPLE, "Simple"},
        {StatisticLevel::STAT_EXTENDED, "Extended"}
};
const size_t STATISTIC_LEVEL_OPTIONS_COUNT = sizeof(STATISTIC_LEVEL_OPTIONS) / sizeof(STATISTIC_LEVEL_OPTIONS[0]);

namespace SettingsOptions {

    template<typename TEnum>
    inline void populateJsonArray(JsonArray &arr, const OptionItem<TEnum> options[], size_t count) {
        for (size_t i = 0; i < count; ++i) {
            JsonObject item = arr.createNestedObject();
            item["value"] = static_cast<unsigned int>(options[i].value);
            item["label"] = options[i].label;
        }
    }

    inline void populateOptionsJson(JsonObject &options) {
        JsonArray tzArr = options.createNestedArray("timezones");
        for (auto i : TIMEZONES) {
            JsonObject item = tzArr.createNestedObject();
            item["value"] = static_cast<unsigned int>(i.id);
            item["label"] = i.label;
        }

        JsonArray wmArr = options.createNestedArray("wiFiModes");
        populateJsonArray(wmArr, WIFI_MODE_OPTIONS, WIFI_MODE_OPTIONS_COUNT);

        JsonArray llArr = options.createNestedArray("loggerLevels");
        populateJsonArray(llArr, LOGGER_LEVEL_OPTIONS, LOGGER_LEVEL_OPTIONS_COUNT);

        JsonArray slArr = options.createNestedArray("statisticLevels");
        populateJsonArray(slArr, STATISTIC_LEVEL_OPTIONS, STATISTIC_LEVEL_OPTIONS_COUNT);
    }

} // namespace SettingsOptions

#endif //EVENT_BUTTON_SETTINGSOPTIONS_HPP
