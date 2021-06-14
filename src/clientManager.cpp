#include "clientManager.hpp"

const char *ClientManager::ADDR = "/stream";
const char *ClientManager::HEADER = "HTTP/1.1 200 OK\r\n" \
                                     "Content-Type: multipart/x-mixed-replace;"\
                                     "boundary = 99999999999999999999999\r\n";
const char *ClientManager::BOUNDARY = "\r\n--99999999999999999999999\r\n";
const char *ClientManager::CONTENT_TYPE = "Content-Type: image/jpeg\r\nContent-Length: ";

WebServer ClientManager::_server;
QueueHandle_t ClientManager::_clients;

void ClientManager::begin(uint32_t period, uint16_t port){
    _server.begin(port);
    _period = pdMS_TO_TICKS(period);
    _clients = xQueueCreate(CLIENT_NUMBER, sizeof(WiFiClient*));

    _server.on(ADDR, HTTP_GET, _registerUser);

    run("client");
}

bool ClientManager::next(){
    /// recive next client
    xQueueReceive(_clients, (void *) &_curretClient, 0);
    if ( ! _curretClient->connected() ){
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
    _curretClient->printf("%s %u", CONTENT_TYPE, size);
    _curretClient->write((uint8_t*) buffer, size);
    _curretClient->print(BOUNDARY);

    /// Put again client in the queue
    xQueueSend(_clients, (void *) &_curretClient, 0);
}

void ClientManager::_loop(){
    /// Answer new clients
    _server.handleClient();
}

void ClientManager::_registerUser(){
    /// Check if There is still space available
    if ( uxQueueSpacesAvailable(_clients) ){
        WiFiClient* client = new WiFiClient();
        *client = _server.client();

        /// Send streaming header
        client->printf("%s%s", HEADER, BOUNDARY);

        /// Add in client in queue
        xQueueSend(_clients, (void *) &client, 0);
    }
}
