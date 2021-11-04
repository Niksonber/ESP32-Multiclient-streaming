#include "tasks.hpp"

void Task::run(const char * nameIn, uint8_t priority, uint8_t core){
    name = nameIn;
    _priority = priority;
    xTaskCreatePinnedToCore(_run, name, _stackSize, (void *) this, _priority, &_handler, core);  
}

void Task::stop(){
    _running = false;
}

void Task::suspend(){
    _suspended = true;
     vTaskSuspend(_handler);
}

bool Task::resume(){
    if(!_suspended) return false;
    _suspended = false;
    vTaskResume(_handler);
    return true;
}

void Task::_run(void* args){
    Task * task = static_cast<Task *>(args);
    /// Last wakeup

    static TickType_t delay, wakeUp;
    while ( task->_running ){;
        wakeUp = xTaskGetTickCount();
        /// Call inner loop of sons
        task->_loop();

        /// Try ensure constant rate
        taskYIELD();
        delay = ( (xTaskGetTickCount() - wakeUp) < task->_period )? task->_period - (xTaskGetTickCount() - wakeUp) : task->_period;
        vTaskDelay(delay);
    }
}