#include "Button.h"

#include "../../Constants.h"

Button::Button(int pin) : _pin(pin), _previous(0), _state(LOW) {
    pinMode(pin, INPUT);
}

bool Button::isPressed() {
    long _delta = millis() - _previous;

    if(_delta > Constants::Button::DEBOUNCE_TIME) {
        _state = bool(digitalRead(_pin));
        _previous = millis();
    }
    
    return _state;
};