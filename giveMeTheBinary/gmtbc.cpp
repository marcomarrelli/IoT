#include "gmtbc.h"
#include "Arduino.h"
#include <avr/sleep.h>

static int fadeAmount = 5;
static int currIntensity = 0;

int flagSleep = false;

void fade() {
  analogWrite(RED_LED_PIN, currIntensity);
  currIntensity = currIntensity + fadeAmount;
  if (currIntensity == 0 || currIntensity == 255) {
    fadeAmount = -fadeAmount;
  }
  delay(15);
}

void setFlagSleep() {
  flagSleep = !flagSleep;
}

void wakeUp() {
  if(flagSleep) flagSleep = false;
}

void deepSleep() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();  
  
  sleep_disable();   
  
  Serial.println("WAKE UP");
}