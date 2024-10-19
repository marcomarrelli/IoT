#ifndef __FADIN_H__
#define __FADING_H__

extern int flagSleep;

#define RED_LED_PIN 6

void fade();
void setFlagSleep();
void wakeUp();
void deepSleep();

#endif