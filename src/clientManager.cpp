#include "clientManager.hpp"

const char *ClientManager_::ADDR = "/stream";
const char *ClientManager_::HEADER = "HTTP/1.1 200 OK\r\n" \
                                     "Access-Control-Allow-Origin: *\r\n" \
                                     "Content-Type: multipart/x-mixed-replace;"\
                                     "boundary = 99999999999999999999999\r\n";
const char *ClientManager_::BOUNDARY = "\r\n--99999999999999999999999\r\n";
const char *ClientManager_::CONTENT_TYPE = "Content-Type: image/jpeg\r\nContent-Length: ";

WebServer ClientManager_::_server;
QueueHandle_t ClientManager_::_clients;

void ClientManager_::begin(uint32_t period, uint16_t port){
    _server.begin(port);
    _period = pdMS_TO_TICKS(period);
    _clients = xQueueCreate(CLIENT_NUMBER, sizeof(WiFiClient*));

    _server.on(ADDR, HTTP_GET, _registerUser);
}

bool ClientManager_::next(){
    /// recive next client
    xQueueReceive(_clients, (void *) &_curretClient, 0);
    if ( ! _curretClient->connected() ){
        delete _curretClient;
        return false;
    }
    return true;
}

UBaseType_t ClientManager_::available(){
    return uxQueueMessagesWaiting(_clients);
}

void ClientManager_::send(uint8_t * buffer, size_t size){
    char sizeText[16];
    /// transform size to string
    sprintf(sizeText, "%d\r\n\r\n", size);
    
    /// send buffer
    _curretClient->write(CONTENT_TYPE, strlen(CONTENT_TYPE));
    _curretClient->write(sizeText, strlen(sizeText));
    _curretClient->write((uint8_t*) buffer, size);
    _curretClient->write(BOUNDARY, strlen(BOUNDARY));

    /// Put again client in the queue
    xQueueSend(_clients, (void *) &_curretClient, 0);
}

void ClientManager_::_loop(){
    /// Answer new clients
    _server.handleClient();
}

void ClientManager_::_registerUser(){
    /// Check if There is still space available
    if ( uxQueueSpacesAvailable(_clients) ){
        WiFiClient* client = new WiFiClient();
        *client = _server.client();

        /// Send streaming header
        client->write(HEADER, strlen(HEADER));
        client->write(BOUNDARY, strlen(BOUNDARY));

        /// Add in client in queue
        xQueueSend(_clients, (void *) &client, 0);
    }
}
