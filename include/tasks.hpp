#pragma once
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

/// Mother class to Managers
class Task{
public:
    
    /// Repeat intenal loop periodicaly
    void run(void* args);

    /// Ends Task
    void stop();

    /// Getter method for required stack size
    inline uint32_t size();

    /// Getter method for required priority
    inline uint8_t priority();

    /// Handler to task
    TaskHandle_t handler;

protected:
    
    /// Used to stop loop
    bool _running;

    /// Repetition period in ticks
    TickType_t _period;

    /// Task priotity
    uint8_t _priority;

    /// Required stack size
    uint32_t _stackSize;

    /// internal loop of managers
    virtual void _loop();

};

uint32_t Task::size(){
    return _stackSize;
}

uint8_t Task::priority(){
    return _priority;
}