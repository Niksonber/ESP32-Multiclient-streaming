#pragma once
#include "tasks.hpp"
#include "streamingProvider.hpp"
#include "clientManager.hpp"

/// Streaming responsable 
class Streamer: public Task{
public:
    /// Begin client manager and provider. That is inst internal variables and
    /// start 3 tasks: client manger (handler with clients queue), provider (get data) and streamer (send to all clients)
    /// @param fps Desired transmission rate 
    void begin(uint8_t fps, StreamingProvider * provider = nullptr);

    /// Interface to client maneger server, adding routes in server
    /// @param uri addres to route @param fn routine to router @param HTTP_METHOD (optional) Especify method to run this routine, defaut HTTP_ANY
    void on(const Uri &uri, WebServer::THandlerFunction fn, HTTPMethod method = HTTP_ANY);

protected:
    /// Stream to all available clients. To do this adequade sleep time to number of clients
    void _loop();

    /// Desired transmission period
    TickType_t _totalPeriod;

    /// Provide content to stream
    StreamingProvider *_provider;

    /// Manager Clients queue
    ClientManager _clientManager;
};