
// todo: in 'connect' method, check firstly if we are connected alrdy. if so, disconnect
// todo: add_stream (to vector) vs start_stream (generate query and start)
// todo: keep thread without joining?
// todo: if streamname in map, abort? in order to avoid dups, simply send the entire path or with symbol
// todo: make ws local and no need for 'new' 'delete'

#include "CryptoExtensions.h"


WebsocketClient::WebsocketClient(std::string host, std::string port)
    : _host{ host }, _port{ port }
{}





WebsocketClient::~WebsocketClient()
{
    delete this->_ws;
}