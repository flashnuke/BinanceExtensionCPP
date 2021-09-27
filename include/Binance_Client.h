
#ifndef BINANCE_CLIENT_H
#define BINANCE_CLIENT_H

#define _WIN32_WINNT 0x0601 // for boost

// external libraries
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>

#include <jsoncpp/json/json.h>
#include <curl/curl.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>

// STL
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

// inl files
#include "inl_files/Websocket_methods.inl"
#include "inl_files/Client_methods.inl"
#include "inl_files/REST_methods.inl"



#endif