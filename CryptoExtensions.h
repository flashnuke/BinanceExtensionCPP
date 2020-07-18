

// todo: construct client with json file!
// todo: instantiate only if ws is called!
// todo: websocket handshakes vector. 

#ifndef CRYPTO_EXTENSIONS_H
#define CRYPTO_EXTENSIONS_H

#define _WIN32_WINNT 0x0601 // for boost


#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <iostream>
#include <json/json.h>
#include <curl/curl.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <chrono>
#include <string>
#include <map>


namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

unsigned long long local_timestamp();
inline auto binary_to_hex_digit(unsigned a) -> char;
auto binary_to_hex(unsigned char const* binary, unsigned binary_len)->std::string;
std::string HMACsha256(std::string const& message, std::string const& key);


class WebsocketClient
{
private:
	std::string _host;
	std::string _port;
	websocket::stream<beast::ssl_stream<tcp::socket>>* _ws{ nullptr };
	void _connect_to_endpoint(std::string endpoint);


public:
	WebsocketClient(std::string host, std::string port);

	void start_stream(std::string endpoint); // todo: insert pointer as param, change void

	~WebsocketClient();

};

class RestSession
{
private:

	std::string _req_raw_get; // todo -> make this get_response and flush everytime	
	Json::Value _req_json_get;
	CURLcode _get_status;

	std::string _req_raw_post; // todo -> make this get_response and flush everytime	
	Json::Value _req_json_post;
	CURLcode _post_status;

public:
	RestSession();

	bool status; // bool for whether session is active or not
	CURL* _get_handle{};
	CURL* _post_handle{};

	Json::Value _getreq(std::string path);
	inline void get_timeout(unsigned long interval);

	Json::Value _postreq(std::string path);
	inline void post_timeout(unsigned long interval);


	void close();

	friend unsigned int _GET_CALLBACK(void* contents, unsigned int size, unsigned int nmemb, RestSession* self); // different because of members
	friend unsigned int _POST_CALLBACK(void* contents, unsigned int size, unsigned int nmemb, RestSession* self);

	~RestSession();
};


struct Params
	// Params will be stored in a map of <str, str> and parsed by the query generator.
{	// todo: in documentation, state to pass empty obj for all

	Params();
	Params(Params& param_obj);
	Params(const Params& param_obj);

	Params& operator=(Params& params_obj);
	Params& operator=(const Params& params_obj);

	std::map<std::string, std::string> param_map;

	template <typename PT>
	void set_param(std::string key, PT value);

	void clear_params(); 
	void empty(); // define this
};


class Client
{
private:
	bool const _public_client;


protected:
	std::string _api_key;
	std::string _api_secret;

	Client();
	Client(std::string key, std::string secret);
	virtual ~Client();

public:
	std::string _generate_query(Params& params_obj);

	const std::string _BASE_REST_FUTURES{ "https://fapi.binance.com" };
	const std::string _BASE_REST{ "https://api.binance.com" };
	const std::string _WS_BASE_FUTURES{"fstream.binance.com"};
	const std::string _WS_BASE{ "stream.binance.com" };
	const std::string _WS_PORT{ "9443" };

	bool flush_params; // if true, param objects passed to functions will be flushed

	virtual unsigned long long exchange_time() = 0;
	virtual bool ping_client() = 0;
	virtual void init_ws() = 0;

	RestSession* _rest_client = nullptr; // move init
	WebsocketClient* _ws_client = nullptr; // move init, leave decl

	void renew_session();

};



class FuturesClient : public Client
{
private:

public:
	FuturesClient();
	FuturesClient(std::string key, std::string secret);

	unsigned long long exchange_time();
	bool ping_client();
	void init_ws();

	Json::Value send_order(Params& parameter_vec);
	Json::Value fetch_balances(Params& param_obj);
	void aggTrade(std::string symbol); // todo: change from void

	~FuturesClient() // move to external
	{
		delete _rest_client;
		delete _ws_client;
	};
};



class SpotClient : public Client
{
private:

public:
	SpotClient();
	SpotClient(std::string key, std::string secret);

	unsigned long long exchange_time();
	bool ping_client();
	void init_ws();

	Json::Value send_order(Params& parameter_vec);
	void aggTrade(std::string symbol); // todo: change from void

	~SpotClient() // move to external
	{
		delete _rest_client;
		delete _ws_client;
	};
};

#endif