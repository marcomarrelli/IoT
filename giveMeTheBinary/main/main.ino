#include <EnableInterrupt.h>
#include <avr/sleep.h>
#include <TimerOne.h>
#include "gameStart.h"

#define b1 5
#define b2 4
#define b3 3
#define b4 2

bool start;
bool game;

void b1Handler(){
  Timer1.detachInterrupt();
  game = true;
  start = false;
}

void setup(){  
  Serial.begin(9600);

  pinMode(b1,INPUT);
  enableInterrupt(b1, b1Handler, FALLING);

  pinMode(b2,INPUT);
  enableInterrupt(b2, wakeUp, FALLING);

  pinMode(b3,INPUT);
  enableInterrupt(b3, wakeUp, FALLING);

  pinMode(b4,INPUT);
  enableInterrupt(b4, wakeUp, FALLING);

  pinMode(RED_LED_PIN, OUTPUT); 

  start = true;
  
  //tra 10 secondi il timer chiama la funzione che imposta il flag di deep sleep a true
  Timer1.initialize(5000000); 
  Timer1.attachInterrupt(setFlagSleep);
}

void loop(){
  if(start){
    //se il flag di dormire è true va in deep sleep e setta il flag a false
    if(flagSleep){
      deepSleep();
      setFlagSleep();
    }
    fade();
  }
  if(game){
    Serial.println("The game is starting");
  }
}