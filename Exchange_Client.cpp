#include "REST_Client.cpp"
#include "auth_utils.cpp"

struct Params
	// Params will be stored in a vector of pairs<str, str> and parsed by the query generator.
	// Note that when passing params to a method, they will be cleared!
	// todo: copy assignment + copy constructor + move
	// todo: 
{
	std::vector<std::pair<std::string, std::string>> param_vector;

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
	static std::string _generate_query(Params& params_obj);

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

	Json::Value send_order(Params &parameter_vec);

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

std::string Client::_generate_query(Params& params_obj)

{
	std::vector<std::pair<std::string, std::string>> params = params_obj.param_vector;
	std::string query = "?";
	for (std::pair<std::string, std::string> param : params)
	{
		query += (param.first + '=' + param.second);
		if (!(param == params.back())) query += '&';
	}

	return query;
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

Json::Value SpotClient::send_order(Params& parameter_vec)
{
	std::string endpoint = "/fapi/v1/order";
	std::string test_endp = "/api/v3/order/test";
	std::string query = Client::_generate_query(parameter_vec);

	std::string signature = HMACsha256(query, this->_api_secret);
	//parameter_vec.set_param() delete?
	//send order...
	query += ("&signature=" + signature);
	std::cout << query;

	parameter_vec.clear_params();

	Json::Value returnme;
	return returnme;

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

Json::Value FuturesClient::send_order(Params& parameter_vec)
{
	std::string endpoint = "/fapi/v1/order";
	std::string query = Client::_generate_query(parameter_vec);

	std::string signature = HMACsha256(query, this->_api_secret);
	//parameter_vec.set_param() delete?
	//send order...
	query += ("&signature=" + signature);
	std::cout << query;

	parameter_vec.clear_params();

	Json::Value returnme;
	return returnme;

}

// Params definitions

template <typename PT>
void Params::set_param(std::string key, PT value)
{
	param_vector.push_back(std::make_pair(key, std::to_string(value)));
}
template <>
void Params::set_param<std::string>(std::string key, std::string value)
{
	param_vector.push_back(std::make_pair(key, value)); // use std::makepair()?
}

void Params::clear_params()
{
	this->param_vector.clear();
}