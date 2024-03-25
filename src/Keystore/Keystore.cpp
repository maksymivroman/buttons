//
// Created by rmaks on 25-Dec-23.
//

#include "Keystore.h"


Keystore::Keystore(unsigned int maxKeystoreItemsToStore) {
    this->maxKeystoreItems = maxKeystoreItemsToStore;
}

void Keystore::clearKeystore() {
    const unsigned int totalCount = this->items.size();
    this->items.clear();
    logger.log("[Keystore]: Clear... ", "removed ", totalCount, " items");
}

void Keystore::addItem(String key, String value) {
    //TODO max items
    auto addResult = this->items.insert({key, value});

    if (!addResult.second) {
        this->items[key] = value;
        logger.logSerial("[Keystore]: Update item - ", key);
    }
}

bool Keystore::removeItem(String &key) {
    auto search = this->items.find(key);
    if (search != this->items.end()) {
        this->items.extract(key);
        logger.log("[Keystore]: Remove item - ", key, ". Success");
        return true;
    }
    logger.log("[Keystore]: Remove item - ", key, ". Item not exist!");
    return false;
}

String Keystore::itemValue(String &key) {
    auto search = this->items.find(key);
    if (search != this->items.end()) {
        return search->second;
    }
    return this->emptyReturnValue;
}

Keystore::KeystoreItems Keystore::keystoreItems() const {
    return this->items;
}

unsigned int Keystore::currentItemsCount() const {
    return this->items.size();
}
