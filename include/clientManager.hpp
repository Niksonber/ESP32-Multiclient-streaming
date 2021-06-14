#pragma once
#include <WebServer.h>
#include <WiFiClient.h>

#include "tasks.hpp"

/// Manage a clients queue conections
class ClientManager: public Task{
public:
    /// Initialize server and routs, set internal as period of execution
    /// @param period repetition period in ms @param port (optional) server port - defaut 80
    void begin(uint32_t period, uint16_t port = 80);

    /// Get next client in the queue. If it doesn't stay connected delete it.
    /// @return true if client still conected, else othewise
    bool next();

    /// @return Number of clients in the queue
    UBaseType_t available();

    /// Send buffer to client and put in the queue again
    /// @param buffer buffer to client @param size buffer size
    void send(uint8_t * buffer, size_t size);

protected:

    /// Check new clients
    void _loop();

    /// Add new users in the queue
    static void _registerUser();
    
    /// current client
    WiFiClient * _curretClient;

    /// Server
    static WebServer _server;

    /// Queue of clients
    static QueueHandle_t _clients;

    /// constants
    static const char *ADDR;
    static const char *HEADER;
    static const char *BOUNDARY;
    static const char *CONTENT_TYPE;
    static const uint8_t CLIENT_NUMBER = 10;
};
