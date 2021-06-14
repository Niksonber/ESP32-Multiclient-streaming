#include "streamer.hpp"

void Streamer::begin(uint8_t fps){
    _totalPeriod = pdMS_TO_TICKS(1000/fps);
    
    _clientManager.begin(2000/fps);
    _provider.begin(fps);
    run("streamer");
}

void Streamer::_loop(){
    UBaseType_t available = _clientManager.available();
    if ( available ){
        /// adjust the waiting time to serve all customers with the desired latency
        _period = _totalPeriod/available;
        if( _clientManager.next() ){
            _provider.waitAvailable();
            _clientManager.send(_provider.buffer(), _provider.size());
            _provider.release();
        }
    }
}