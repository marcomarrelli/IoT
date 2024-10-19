#include <EnableInterrupt.h>
#include <LiquidCrystal_I2C.h>
#include <avr/sleep.h>
#include <TimerOne.h>
#include "gmtbc.h"

#define WAITING_TIME 10000000

#define b1 5
#define b2 4
#define b3 3
#define b4 2

#define POT_PIN A0

int current = -1;

bool __welcome_printed = false;
bool __go_printed = false;
bool __chooseLevel_printed = false;

bool start = false;
bool chooseLevel = false;
bool levelSet = false;

int level, F;

bool b1Pressed = false;

LiquidCrystal_I2C lcd(0x27, 20, 4);

void b1Handler() {
  if(chooseLevel) b1Pressed = true;

  else if(start) {
    wakeUp();
    
    Timer1.detachInterrupt();
    chooseLevel = true;
    start = false;
  }
}

void clearScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);

  __welcome_printed = false;
  __go_printed = false;
}

void welcomeScreen() {
  clearScreen();

  lcd.print("Welcome to GMB!");
  lcd.setCursor(0, 1);
  lcd.print("Press B1 to Start");

  __welcome_printed = true;
}

int writeLevel(int val) {
  return (val/256)+1;
}

void updateLevelToBeChosen() {
  if(current == -1) {
    clearScreen();
    lcd.setCursor(0, 0);
    lcd.print("Choose Level: ");
  }

  int newValue = analogRead(POT_PIN);
  if((newValue/256) != (current/256)) {
    current = newValue;
    lcd.setCursor(14, 0);
    lcd.print((current/256)+1);
  }

  lcd.setCursor(0, 2);
  lcd.print("Press B1 to Select.");
}

void chosenLevelScreen() {
  clearScreen();

  lcd.print("You Chose Level ");
  lcd.setCursor(17, 0);
  lcd.print(level);
  lcd.setCursor(0, 2);
  lcd.print("Go!");

  delay(3000);
}

void goScreen() {
  clearScreen();

  lcd.print("Welcome to GMB!");
  lcd.setCursor(0, 1);
  lcd.print("Press B1 to Start");

  __go_printed = true;
}

void setup(){  
  Serial.begin(9600);

  pinMode(b1,INPUT);
  enableInterrupt(b1, b1Handler, FALLING);

  pinMode(b2, INPUT);
  enableInterrupt(b2, wakeUp, FALLING);
  
  pinMode(b3, INPUT);
  enableInterrupt(b3, wakeUp, FALLING);
  
  pinMode(b4, INPUT);
  enableInterrupt(b4, wakeUp, FALLING);

  pinMode(RED_LED_PIN, OUTPUT); 

  start = true;

  lcd.init();
  lcd.backlight(); 

  Timer1.initialize(WAITING_TIME);
  Timer1.attachInterrupt(setFlagSleep);
}

void loop(){
  if(start) {
    if(!__welcome_printed) {
      welcomeScreen();
    }

    fade();

    if(flagSleep) {
      clearScreen();
      deepSleep();
    }
  }
  if(chooseLevel) {
    if (!b1Pressed) {
      updateLevelToBeChosen();
    }
    if (b1Pressed && !levelSet) {
      level = writeLevel(analogRead(POT_PIN));
      F = level;
      
      chosenLevelScreen();

      levelSet = true;
      chooseLevel = false;
    }
  }
  else if(!chooseLevel && levelSet) {
    
  }
}