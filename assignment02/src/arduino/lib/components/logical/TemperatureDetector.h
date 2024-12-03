#ifndef __A02_TEMPERATURE_DETECTOR__
#define __A02_TEMPERATURE_DETECTOR__

#include "../physical/Thermistor.h"
#include "../../communication/MsgService.h"
#include "../../Constants.h"

/**
 * @brief Class for the temperature detector
 * @implements Thermistor
 */
class TemperatureDetector : public Thermistor {
public:
    /**
     * @brief Creates New Temperature Detector Instance
     * @param pin Analog Input for Thermistor
     */
    TemperatureDetector(int pin) : Thermistor(pin) {
        // _highTemperatureAlarm = false;
    }

    /**
     * @brief Read the Temperature from the Thermistor. 
     * 
     * Convert ADC Value to Equivalent Voltage and Gives Output of 10mv/°C
     * @return Temperature in Celsius
     */
    float read() override {
        float temperature = analogRead(_pin);
        temperature = (temperature*4.88);
        temperature = (temperature/10);
        return temperature;
    }

    /**
     * @brief Check if the temperature is overheated
     * @return if the temperature is overheated
     */
    bool getTemperatureAlarm() {
        return _highTemperatureAlarm;
    }

    /**
     * @brief Set the alarm for the temperature
     * @param alarm if the alarm is set
     */
    void setTemperatureAlarm(bool alarm) {
        _highTemperatureAlarm = alarm;
        
        Serial.print(F("[Alarm:Temperature]"));
        Serial.println(_highTemperatureAlarm ? F("true") : F("false"));
        Serial.flush();
    }

private:
    bool _highTemperatureAlarm = false;
};

#endif