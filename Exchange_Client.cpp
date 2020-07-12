#include "REST_Client.cpp"
#include "auth_utils.cpp"

class Client
{
private:
	static std::string _generate_query(std::vector<std::pair<std::string, std::string>> params);

	std::string _api_key;
	std::string _api_secret;
	bool const _public_client;

protected:
	Client();
	Client(std::string key, std::string secret);
	virtual ~Client();

public:
	static const std::string _BASE_REST_FUTURES;
	static const std::string _BASE_REST_GEN;

	virtual unsigned long long exchange_time() = 0;
	virtual bool ping_client() = 0;

	RestSession* _rest_client = nullptr;

	void renew_session();

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

Client::Client() : _public_client{ 1 } 
{
	renew_session();
};

Client::Client(std::string key, std::string secret) : _public_client{ 0 }, _api_key { key }, _api_secret{ secret }
{
	renew_session();
};

std::string Client::_generate_query(std::vector<std::pair<std::string, std::string>> params)
{
	std::string query = "?";
	for (std::pair<std::string, std::string> param : params)
	{
		query += (param.first + '=' + param.second);
		if (!(param == params.back())) query += '?';
	}
}


void Client::renew_session()
{
	if (this->_rest_client) delete this->_rest_client;

	this->_rest_client = new RestSession{};
	if (!this->_public_client)
	{
		std::string key_header = "X-MBX-APIKEY:" + this->_api_key; // header for api key
		struct curl_slist* auth_headers;
		auth_headers = curl_slist_append(NULL, key_header.c_str());

		curl_easy_setopt((this->_rest_client)->_get_handle, CURLOPT_HTTPHEADER, auth_headers);
		curl_easy_setopt((this->_rest_client)->_post_handle, CURLOPT_HTTPHEADER, auth_headers);
	}
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
