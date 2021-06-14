#include "streamingProvider.hpp"

StreamingProvider::~StreamingProvider(){
    /// Desallocate memory
    for(auto buffer : _buffers)
        delete buffer;
}

void StreamingProvider::begin(uint8_t fps){
    SPIFFS.begin();
    
    /// initilize unlocked mutex
    _mutex = portMUX_INITIALIZER_UNLOCKED;
    
    /// Repetition period
    _period = pdMS_TO_TICKS(1000/fps);
    
    /// Create task to Provider
    run("provider");

}

void StreamingProvider::waitAvailable(){
    xSemaphoreTake(_semaphore, portMAX_DELAY);
}

void StreamingProvider::release(){
    xSemaphoreGive(_semaphore);
}

uint8_t *StreamingProvider::buffer(){
    return _buffers[_current];
}

size_t StreamingProvider::size(){
    return _sizes[_current];
}

void StreamingProvider::_loop(){
    /// Write in different buffer
    uint8_t writeIndex = _current^1;
    
    /// Check available space
    uint8_t * buf = _getPtr();
    if(_sizes[writeIndex] <= _size){
        _sizes[writeIndex] = _size;
    }else{
        _sizes[writeIndex] = _size;
        /// if cannot reallocate, do nothing
        if( !reallocate(writeIndex) )
            return;
    }
    /// Copy content
    memcpy(_buffers[writeIndex], buf, _sizes[writeIndex]);
    
    /// Wait if someone is reading readbuffer
    waitAvailable();
    portENTER_CRITICAL(&_mutex);
    /// New data is ready
    _current = writeIndex;
    portEXIT_CRITICAL(&_mutex);
    /// allow read
    release();
}


uint8_t *StreamingProvider::_getPtr(){
    static uint8_t idx, *buf;
    String filename = "/" + String(idx) + ".jpeg";
    idx = (idx + 1) % 10; 

    File file = SPIFFS.open(filename);
    size_t size = file.size();
    
    if (size > _size){
        delete buf;
        buf = new uint8_t[size];
    }

    _size = file.read(buf, size);
    return buf;
}

bool StreamingProvider::reallocate(uint8_t idx){
    size_t availableHeap;
    /// delete previus allocated buffer
    delete _buffers[idx];
    availableHeap = ESP.getFreeHeap();

    /// Check if there is available space in heap
    if ( _sizes[idx] < availableHeap ){
        _buffers[idx] = new uint8_t [_sizes[idx]];
    }
    /// Check if there is psram with suficient space then allocate
    else if ( psramFound() && _sizes[idx] < ESP.getFreePsram() ){
        _buffers[idx] = (uint8_t *) ps_malloc(_sizes[idx]);
    }
    return _buffers[idx] != nullptr;
}