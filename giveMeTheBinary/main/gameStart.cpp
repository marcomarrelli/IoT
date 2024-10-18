#include "gameStart.h"
#include "Arduino.h"
#include <avr/sleep.h>

static int fadeAmount = 5;
static int currIntensity = 0;
int flagSleep = false;

//funzione che si occupa del fading del les
void fade (){
  analogWrite(RED_LED_PIN, currIntensity);   
  currIntensity = currIntensity + fadeAmount;
  if (currIntensity == 0 || currIntensity == 255) {
    fadeAmount = -fadeAmount ; 
  }     
  delay(15);                               
}

//funzione che si occupa di camiare la flag che manda in deep sleep arduino
void setFlagSleep(){
  flagSleep = !flagSleep;
}

//funzione che sveglia erduino
void wakeUp(){}

//funzione che manda in sleep mode arduino
void deepSleep(){
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);  
  sleep_enable();
  //va in sleep mode e il programma si blocca qua
  sleep_mode();
  //viene disabilitato solo quando c'è un interrupt
  sleep_disable();   
  Serial.println("WAKE UP");
}