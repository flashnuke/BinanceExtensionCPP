#include "REST_Client.cpp"
#include "auth_utils.cpp"

class Client
{
private:
	std::string api_key;
	std::string api_secret;

protected:
	Client();
	Client(std::string key, std::string secret);
	virtual ~Client();

public:
	static const std::string _BASE_REST_FUTURES;
	static const std::string _BASE_REST_GEN;

	virtual unsigned long long exchange_time() = 0;
	virtual bool ping_client() = 0;


	void renew_session();

	RestSession* _rest_client = new RestSession{};
	
};


class FuturesClient: public Client
{
private:

public:
	FuturesClient();
	FuturesClient(std::string key, std::string secret);

	unsigned long long exchange_time();
	bool ping_client();

	~FuturesClient()
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

	~SpotClient()
	{
		delete _rest_client;
	};
};



// Client definitions

const std::string Client::_BASE_REST_FUTURES = "https://fapi.binance.com"; // static
const std::string Client::_BASE_REST_GEN = "https://api.binance.com"; // static

Client::Client() {};

Client::Client(std::string key, std::string secret) : api_key{ key }, api_secret{ secret } {};

void Client::renew_session()
{
	delete this->_rest_client;
	_rest_client = new RestSession{};
}

Client::~Client()
{
	delete _rest_client;
};


// SpotClient definitions

SpotClient::SpotClient() : Client()
{
	if (!(this->ping_client())) throw("bad_ping"); // for exceptions class
};

SpotClient::SpotClient(std::string key, std::string secret)
	: Client(key, secret)
{
	if (!(this->ping_client())) throw("bad_ping"); // for exceptions class
}

unsigned long long SpotClient::exchange_time()
{
	std::string endpoint = "/api/v3/time";
	std::string ex_time = (this->_rest_client)->_getreq(this->_BASE_REST_GEN + endpoint)["serverTime"].asString();

	return std::atoll(ex_time.c_str());
}

bool SpotClient::ping_client()
{
	try
	{
		std::string endpoint = "/api/v3/ping";
		Json::Value ping_response = (this->_rest_client)->_getreq(this->_BASE_REST_GEN + endpoint);
		return (ping_response != Json::nullValue);
	}
	catch (...)
	{
		throw("bad_ping");
	}
}



// FuturesClient definitions

FuturesClient::FuturesClient() : Client()
{
	if (!(this->ping_client())) throw("bad_ping"); // for exceptions class
};

FuturesClient::FuturesClient(std::string key, std::string secret)
	: Client(key, secret)
{
	if (!(this->ping_client())) throw("bad_ping"); // for exceptions class
}

unsigned long long FuturesClient::exchange_time()
{
	std::string endpoint = "/fapi/v1/time";
	std::string ex_time = (this->_rest_client)->_getreq(this->_BASE_REST_FUTURES + endpoint)["serverTime"].asString();

	return std::atoll(ex_time.c_str());
}

bool FuturesClient::ping_client()
{
	try
	{
		std::string endpoint = "/fapi/v1/ping";
		Json::Value ping_response = (this->_rest_client)->_getreq(this->_BASE_REST_FUTURES + endpoint);
		return (ping_response != Json::nullValue);
	}
	catch (...)
	{ 
		throw("bad_ping");
	}
}
