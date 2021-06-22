#include "streamer.hpp"

void Streamer::begin(uint8_t fps, StreamingProvider *provider){
    _totalPeriod = pdMS_TO_TICKS(1000/fps);
    
    if ( provider != nullptr ){
        log_i("Getting provider");
        _provider = provider;
    } 
    else{
        log_i("Getting defaut provider");
        _provider = new StreamingProvider;
        return;
    }

    _provider->begin(fps);
    _clientManager.begin(333/fps);
    run("streamer");
}

void Streamer::on(const Uri &uri, WebServer::THandlerFunction fn, HTTPMethod method){
    _clientManager._server.on(uri, method, fn);
}

void Streamer::_loop(){
    UBaseType_t available = _clientManager.available();
    _period = _totalPeriod;
    if ( available ){
        /// adjust the waiting time to serve all customers with the desired latency
        _period /= available;
        if( _clientManager.next() ){
            _provider->waitAvailable();
            _clientManager.send(_provider->buffer(), _provider->size());
            _provider->release();
        }
    }
}