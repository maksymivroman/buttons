//
// Created for Event Button API responses
//

#include "ApiResponse.h"

ApiResponse::ApiResponse(size_t bufferSize, int statusCode) {
    _response = new AsyncJsonResponse(false, bufferSize);
    _root = _response->getRoot();
    _root["status"] = statusCode;
    _root["error"] = nullptr;
    _response->setCode(statusCode);
}

ApiResponse::~ApiResponse() {
    if (_response != nullptr) {
        delete _response;
        _response = nullptr;
    }
}

ApiResponse::ApiResponse(ApiResponse &&other) noexcept
    : _response(other._response), _root(other._root), _result(other._result), _hasResult(other._hasResult) {
    other._response = nullptr;
}

ApiResponse &ApiResponse::operator=(ApiResponse &&other) noexcept {
    if (this != &other) {
        if (_response != nullptr) {
            delete _response;
        }
        _response = other._response;
        _root = other._root;
        _result = other._result;
        _hasResult = other._hasResult;
        other._response = nullptr;
    }
    return *this;
}

JsonObject ApiResponse::getRoot() {
    return _root;
}

JsonObject ApiResponse::getResult() {
    if (!_hasResult) {
        _result = _root.createNestedObject("result");
        _hasResult = true;
    }
    return _result;
}

void ApiResponse::setStatus(int statusCode) {
    _root["status"] = statusCode;
    if (_response != nullptr) {
        _response->setCode(statusCode);
    }
}

void ApiResponse::setError(const String &error) {
    _root["error"] = error;
}

void ApiResponse::send(AsyncWebServerRequest *request) {
    if (_response != nullptr && request != nullptr) {
        _response->setLength();
        request->send(_response);
        _response = nullptr;
    }
}

void ApiResponse::sendSuccess(AsyncWebServerRequest *request, const String &message, size_t bufferSize) {
    auto *response = new AsyncJsonResponse(false, bufferSize);
    JsonObject root = response->getRoot();
    root["status"] = 200;
    root["error"] = nullptr;
    root["result"] = message;
    response->setLength();
    request->send(response);
}

void ApiResponse::sendError(AsyncWebServerRequest *request, int statusCode, const String &errorMessage, size_t bufferSize) {
    auto *response = new AsyncJsonResponse(false, bufferSize);
    JsonObject root = response->getRoot();
    root["status"] = statusCode;
    root["result"] = nullptr;
    root["error"] = errorMessage;
    response->setCode(statusCode);
    response->setLength();
    request->send(response);
}
