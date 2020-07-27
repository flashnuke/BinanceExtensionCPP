// todo: futures and client source files in different files?
// todo: return empty json with "status = 1" if no cb passed.
// todo: custom stream. this way you can connect to several... pass bool for 'renew_key'


// DOCs todos:
// 1. order book fetch from scratch example
// 2. ws symbols must be lower case
// 3. v_ is for crtp

// First make everything for spot and then for futures

#ifndef CRYPTO_EXTENSIONS_H
#define CRYPTO_EXTENSIONS_H

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

unsigned long long local_timestamp();
inline auto binary_to_hex_digit(unsigned a) -> char;
auto binary_to_hex(unsigned char const* binary, unsigned binary_len)->std::string;
std::string HMACsha256(std::string const& message, std::string const& key);


class RestSession
{
private:

	struct RequestHandler // handles response
	{
		RequestHandler();

		std::string req_raw;
		Json::Value req_json;
		CURLcode req_status;
		std::unique_lock<std::mutex>* locker;
	};


public:
	RestSession();

	bool status; // bool for whether session is active or not
	CURL* _get_handle{};
	CURL* _post_handle{};
	CURL* _put_handle{};

	Json::Value _getreq(std::string full_path);
	inline void get_timeout(unsigned long interval);
	std::mutex _get_lock;

	Json::Value _postreq(std::string full_path);
	inline void post_timeout(unsigned long interval);
	std::mutex _post_lock;

	Json::Value _putreq(std::string full_path);
	inline void put_timeout(unsigned long interval);
	std::mutex _put_lock;

	bool close();

	friend unsigned int _REQ_CALLBACK(void* contents, unsigned int size, unsigned int nmemb, RestSession::RequestHandler* req);

	~RestSession();
};

class WebsocketClient
{
private:
	const std::string _host;
	const std::string _port;
	bool _reconnect_on_error;

	template <class FT>
	void _connect_to_endpoint(std::string stream_map_name, std::string& buf, FT& functor, std::pair<RestSession*,
		std::string> user_stream_pair = std::make_pair<RestSession*, std::string>(nullptr, ""));

public:
	WebsocketClient(std::string host, std::string port);

	std::unordered_map<std::string, bool> running_streams; // will be a map, containing pairs of: <bool(status), ws_stream> 

	void close_stream(const std::string stream_name);
	std::vector<std::string> open_streams();
	bool is_open(const std::string& stream_name);
	unsigned int refresh_listenkey_interval;

	template <class FT>
	void _stream_manager(std::string stream_map_name, std::string& buf, FT& functor, std::pair<RestSession*,
		std::string> user_stream_pair = std::make_pair<RestSession*, std::string>(nullptr, ""));
	
	void _set_reconnect(const bool& reconnect);


	~WebsocketClient();

};




struct Params
	// Params will be stored in a map of <str, str> and parsed by the query generator.
{

	Params();
	explicit Params(Params& param_obj);
	explicit Params(const Params& param_obj);

	Params& operator=(Params& params_obj);
	Params& operator=(const Params& params_obj);

	std::unordered_map<std::string, std::string> param_map;

	template <typename PT>
	void set_param(std::string key, PT value);

	bool clear_params();
	bool empty();
};


template<typename T>
class Client
{
private:


protected:
	std::string _api_key;
	std::string _api_secret;

	explicit Client();
	Client(std::string key, std::string secret);
	~Client();

public:
	bool const _public_client;

	std::string _generate_query(Params& params_obj);

	const std::string _BASE_REST_FUTURES{ "https://fapi.binance.com" };
	const std::string _BASE_REST_SPOT{ "https://api.binance.com" };
	const std::string _WS_BASE_FUTURES{"fstream.binance.com"};
	const std::string _WS_BASE_SPOT{ "stream.binance.com" };
	const std::string _WS_PORT{ "9443" };

	bool flush_params; // if true, param objects passed to functions will be flushed

	// CRTP methods
	unsigned long long exchange_time();
	bool ping_client();
	bool init_ws_session();
	std::string _get_listen_key();
	void close_stream(const std::string& symbol, const std::string& stream_name);
	bool is_stream_open(const std::string& symbol, const std::string& stream_name);
	std::vector<std::string> get_open_streams();
	void ws_auto_reconnect(const bool& reconnect);
	inline void set_refresh_key_interval(const bool val);

	// end CRTP methods

	bool init_rest_session();
	bool set_headers(RestSession* rest_client);
	RestSession* _rest_client = nullptr; // move init
	WebsocketClient* _ws_client = nullptr; // move init, leave decl

};



class FuturesClient : public Client<FuturesClient>
{
private:

public:
	FuturesClient();
	FuturesClient(std::string key, std::string secret);

	inline unsigned long long v_exchange_time();
	inline bool v_ping_client();
	inline bool v_init_ws_session();
	inline std::string v__get_listen_key();
	inline void v_close_stream(const std::string& symbol, const std::string& stream_name);
	inline bool v_is_stream_open(const std::string& symbol, const std::string& stream_name);
	inline std::vector<std::string> v_get_open_streams();
	inline void v_ws_auto_reconnect(const bool& reconnect);
	inline void v_set_refresh_key_interval(const bool val);
	

	Json::Value send_order(Params& parameter_vec);
	Json::Value fetch_balances(Params& param_obj);
	unsigned int aggTrade(std::string symbol);
	template <class FT>
	unsigned int userStream(std::string& buffer, FT& functor);

	~FuturesClient();
};


class SpotClient : public Client<SpotClient>
{
private:

public:
	SpotClient();
	SpotClient(std::string key, std::string secret);

	unsigned long long v_exchange_time();
	bool v_ping_client();
	bool v_init_ws_session();
	std::string v__get_listen_key();
	void v_close_stream(const std::string& symbol, const std::string& stream_name);
	bool v_is_stream_open(const std::string& symbol, const std::string& stream_name);
	std::vector<std::string> v_get_open_streams();
	void v_ws_auto_reconnect(const bool& reconnect);
	inline void v_set_refresh_key_interval(const bool val);


	Json::Value send_order(Params& parameter_vec);


	template <class FT>
	unsigned int aggTrade(std::string symbol, std::string& buffer, FT& functor);
	template <class FT>
	unsigned int userStream(std::string& buffer, FT& functor);

	~SpotClient();
};

#endif