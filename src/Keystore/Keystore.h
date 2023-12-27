//
// Created by rmaks on 25-Dec-23.
//

#ifndef EVENT_BUTTON_KEYSTORE_H
#define EVENT_BUTTON_KEYSTORE_H

#include <Arduino.h>
#include <map>
#include "Global/Global.hpp"

class Keystore {
    typedef std::map<String, String> KeystoreItems;

public:
    explicit Keystore(unsigned int maxKeystoreItemsToStore);

    void clearKeystore();
    void addItem(String key, String value);

    bool removeItem(String &key);
    String itemValue(String &key);

    KeystoreItems keystoreItems() const;

private:
    std::map<String, String> items;

    const String emptyReturnValue = "";
    unsigned int maxKeystoreItems;

};


#endif //EVENT_BUTTON_KEYSTORE_H
