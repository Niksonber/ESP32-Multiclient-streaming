#include "tasks.hpp"


void Task::run(const char * name, uint8_t priority, uint8_t core){
    _priority = priority;
    xTaskCreatePinnedToCore(_run, name, _stackSize, (void *) this, _priority, &_handler, core);  
}

void Task::_run(void* args){
    Task * task = static_cast<Task *>(args);
    /// Last wakeup
    static TickType_t wakeUp = xTaskGetTickCount();

    while ( task->_running ){
        /// Call inner loop of sons
        task->_loop();

        /// Ensure constant rate
        taskYIELD();
        vTaskDelayUntil(&wakeUp, task->_period);
    }
}