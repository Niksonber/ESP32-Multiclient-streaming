#pragma once
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "FreeRTOSConfig.h"

/// Mother class to Managers
class Task{
public:
    /// Task name
    const char * name;
    
    /// Create task that runs periodicaly principal method @param name name of task
    /// @param priority (optional) default 2, @param core (optinal) default 1
    void run(const char * name, uint8_t priority = 2, uint8_t core = 1);

    /// Ends Task
    void stop();
    
    // Suspend task
    void suspend();

    // Resume task
    bool resume();

    /// Getter method for required stack size
    inline uint32_t size();

    /// Getter method for required priority
    inline uint8_t priority();

protected:

    /// Used to stop task
    bool _running =  true;

    // Used to check status
    bool _suspended =  false;

    /// Repetition period in ticks
    TickType_t _period;

    /// Task priotity
    uint8_t _priority;

    /// Required stack size
    uint32_t _stackSize = 4 * 1024;

    /// Handler to task
    TaskHandle_t _handler;

    /// internal loop of managers
    virtual void _loop();

    /// Repeat intenal loop periodicaly
    static void _run(void* args);

};

uint32_t Task::size(){
    return _stackSize;
}

uint8_t Task::priority(){
    return _priority;
}