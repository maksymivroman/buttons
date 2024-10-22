//
// Created by rmaks on 16-Oct-24.
//

#ifndef EVENT_BUTTON_TRIGGER_H
#define EVENT_BUTTON_TRIGGER_H


class Trigger {

public:
    Trigger() : _state{false} {}

    explicit operator bool() const {
        return _state;
    }

    bool operator==(const bool &compare) const {
        return _state == compare;
    }

    bool operator!=(const bool &compare) const {
        return _state != compare;
    }

    void set() {
        _state = true;
    }
    bool setIf(const bool &predicate) {
        if(predicate) {
            _state = true;
            return true;
        }
        return false;
    }
    void reset() {
        _state = false;
    }
    bool get() const {
        return _state;
    }

private:
    bool _state;

};

class ButtonTriggers {

public:
    template<typename... Args>
    static void reset(Trigger &trigger, const Args &... arguments) {
        trigger.reset();
        reset(arguments...);
    }

    static void reset(Trigger &trigger) {
        trigger.reset();
    }

};


#endif //EVENT_BUTTON_TRIGGER_H
