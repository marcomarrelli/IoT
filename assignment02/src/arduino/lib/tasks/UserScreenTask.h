#ifndef __USER_SCREEN_TASK__
#define __USER_SCREEN_TASK__

#include "Task.h"
#include "../components/physical/LCD.h"

//#define DEFAULT_MSG "Press Open to enter waste"

class UserScreenTask: public Task {
public:
    UserScreenTask(LCD& lcd);  
    void init(int period);  
    void tick();

private:
    String msg;
    long time;
    bool clear;
    int _pin;
    enum State { PRINT_DEFAULT, PRINT};
    State _state;
    LCD* _lcd;
    String DEFAULT_MSG = "Press Open to enter waste";
};

#endif