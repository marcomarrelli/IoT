#include <Arduino.h>

#include "../lib/components/physical/Button.h"
#include "../lib/components/logical/Window.h"
#include "../lib/components/physical/Potentiometer.h"

#include "../lib/tasks/WindowTask.h"
#include "../lib/tasks/DisplayTask.h"
#include "../lib/tasks/ModeTask.h"

#include "../lib/tasks/Scheduler.h"



Scheduler scheduler; /** Schedule Object for Managing Tasks */

Button* modeButton; /** [Pointer] Close Button */

Potentiometer* potentiometer; /** [Pointer] Potentiometer */

ServoMotor* servo; /** [Pointer] Servo Motor */
Window* window; /** [Pointer] Window (ServoMotor Handler Class) */

SystemManager* systemManager; /** System Mode */

WindowTask* windowManager; /** [Pointer] Window Task */
DisplayTask* displayManager; /** [Pointer] User Display Task */
ModeTask* modeManager; /** [Pointer] Mode Task */



/**
 * \brief Setup Function
 */
void setup() {
    Serial.begin(CONFIG_SERIAL_BAUD_RATE);
    Serial.flush();

    systemManager = new SystemManager();

    modeButton = new Button(CONFIG_BUTTON_PIN);

    potentiometer = new Potentiometer(CONFIG_POTENTIOMETER_PIN);

    servo = new ServoMotor(CONFIG_SERVO_PIN);
    window = new Window(servo);

    windowManager = new WindowTask(window, potentiometer, systemManager);
    windowManager->init(2000);

    displayManager = new DisplayTask(window, systemManager);
    displayManager->init(2000);

    modeManager = new ModeTask(modeButton, systemManager);
    modeManager->init(2000);

    scheduler.init(500);

    scheduler.addTask(windowManager);
    scheduler.addTask(displayManager);
    scheduler.addTask(modeManager);
}

/**
 * \brief Main Loop
 */
void loop() {
    scheduler.schedule();
}
