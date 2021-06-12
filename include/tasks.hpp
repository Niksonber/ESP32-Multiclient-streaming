#pragma once
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

/// Mother class to Managers
class Task{
public:
    
    /// Repeat intenal loop periodicaly
    void loop();

    /// Ends Task
    void stop();

protected:

    /// internal loop of managers
    virtual void _loop();
    
    /// Used to stop loop
    bool _running;

    /// Repetition period in ticks
    TickType_t _period;

    /// Handler to task
    TaskHandle_t _handler;
};