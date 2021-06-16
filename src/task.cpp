#include "tasks.hpp"


void Task::run(const char * name, uint8_t priority, uint8_t core){
    _priority = priority;
    xTaskCreatePinnedToCore(_run, name, _stackSize, (void *) this, _priority, &_handler, core);  
}

void Task::_run(void* args){
    Task * task = static_cast<Task *>(args);
    /// Last wakeup

    static TickType_t delay, wakeUp;
    while ( task->_running ){
        wakeUp = xTaskGetTickCount();
        /// Call inner loop of sons
        task->_loop();

        /// Try ensure constant rate
        taskYIELD();
        delay = task->_period - (xTaskGetTickCount() - wakeUp);
        if(delay <= 0 || delay > task->_period) delay = task->_period;
        vTaskDelay(delay);
    }
}