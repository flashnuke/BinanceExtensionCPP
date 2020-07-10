#include "REST_Client.cpp"

#ifndef std::string
#include <string>
#endif

class Client
{
private:
	std::string api_key;
	std::string api_secret;
public:
	Client();
	Client(std::string key, std::string secret);

	const std::string _BASE_REST_FUTURES = "https://fapi.binance.com";
	const std::string _BASE_REST = "https://api.binance.com";
	bool return_json{ 0 }; // make this more clean

	std::string exchange_time();


	RestSession _rest_client{};

	~Client() {};
};

Client::Client() {};
Client::Client(std::string key, std::string secret) : api_key{ key }, api_secret{ secret }{};

std::string Client::exchange_time()
{
	std::string endpoint = "/api/v3/time";
	return this->_rest_client._getreq(this->_BASE_REST + endpoint);
}

class FuturesClient: public Client
{
private:

public:
	FuturesClient();
	FuturesClient(std::string key, std::string secret);


	~FuturesClient() {};
};

FuturesClient::FuturesClient() : Client() {};

FuturesClient::FuturesClient(std::string key, std::string secret)
	: Client(key, secret){}


class SpotClient: public Client
{
private:
public:
	SpotClient();
	SpotClient(std::string key, std::string secret);

	~SpotClient() {};
};

SpotClient::SpotClient() : Client() {};

SpotClient::SpotClient(std::string key, std::string secret)
	: Client(key, secret) {}

