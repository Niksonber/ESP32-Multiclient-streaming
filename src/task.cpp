#include "tasks.hpp"

void Task::run(void* args){
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