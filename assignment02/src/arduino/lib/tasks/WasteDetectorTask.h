#ifndef __WASTE_DETECTOR_TASK__
#define __WASTE_DETECTOR_TASK__

#include "Task.h"
#include "UserScreenTask.h"
#include "../components/Sonar.h"

#define MSG_FULL "Container full"

class WasteDetectorTask : public Task {
public:
    WasteDetectorTask(Sonar& sonar, String& msg, bool& clear);
    void init(int period);
    void tick();
    bool full;

private:
    int _pin;
    enum State { NOT_FULL, FULL};
    State _state;
    Sonar* _sonar;
    String* _msg;
    bool* _clear;
};

#endif