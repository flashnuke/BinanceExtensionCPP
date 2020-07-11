#include "REST_Client.cpp"
#include "auth_utils.cpp"

class Client
{
private:
	bool public_client;
	std::string api_key;
	std::string api_secret;
public:
	Client();
	Client(std::string key, std::string secret);

	const std::string _BASE_REST_FUTURES = "https://fapi.binance.com";
	const std::string _BASE_REST = "https://api.binance.com";

	unsigned long long exchange_time(); //


	RestSession _rest_client{};

	~Client() {};
};

Client::Client() : public_client{ 1 } {};
Client::Client(std::string key, std::string secret) : api_key{ key }, api_secret{ secret }, public_client{ 0 }{};

unsigned long long Client::exchange_time()
{
	std::string endpoint = "/api/v3/time";
	std::string ex_time = this->_rest_client._getreq(this->_BASE_REST + endpoint)["serverTime"].asString();
	std::cout << std::endl << "test local time " << local_timestamp();

	return std::atoll(ex_time.c_str());
}
