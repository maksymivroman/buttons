//
// Created by rmaks on 19-Aug-26.
//

#ifndef EVENT_BUTTON_TIMEZONES_HPP
#define EVENT_BUTTON_TIMEZONES_HPP

#include "../Global/Global.hpp"

#include <TZ.h>

static const TimeZoneEntry TIMEZONES[] PROGMEM = {
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

static String GET_TIMEZONE(TimeZoneId timezone) {
    auto index = static_cast<uint8_t>(timezone);
    if (index >= static_cast<uint8_t>(TimeZoneId::COUNT)) {
        index = static_cast<uint8_t>(TimeZoneId::Kyiv);
    }
    return TIMEZONES[index].tzRule;
}


#endif //EVENT_BUTTON_TIMEZONES_HPP
