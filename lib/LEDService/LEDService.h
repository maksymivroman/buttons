#ifndef EVENT_BUTTON_LEDSERVICE_H
#define EVENT_BUTTON_LEDSERVICE_H


class LEDService {
public:
    void pinConfig(int r, int g, int b);
    void lightOnRed(bool on);
    void lightOnGreen(bool on);
    void lightOnBlue(bool on);
    void blinkWarn();
    void blinkPrimary();
    void blinkDone();

private:
    int pinRed;
    int pinGreen;
    int pinBlue;

    void switchPin(int pin, int state) const;
    void blink(int pin, int count);

};


#endif
