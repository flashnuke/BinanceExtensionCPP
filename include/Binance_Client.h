// todo: better handle error codes api
// todo: v_close_stream from symbol+stream to just stream_name


// DOCs todos:
// 1. order book fetch from scratch example
// 2. ws symbols must be lower case
// 3. v_ is for crtp
// 4. custom requests, pass params into query
// 5. I let passing empty or none params so the user can receive the error and see whats missing! better than runtime error
// 6. all structs require auth (even margin requires header)
// 7. no default arguments for ws streams when using threads. Must specify...
// 8. I initialize up to Client() constructor with a reference of 'this' in order to gain access to Renew listen key
// 9. ping listen key spot: if ping is empty, post req is sent
// 10. explain how exceptions work
// 11. example of handling 'BadRequest' where you retry sending the request
// 12. include headers: none. you should only include binance_client.h
// 13. include src: auth utils, client exceptions, rest client, ws client, binance_client
// 

// First make everything for spot and then for futures

#ifndef BINANCE_CLIENT_H
#define BINANCE_CLIENT_H

#define _WIN32_WINNT 0x0601 // for boost

// external libraries
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>

#include <json/json.h>
#include <curl/curl.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>

// STL
#include <iostream>
#include <chrono>
#include <string>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <vector>


namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

// this library
#include "Client_Exceptions.h"
#include "REST_Client.h"
#include "WS_Client.h"
#include "Exchange_Client.h"
#include "Websocket_methods.inl"



#endif