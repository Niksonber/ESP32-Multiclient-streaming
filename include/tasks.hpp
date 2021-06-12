#pragma once
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

/// Mother class to Managers
class Task{
public:
    
    /// Create task that runs periodicaly principal method @param name name of task
    /// @param priority (optional) default 2, @param core (optinal) default 1
    static void run(const char * name, uint8_t priority = 2, uint8_t core = 1);

    /// Ends Task
    void stop();

    /// Getter method for required stack size
    inline uint32_t size();

    /// Getter method for required priority
    inline uint8_t priority();

protected:

    /// Used to stop loop
    static bool _running;

    /// Repetition period in ticks
    static TickType_t _period;

    /// Task priotity
    static uint8_t _priority;

    /// Required stack size
    static uint32_t _stackSize;

    /// Handler to task
    static TaskHandle_t _handler;

    /// internal loop of managers
    static void _loop();

    /// Repeat intenal loop periodicaly
    static void _run(void* args);

};

uint32_t Task::size(){
    return _stackSize;
}

uint8_t Task::priority(){
    return _priority;
}