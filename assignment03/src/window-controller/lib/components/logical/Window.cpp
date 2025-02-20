#include "Window.h"

Window::Window(ServoMotor* servoMotor) {
    _servoMotor = servoMotor;
    _angle = CONFIG_WINDOW_CLOSED;   
}

void Window::setWindowAperture(int angle) {
    _percentage = angle;
    _angle = map(_percentage, 0, 100, CONFIG_WINDOW_CLOSED, CONFIG_WINDOW_OPEN);

    _servoMotor->on();
    _servoMotor->setPosition(_angle);
    delay(80);
    _servoMotor->off();
}

int Window::getWindowAperture() {
    return _angle;
}

int Window::getPercentage() {
    return _percentage;
}

bool Window::isOpen() {
    return (_angle == CONFIG_WINDOW_CLOSED);
}
