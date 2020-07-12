#include "REST_Client.cpp"
#include "auth_utils.cpp"
#include <map>

struct Params
	// Params will be stored in a map of <str, str> and parsed by the query generator.
	// todo: copy assignment + copy constructor + move
{
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
	static std::string _generate_query(Params& params_obj);

	static const std::string _BASE_REST_FUTURES;
	static const std::string _BASE_REST_GEN;

	bool flush_params; // if true, param objects passed to functions will be flushed

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

Client::Client() : _public_client{ 1 }, flush_params{ 0 }
{
	renew_session();
};

Client::Client(std::string key, std::string secret) : _public_client{ 0 }, _api_key { key }, _api_secret{ secret }, flush_params{ 0 }
{
	renew_session();
};

std::string Client::_generate_query(Params& params_obj)

{
	std::map<std::string, std::string> params = params_obj.param_map;
	std::string query;

	for (std::map<std::string, std::string>::iterator itr = params.begin();
		itr != params.end();
		itr++)
	{
		if (itr == params.begin()) query += "?";
		else query += "&";

		query += (itr->first + "=" + itr->second);
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
	std::string endpoint = "/api/v3/order";

	std::string query = Client::_generate_query(parameter_vec);

	std::string signature = HMACsha256(query, this->_api_secret);
	//parameter_vec.set_param() delete?
	//send order...
	query += ("&signature=" + signature);
	std::cout << query;

	if (this->flush_params) parameter_vec.clear_params();

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
	std::string endpoint = "/fapi/v1/time"; // fix
	Json::Value response = (this->_rest_client)->_getreq(this->_BASE_REST_FUTURES + endpoint);

	if (!response["status"])
	{
		std::cout << response;
		return 0;
	}
	
	std::string ex_time = response["serverTime"].asString();

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
	query += ("&signature=" + signature);
	std::cout << query;

	Json::Value response = (this->_rest_client)->_postreq(this->_BASE_REST_FUTURES + query, query); // delete??
	std::cout << response; // delete!

	if (this->flush_params) parameter_vec.clear_params();

	return response;
}

// Params definitions

template <typename PT>
void Params::set_param(std::string key, PT value)
{
	param_map[key] = std::to_string(value);
}
template <>
void Params::set_param<std::string>(std::string key, std::string value)
{
	param_map[key] = value;
}

void Params::clear_params()
{
	this->param_map.clear();
}
