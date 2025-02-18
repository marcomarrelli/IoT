#include "SerialHandler.h"

String content; /** Serial Content Buffer */

SerialHelperObject SerialHelper; /** Global Serial Helper Object */

/**
 * \brief Initializes the Serial Helper Object
 */
void SerialHelperObject::init() {
    content.reserve(256);
    content = "";

    _temperature = 0.0f;
    _aperture = 0;
    _mode = SystemManager::Mode::MANUAL;

    _temperatureAvailable = false;
    _apertureAvailable = false;
    _modeAvailable = false;
}

/**
 * \brief Gets the Current Temperature Value
 */
float SerialHelperObject::getTemperature() {
    if(_temperatureAvailable) {
        _temperatureAvailable = false;
        return _temperature;
    }

    return -1.0f;
}

/**
 * \brief Gets the Current Window Aperture Value
 */
int SerialHelperObject::getAperture() {
    if(_apertureAvailable) {
        _apertureAvailable = false;
        return _aperture;
    }

    return -1;
}

/**
 * \brief Gets the Current System Mode
 */
SystemManager::Mode SerialHelperObject::getMode() {
    if(_modeAvailable) {
        _modeAvailable = false;
        return _mode;
    }

    return SystemManager::Mode::MANUAL;
}

/**
 * \brief Sets the Temperature Value
 */ 
void SerialHelperObject::setTemperature(float temperature) {
    _temperature = temperature;
    _temperatureAvailable = true;
}

/**
 * \brief Sets the Window Aperture Value
 */
void SerialHelperObject::setAperture(int aperture) {
    _aperture = aperture;
    _apertureAvailable = true;
}

/**
 * \brief Sets the System Mode
 */
void SerialHelperObject::setMode(SystemManager::Mode mode) {
    _mode = mode;
    _modeAvailable = true;
}

/**
 * \brief [Override] Function Called Whenever a New Message is Available (in the Serial Port).
 * 
 * \brief Uses '10' (\n) and '13' (\r) as EOS (End of String) Characters.
 */
void serialEvent() {
    content = "";

    while(Serial.available()) {
        char ch = (char) Serial.read();

        if (ch == 10 || ch == 0 || ch == 13) {
            Serial.print("Received: ");
            Serial.println(content);

            if(content.startsWith("temperature:")) {
                SerialHelper.setTemperature(content.substring(12).toFloat());
                content = "";
            }
            else if(content.startsWith("mode:")) {
                String mode = content.substring(5);
                mode.toLowerCase();

                if(mode == "manual") {
                    SerialHelper.setMode(SystemManager::Mode::MANUAL);
                }
                else if(mode == "auto") {
                    SerialHelper.setMode(SystemManager::Mode::AUTOMATIC);
                }

                content = "";
            }
            else if(content.startsWith("aperture:")) {
                SerialHelper.setAperture(content.substring(9).toInt());
                content = "";
            }
        }
        else {
            content += ch;
        }
    }
}
