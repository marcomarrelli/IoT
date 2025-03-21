#include "Monitoring.h"

Monitoring::Monitoring() : _ok(CONFIG_LED_OK_PIN), _error(CONFIG_LED_ERROR_PIN), _thermistor(CONFIG_THERMISTOR_PIN) {
    _problem = true;
    setProblem(); // Starts as problem active
}

void Monitoring::setProblem() {
    _ok.off();
    _error.on();
}

void Monitoring::clearProblem() {
    _ok.on();
    _error.off();
}

bool Monitoring::hasProblem() {
    return _problem;
}

float Monitoring::getTemperature() {
    float currentTemperature = _thermistor.read();
    return currentTemperature;
}


void Monitoring::eventLoop() {
    float _currentTemperature = getTemperature();
    
    bool _isCommunicating = communication->connected(); // Check if the device is connected to the network, if not, raise a problem

    if(!_isCommunicating) {
        _problem = true; // Same as writing "_problem = !_isCommunicating",
    }
    else {
        _problem = false; // but we want to make it explicit for the reader ;)
    }

    if(hasProblem()) {
        setProblem();
        Serial.println("Connection Problem Detected!");
    }
    else {
        clearProblem();

        // Send the temperature to the MQTT server
        String message = "temperature:" + String(_currentTemperature);
        communication->sendMessage(message.c_str());
    }
}
