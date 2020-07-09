#include "REST_Client.cpp"

#ifndef std::string
#include <string>
#endif

class Client
{
private:
	std::string api_key;
	std::string api_secret;
	std::string _BASE_REST;
	RestSession rest_client{};
public:
	Client();
	Client(std::string key, std::string secret);
};

Client::Client() {};
Client::Client(std::string key, std::string secret) : api_key{ key }, api_secret{ secret }{};


class FuturesClient: private Client
{
private:
	const std::string _BASE_REST = "https://fapi.binance.com";
public:
	FuturesClient();
	FuturesClient(std::string key, std::string secret);
};

FuturesClient::FuturesClient() : Client() {};

FuturesClient::FuturesClient(std::string key, std::string secret)
	: Client(key, secret){}

class SpotClient: private Client
{
private:
	const std::string _BASE_REST = "https://api.binance.com";
public:
	SpotClient();
	SpotClient(std::string key, std::string secret);
};

SpotClient::SpotClient() : Client() {};

SpotClient::SpotClient(std::string key, std::string secret)
	: Client(key, secret) {}

