#include "streamingProvider.hpp"

StreamingProvider::StreamingProvider(){
    for(uint8_t i = 0; i < PIPELINE_SIZE; i++){
        _buffers[i] = nullptr;
        _sizes[i] = 0;
    }
}

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

    /// Init semaphore
    _semaphore = xSemaphoreCreateBinary();
    release();
    
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
    size_t size;
    uint8_t *buf;
    /// Write in different buffer
    uint8_t writeIndex = _current^1;
    
    if ( _getData(&buf, &size) ){
        /// Check available space
        if ( size <= _maxSize[writeIndex] ){
            _sizes[writeIndex] = size;
        }else{
            _sizes[writeIndex] = size;
            _maxSize[writeIndex] = 1.3*size;
            /// if cannot reallocate, do nothing
            if( !reallocate(writeIndex) )
                return;
        }
        /// Copy content
        // log_i("Allocated: %d, cpy %d", _maxSize,_sizes[writeIndex]);
        memcpy(_buffers[writeIndex], buf, _sizes[writeIndex]);
        
        /// Allows changes in data buffer
        _releaseData();
        
        /// Wait if someone is reading readbuffer
        waitAvailable();
        portENTER_CRITICAL(&_mutex);
        /// New data is ready
        _current = writeIndex;
        portEXIT_CRITICAL(&_mutex);
        /// allow read
        release();
    }else{
        log_e("Error get data buffer");
    }

}


bool StreamingProvider::_getData(uint8_t ** ptr, size_t * size){
    static uint8_t idx;
    static size_t maxSize;
    String filename = "/" + String(idx) + ".jpg";
    idx = (idx + 1) % 10; 

    File file = SPIFFS.open(filename);
    *size = file.size();
    
    if (*size > maxSize && *size < ESP.getFreeHeap()){
        try{
            delete[] (*ptr);
            *ptr = new uint8_t[*size];
            maxSize = *size;
        }catch( std::bad_alloc ){
            *size = maxSize;
            return false;
        }
    }

    *size = file.read(*ptr, *size);
    return *size != 0;
}

bool StreamingProvider::reallocate(uint8_t idx){
    log_i("Realocating to %d", _maxSize[idx]);
    portENTER_CRITICAL(&_mutex);
    if ( psramFound() ){
        _buffers[idx] = (uint8_t *) ps_realloc(_buffers[idx], _maxSize[idx]);
    }else{
        _buffers[idx] = (uint8_t *) realloc(_buffers[idx], _maxSize[idx]);
    }
    portEXIT_CRITICAL(&_mutex);
    return _buffers[idx] != nullptr;
}