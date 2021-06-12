#include "tasks.hpp"

void Task::run(const char * name, uint8_t priority, uint8_t core){
    _priority = priority;
    xTaskCreatePinnedToCore(_run, name, _stackSize, NULL, _priority, &_handler, core);  
}

void Task::_run(void* args){
    /// Last wakeup
    static TickType_t wakeUp = xTaskGetTickCount();

    while ( _running ){
        /// Call inner loop of sons
        _loop();

        /// Ensure constant rate
        taskYIELD();
        vTaskDelayUntil(&wakeUp, _period);
    }
}