#pragma once

#include <SPIFFS.h>
#include <freertos/semphr.h> 

#include "tasks.hpp"

/// Provides data in parallel (ensure it's integrity) to Streamer, i.e, contain 2 diferent buffers (while reading in one writes in other)
class StreamingProvider: public Task{
public:
    /// Initialy alloc buffers  
    StreamingProvider();
    
    /// Delete all alocated buffers
    ~StreamingProvider();

    /// Begin SPIFFS, set desired fps and create a task to get stream buffer
    /// @param fps Desired transmission rate
    void begin(uint8_t fps);

    /// Wait until read loop is not over. This avoid partial read/corrupted image
    void waitAvailable();

    /// Release semaphore
    void release();

    /// Getter for ready buffer while is writting in other
    uint8_t *buffer();

    /// Getter for size buffer
    size_t size();

    /// Number of buffer, this allows that simultaneous read and write (in deferent buffers)
    static const uint8_t PIPELINE_SIZE = 2;
protected:
    /// Index of actual reading buffer
    uint8_t _current = 0;
    
    /// Buffers sizes
    size_t _sizes[PIPELINE_SIZE] = {0};
    
    /// Buffers
    uint8_t * _buffers[PIPELINE_SIZE] = {nullptr};
    
    /// size of new content
    size_t _size = 0;
    
    /// Maximum size
    size_t _maxSize = 50000;

    /// Avoid interruptons in buffer cange
    portMUX_TYPE _mutex;

    /// Avoid currupteed data
    SemaphoreHandle_t _semaphore;

    /// Loop. Get new data, if necessary reallocate buffer
    void _loop();
    
    /// Get ptr to new data. @return pointer to data
    uint8_t *_getPtr();

    /// Reallocate buffer, if necessáry use psram
    /// @param index of buffer (in _buffers) @return true if sucessfully reallocate
    bool reallocate(uint8_t idx);  
};

