#include "clientManager.hpp"

const char *ClientManager::ADDR_STREAM = "/stream";
const char *ClientManager::ADDR_SAMPLE = "/sample";
const char *ClientManager::HEADER_OK = "HTTP/1.1 200 OK\r\n";
const char *ClientManager::HEADER = "Content-Type: multipart/x-mixed-replace; " \
                                    "boundary=9999999999999999999999";
const char *ClientManager::BOUNDARY = "\r\n\r\n--9999999999999999999999\r\n";
const char *ClientManager::CONTENT_TYPE = "Content-Type: image/jpeg\r\nContent-Length:";

WebServer ClientManager::_server;
portMUX_TYPE ClientManager::_mutex;
QueueHandle_t ClientManager::_clients;

void ClientManager::begin(uint32_t period, uint16_t port){
    _server.begin(port);
    _period = pdMS_TO_TICKS(period);
    _mutex = portMUX_INITIALIZER_UNLOCKED;
    _clients = xQueueCreate(CLIENT_NUMBER, sizeof(WiFiClient*));

    _curretClient = nullptr;

    _server.on(ADDR_STREAM, HTTP_GET, _registerStreamUser);
    _server.on(ADDR_SAMPLE, HTTP_GET, _registerSampleUser);

    run("client");
}

bool ClientManager::next(){
    /// recive next client
    portENTER_CRITICAL(&_mutex);
    xQueueReceive(_clients, (void *) &_curretClient, 0);
    portEXIT_CRITICAL(&_mutex);

    if ( _curretClient != nullptr && ! _curretClient->client.connected() ){
        log_i("Deleting client");
        delete _curretClient;
        return false;
    }
    return true;
}

UBaseType_t ClientManager::available(){
    return uxQueueMessagesWaiting(_clients);
}

void ClientManager::send(uint8_t * buffer, size_t size){
    /// send buffer
    if ( _curretClient != nullptr && _curretClient->client.connected()){
        _curretClient->client.printf("%s %u\r\n\r\n", CONTENT_TYPE, static_cast<uint32_t>(size));
        _curretClient->client.write((uint8_t*) buffer, size);
        if(_curretClient->stream){
            _curretClient->client.print(BOUNDARY);

            /// Put again client in the queue
            portENTER_CRITICAL(&_mutex);
            xQueueSend(_clients, (void *) &_curretClient, 0);
            portEXIT_CRITICAL(&_mutex);
            return;
        }
    }
    delete _curretClient;

    _curretClient = nullptr;
    log_i("Client will not return to queue");
}

void ClientManager::_loop(){
    /// Answer new clients
    _server.handleClient();
}

void ClientManager::_registerStreamUser(){
    _registerUser();
}

void ClientManager::_registerSampleUser(){
    _registerUser(false);
}

void ClientManager::_registerUser(bool stream){
    /// Check if There is still space available
    if ( uxQueueSpacesAvailable(_clients) ){
        try{
            portENTER_CRITICAL(&_mutex);
            client_t * handler = new client_t;
            portEXIT_CRITICAL(&_mutex);
        
            handler->stream = stream;
            handler->client = _server.client();

            /// Send streaming header
            handler->client.printf("%s", HEADER_OK);
            if(stream) handler->client.printf("%s%s", HEADER, BOUNDARY);

            /// Add in client in queue
            portENTER_CRITICAL(&_mutex);
            xQueueSend(_clients, (void *) &handler, 0);
            portEXIT_CRITICAL(&_mutex);

            log_i("Clients in queue: %d", uxQueueMessagesWaiting(_clients));
        }catch(std::bad_alloc){
            log_i("Fail allocaation of Client");
        }
    }
}
