
// todo: in 'connect' method, check firstly if we are connected alrdy. if so, disconnect
// todo: keep thread without joining?
// todo: if streamname in map, abort? in order to avoid dups, simply send the entire path or with symbol
// todo: form a 'stream' object that would contain: thread addr, functor, pointer to stream string, status, error_code
// todo: stream object contains: disconnect(), reconnect() - delete current thread and call _connect_endpoint() method
// todod dest: while status != 0: set 0 and delete thread pointer!

#include "CryptoExtensions.h"


WebsocketClient::WebsocketClient(std::string host, std::string port)
    : _host{ host }, _port{ port }
{}


void WebsocketClient::close_stream(const std::string full_stream_name)
{
    this->running_streams[full_stream_name] = 0;
}


WebsocketClient::~WebsocketClient()
{
    std::map<std::string, bool>::iterator stream_itr; // while status != 0: set 0 and delete thread pointer!

    for (stream_itr = this->running_streams.begin(); stream_itr != this->running_streams.end(); stream_itr++)
    {
        (stream_itr->second) = 0; // set status to false to ensure closing streams

    }
}