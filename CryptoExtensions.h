

// todo: construct client with json file!

#ifndef CRYPTO_EXTENSIONS_H
#define CRYPTO_EXTENSIONS_H

#include <iostream>
#include <json/json.h>
#include <curl/curl.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <chrono>
#include <string>
#include <map>

unsigned long long local_timestamp();
inline auto binary_to_hex_digit(unsigned a) -> char;
auto binary_to_hex(unsigned char const* binary, unsigned binary_len)->std::string;
std::string HMACsha256(std::string const& message, std::string const& key);




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

	void clear_params(); // define this
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
	const std::string _BASE_REST_GEN{ "https://api.binance.com" };

	bool flush_params; // if true, param objects passed to functions will be flushed

	virtual unsigned long long exchange_time() = 0;
	virtual bool ping_client() = 0;

	RestSession* _rest_client = nullptr;

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

	Json::Value send_order(Params& parameter_vec);
	Json::Value fetch_balances(Params& param_obj);

	~FuturesClient() // move to external
	{
		delete _rest_client;
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

	Json::Value send_order(Params& parameter_vec);

	~SpotClient() // move to external
	{
		delete _rest_client;
	};
};

#endif