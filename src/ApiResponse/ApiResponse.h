//
// Created for Event Button API responses
//

#ifndef EVENT_BUTTON_APIRESPONSE_H
#define EVENT_BUTTON_APIRESPONSE_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>

class ApiResponse {
public:
    explicit ApiResponse(size_t bufferSize = 1024, int statusCode = 200);
    ~ApiResponse();

    ApiResponse(const ApiResponse &) = delete;
    ApiResponse &operator=(const ApiResponse &) = delete;
    ApiResponse(ApiResponse &&other) noexcept;
    ApiResponse &operator=(ApiResponse &&other) noexcept;

    JsonObject getRoot();
    JsonObject getResult();
    void setStatus(int statusCode);
    void setError(const String &error);
    void send(AsyncWebServerRequest *request);

    template<typename PopulateFn>
    static void sendSuccess(AsyncWebServerRequest *request, PopulateFn populateResult, size_t bufferSize = 1024) {
        auto *response = new AsyncJsonResponse(false, bufferSize);
        JsonObject root = response->getRoot();
        root["status"] = 200;
        root["error"] = nullptr;
        JsonObject result = root.createNestedObject("result");
        populateResult(result);
        response->setLength();
        request->send(response);
    }

    static void sendSuccess(AsyncWebServerRequest *request, const String &message = "OK", size_t bufferSize = 256);

    static void sendError(AsyncWebServerRequest *request, int statusCode, const String &errorMessage, size_t bufferSize = 256);

private:
    AsyncJsonResponse *_response{nullptr};
    JsonObject _root;
    JsonObject _result;
    bool _hasResult{false};
};

#endif //EVENT_BUTTON_APIRESPONSE_H
