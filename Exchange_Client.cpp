
#include "CryptoExtensions.h"

// Client definitions



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
		if (itr != params.begin()) query += "&";

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
	delete _ws_client;
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
	std::string ex_time = (this->_rest_client)->_getreq(this->_BASE_REST + endpoint)["response"]["serverTime"].asString();

	return std::atoll(ex_time.c_str());
}

bool SpotClient::ping_client()
{
	try
	{
		std::string endpoint = "/api/v3/ping";
		Json::Value ping_response = (this->_rest_client)->_getreq(this->_BASE_REST + endpoint)["response"];
		return (ping_response != Json::nullValue);
	}
	catch (...)
	{
		throw("bad_ping");
	}
}

void SpotClient::init_ws(std::string host, std::string port)
{
	this->_ws_client = new WebsocketClient{this->_WS_BASE, this->_WS_PORT};
}

Json::Value SpotClient::send_order(Params& param_obj)
{

	std::string endpoint = "/api/v3/order";
	param_obj.set_param<unsigned long long>("timestamp", local_timestamp());
	std::string query = Client::_generate_query(param_obj);

	std::string signature = HMACsha256(query, this->_api_secret);
	query += ("&signature=" + signature);
	query = "?" + query;
	std::cout << query;

	std::cout << this->_BASE_REST_FUTURES + endpoint + query;
	Json::Value response = (this->_rest_client)->_postreq(this->_BASE_REST + endpoint + query);

	if (this->flush_params) param_obj.clear_params();

	return response;

}



// FuturesClient definitions

FuturesClient::FuturesClient()
	: Client()
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
	std::string ex_time = (this->_rest_client)->_getreq(this->_BASE_REST_FUTURES + endpoint)["response"]["serverTime"].asString();
	
	return std::atoll(ex_time.c_str());
}

bool FuturesClient::ping_client()
{
	try
	{
		std::string endpoint = "/fapi/v1/ping";
		Json::Value ping_response = (this->_rest_client)->_getreq(this->_BASE_REST_FUTURES + endpoint)["response"];
		return (ping_response != Json::nullValue);
	}
	catch (...)
	{ 
		throw("bad_ping");
	}
}

void FuturesClient::init_ws(std::string host, std::string port)
{
	this->_ws_client = new WebsocketClient{ this->_WS_BASE_FUTURES, this->_WS_PORT };
}

Json::Value FuturesClient::send_order(Params& param_obj)
{
	std::string endpoint = "/fapi/v1/order";
	param_obj.set_param<unsigned long long>("timestamp", local_timestamp());
	std::string query = Client::_generate_query(param_obj);

	std::string signature = HMACsha256(query, this->_api_secret);
	query += ("&signature=" + signature);
	query = "?" + query;

	std::cout << this->_BASE_REST_FUTURES + endpoint + query;
	Json::Value response = (this->_rest_client)->_postreq(this->_BASE_REST_FUTURES + endpoint + query); // return entire json?

	if (this->flush_params) param_obj.clear_params();

	return response;
}

Json::Value FuturesClient::fetch_balances(Params& param_obj)
{
	std::string endpoint = "/fapi/v2/balance";

	param_obj.set_param<unsigned long long>("timestamp", local_timestamp());
	std::string query = Client::_generate_query(param_obj);

	std::string signature = HMACsha256(query, this->_api_secret);
	query += ("&signature=" + signature);
	query = "?" + query;

	std::cout << this->_BASE_REST_FUTURES + endpoint + query;
	Json::Value response = (this->_rest_client)->_getreq(this->_BASE_REST_FUTURES + endpoint + query);

	if (this->flush_params) param_obj.clear_params();


	return response;
}


void FuturesClient::aggTrade(std::string symbol)
{
	this->_ws_client->start_stream("/ws/btcusdt@aggTrade"); // todo: delete 'btcusdt'
}



// Params definitions

Params::Params() {};

Params::Params(Params& params_obj)
{
	this->param_map = params_obj.param_map;
}

Params::Params(const Params& params_obj)
{
	this->param_map = params_obj.param_map;
}

Params& Params::operator=(Params& params_obj)
{
	this->param_map = params_obj.param_map;

	return *this;
}

Params& Params::operator=(const Params& params_obj)
{
	this->param_map = params_obj.param_map;

	return *this;
}

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
