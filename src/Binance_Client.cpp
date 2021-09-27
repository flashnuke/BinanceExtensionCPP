
#include "../include/Binance_Client.h"

//  ------------------------------ Start Client General methods - Infrastructure

const std::string _BASE_REST_FUTURES_USDT{ "https://fapi.binance.com" };
const std::string _BASE_REST_FUTURES_COIN{ "https://dapi.binance.com" };
const std::string _BASE_REST_FUTURES_TESTNET{ "https://testnet.binancefuture.com" };
const std::string _BASE_REST_SPOT{ "https://api.binance.com" };
const std::string _BASE_REST_OPS{ "https://vapi.binance.com" };
const std::string _BASE_REST_OPS_TESTNET{ "https://testnet.binanceops.com" };
const std::string _WS_BASE_FUTURES_USDT{ "fstream.binance.com" };
const std::string _WS_BASE_FUTURES_USDT_TESTNET{ "stream.binancefuture.com" };
const std::string _WS_BASE_FUTURES_COIN{ "dstream.binance.com" };
const std::string _WS_BASE_FUTURES_COIN_TESTNET{ "dstream.binancefuture.com" };
const std::string _WS_BASE_SPOT{ "stream.binance.com" };
const std::string _WS_BASE_OPS{ "vstream.binance.com" };
const std::string _WS_BASE_OPS_TESTNET{ "testnetws.binanceops.com" };
const unsigned int _WS_PORT_SPOT{ 9443 };
const unsigned int _WS_PORT_FUTURES{ 443 };
const unsigned int _WS_PORT_OPS{ 443 };

/**
	An abstract constructor - called by the CRTP lower classes
	Public Client
*/
template<typename T>
Client<T>::Client(T& exchange_client) : _public_client{ 1 }, refresh_listenkey_interval{ 1800 }
{
	try
	{
		this->init_rest_session(); // important to init rest first - ws is dependant on it
		this->_ws_client = new WebsocketClient<T>{ &exchange_client, "", 0 };
		this->init_ws_session();
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	An abstract constructor - called by the CRTP lower classes
	Private Client
	@param key - an API key string
	@param secret - an API secret string
*/
template<typename T>
Client<T>::Client(T& exchange_client, const std::string key, const std::string secret) : _public_client{ 0 }, _api_key{ key }, _api_secret{ secret }, refresh_listenkey_interval{ 1800 }
{
	try
	{
		this->init_rest_session();
		this->_ws_client = new WebsocketClient<T>{ &exchange_client, "", 0 }; 
		this->init_ws_session();
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	The destructor - no dynamic memory is used
*/
template <typename T>
Client<T>::~Client()
{};


//  ------------------------------ End | Client General methods - Infrastructure

//  ------------------------------ Start | Client CRTP methods - Infrastructure

/**
	Initialize a websocket session
*/
template<typename T>
void Client<T>::init_ws_session()
{
	try
	{
		static_cast<T*>(this)->v_init_ws_session();
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get a listen key from the exchange
	@return a string representing the listen key
*/
template<typename T>
std::string Client<T>::get_listen_key()
{
	try
	{
		return static_cast<T*>(this)->v_get_listen_key();
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}
	
/**
	Ping the exchange with an existing listen key
	@param listen_key - a string representing the listen key
	@return the json returned by the request
*/
template<typename T>
Json::Value Client<T>::ping_listen_key(const std::string& listen_key) 
{
	try
	{
		return static_cast<T*>(this)->v_ping_listen_key(listen_key);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Revoke an existing listen key
	@param listen_key - a string of the key to be revoked
	@return the json returned by the request
*/
template<typename T>
Json::Value Client<T>::revoke_listen_key(const std::string& listen_key)
{
	try
	{
		return static_cast<T*>(this)->v_revoke_listen_key(listen_key);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}


//  ------------------------------ End | Client CRTP methods - Infrastructure

//  ------------------------------ Start | Client CRTP methods - Market Data Endpoints 

/**
	Get an epoch timestamp of th exchange time
	@return unsigned long long representing the epoch time
*/
template<typename T>
unsigned long long Client<T>::exchange_time()
{
	try
	{
		return static_cast<T*>(this)->v_exchange_time();
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Ping the exchange
	@return a boolean representing success or failure
*/
template<typename T>
bool Client<T>::ping_client() 
{
	try
	{
		return static_cast<T*>(this)->v_ping_client();
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get exchange info
	@return the json returned by the request
*/
template<typename T>
Json::Value Client<T>::exchange_info()
{
	try
	{
		return static_cast<T*>(this)->v_exchange_info();
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get order book
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template<typename T>
Json::Value Client<T>::order_book(const Params* params_ptr) 
{
	try
	{
		return static_cast<T*>(this)->v_order_book(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get recent public trades
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template<typename T>
Json::Value Client<T>::public_trades_recent(const Params* params_ptr)
{
	try
	{
		return static_cast<T*>(this)->v_public_trades_recent(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get historical public trades
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template<typename T>
Json::Value Client<T>::public_trades_historical(const Params* params_ptr) 
{
	try
	{
		return static_cast<T*>(this)->v_public_trades_historical(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get aggregated public trades
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template<typename T>
Json::Value Client<T>::public_trades_agg(const Params* params_ptr) 
{
	try
	{
		return static_cast<T*>(this)->v_public_trades_agg(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get historical candlesticks data
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template<typename T>
Json::Value Client<T>::klines(const Params* params_ptr)
{
	try
	{
		return static_cast<T*>(this)->v_klines(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get daily ticker statistics
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template<typename T>
Json::Value Client<T>::daily_ticker_stats(const Params* params_ptr)
{
	try
	{
		return static_cast<T*>(this)->v_daily_ticker_stats(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get individual ticker statistics
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template<typename T>
Json::Value Client<T>::get_ticker(const Params* params_ptr)
{
	try
	{
		return static_cast<T*>(this)->v_get_ticker(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get orderbook ticker statistics
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template<typename T>
Json::Value Client<T>::get_order_book_ticker(const Params* params_ptr) 
{
	try
	{
		return static_cast<T*>(this)->v_get_order_book_ticker(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

//  ------------------------------ End | Client CRTP methods - Market Data Endpoints 


//  ------------------------------ Start | Client CRTP methods - Trade Endpoints

/**
	Place a new order - testing
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template<typename T>
Json::Value Client<T>::test_new_order(const Params* params_ptr) 
{
	try
	{
		return static_cast<T*>(this)->v_test_new_order(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Place a new order
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template<typename T>
Json::Value Client<T>::new_order(const Params* params_ptr) 
{
	try
	{
		return static_cast<T*>(this)->v_new_order(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Modify order
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template<typename T>
Json::Value Client<T>::modify_order(const Params* params_ptr)
{
	try
	{
		return static_cast<T*>(this)->v_modify_order(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Cancel an order
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template<typename T>
Json::Value Client<T>::cancel_order(const Params* params_ptr) 
{
	try
	{
		return static_cast<T*>(this)->v_cancel_order(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Cancel all orders
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template<typename T>
Json::Value Client<T>::cancel_all_orders(const Params* params_ptr) 
{
	try
	{
		return static_cast<T*>(this)->v_cancel_all_orders(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Query an order
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template<typename T>
Json::Value Client<T>::query_order(const Params* params_ptr) 
{
	try
	{
		return static_cast<T*>(this)->v_query_order(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get open orders
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template<typename T>
Json::Value Client<T>::open_orders(const Params* params_ptr) 
{
	try
	{
		return static_cast<T*>(this)->v_open_orders(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get all orders
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template<typename T>
Json::Value Client<T>::all_orders(const Params* params_ptr) 
{
	try
	{
		return static_cast<T*>(this)->v_all_orders(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get account info
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template<typename T>
Json::Value Client<T>::account_info(const Params* params_ptr) 
{
	try
	{
		return static_cast<T*>(this)->v_account_info(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get account trades list
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template<typename T>
Json::Value Client<T>::account_trades_list(const Params* params_ptr) 
{
	try
	{
		return static_cast<T*>(this)->v_account_trades_list(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}


//  ------------------------------ End | Client CRTP methods - Trade Endpoints

//  ------------------------------ Start | Client global + CRTP methods - WS Streams


//  ------------------------------ End | Client Global + CRTP methods - WS Streams


//  ------------------------------ Start | Client General methods - Infrastructure


/**
	Initialize a rest session
	@return bool value for success (pings the exchange to determine success)
*/
template <typename T>
bool Client<T>::init_rest_session()
{
	try
	{
		if (this->_rest_client) delete this->_rest_client;

		this->_rest_client = new RestSession{};
		if (!this->_public_client)
		{
			this->set_headers(this->_rest_client);
		}
		if (!(this->ping_client())) return 0;

		return 1;
	}
	catch (...)
	{
		BadSetupSessionREST e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}

}

/**
	Send a custom GET request
	@param base - base URL of request path
	@param endpoint - endpoint of request path
	@param params_ptr - a pointer to the request Params object
	@bool signature - a bool for whether to sign the request or not
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::custom_get_req(const std::string& base, const std::string& endpoint, const Params* params_ptr, const bool& signature)
{
	try
	{
		std::string query = this->_generate_query(params_ptr, signature);
		std::string full_path = base + endpoint + query;
		return this->_rest_client->_getreq(full_path);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Send a custom POST request
	@param base - base URL of request path
	@param endpoint - endpoint of request path
	@param params_ptr - a pointer to the request Params object
	@bool signature - a bool for whether to sign the request or not
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::custom_post_req(const std::string& base, const std::string& endpoint, const Params* params_ptr, const bool& signature)
{
	try
	{
	std::string query = this->_generate_query(params_ptr, signature);
	std::string full_path = base + endpoint + query;
	return this->_rest_client->_postreq(full_path);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Send a custom PUT request
	@param base - base URL of request path
	@param endpoint - endpoint of request path
	@param params_ptr - a pointer to the request Params object
	@bool signature - a bool for whether to sign the request or not
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::custom_put_req(const std::string& base, const std::string& endpoint, const Params* params_ptr, const bool& signature)
{
	try
	{
		std::string query = this->_generate_query(params_ptr, signature);
		std::string full_path = base + endpoint + query;
		return this->_rest_client->_putreq(full_path);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
}
}

/**
	Send a custom DELETE request
	@param base - base URL of request path
	@param endpoint - endpoint of request path
	@param params_ptr - a pointer to the request Params object
	@bool signature - a bool for whether to sign the request or not
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::custom_delete_req(const std::string& base, const std::string& endpoint, const Params* params_ptr, const bool& signature)
{
	try
	{
		std::string query = this->_generate_query(params_ptr, signature);
		std::string full_path = base + endpoint + query;
		return this->_rest_client->_deletereq(full_path);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Set headers for REST session
	@param rest_client - a pointer to the RestSession object
	@return bool - return true if success
*/
template <typename T>
bool Client<T>::set_headers(RestSession* rest_client)
{
	try
	{
	std::string key_header = "X-MBX-APIKEY:" + this->_api_key;
	struct curl_slist* auth_headers;
	auth_headers = curl_slist_append(NULL, key_header.c_str());

	curl_easy_setopt((rest_client->_get_handle), CURLOPT_HTTPHEADER, auth_headers);
	curl_easy_setopt((rest_client->_post_handle), CURLOPT_HTTPHEADER, auth_headers);
	curl_easy_setopt((rest_client->_put_handle), CURLOPT_HTTPHEADER, auth_headers);
	curl_easy_setopt((rest_client->_delete_handle), CURLOPT_HTTPHEADER, auth_headers);

	return 1;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	} 
	catch (...)
	{
		BadSetupHeadersREST e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}; 

}

/**
	Set verbose state for REST session (prints more info regarding requests)
	@param state - a bool for enabling (1) or disabling (0)
*/
template <typename T>
void Client<T>::rest_set_verbose(const bool& state)
{
	if (state) this->_rest_client->set_verbose(1L);
	else this->_rest_client->set_verbose(0);
}

/**
	Generate a REST request query
	@param params_ptr - a pointer to the request Params object
	@param sign_query - a bool for whether to sign the request or not
	@return string - the query generated
*/
template <typename T>
std::string Client<T>::_generate_query(const Params* params_ptr, const bool& sign_query) const
{
	try
	{
		std::string query{};
		bool no_params{ 1 };
		if (params_ptr && (!params_ptr->empty())) // if ptr passed and it's not empty
		{
			no_params = 0;
			for (std::unordered_map<std::string, std::string>::const_iterator itr = params_ptr->param_map.begin();
				itr != params_ptr->param_map.end();
				itr++)
			{

				if (itr != params_ptr->param_map.begin()) query += "&";

				query += (itr->first + "=" + itr->second);
			}
		}

		if (sign_query)
		{
			unsigned long long timestamp = local_timestamp();
			query = no_params ? "timestamp=" : query + "&timestamp=";

			query += std::to_string(timestamp);

			std::string signature = HMACsha256(query, this->_api_secret);
			query += "&signature=" + signature;
		}

		query = query.empty() ? "" : ("?" + query); // no need for the question mark if empty

		return query;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
	catch (...)
	{
		BadQuery e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}


//  ------------------------------ End | Client General methods - Infrastructure


// ***************************************************************************


//  ------------------------------ Start | Client Wallet - User Wallet Endpoints

// ------ Class methods


/**
	A constructor - called directly by the user
	@param client_obj - the exchange client object
*/
template <typename T>
Client<T>::Wallet::Wallet(Client<T>& client_obj)
	: user_client{ &client_obj } // snatching pointer and releasing later on to avoid deleting this reference
{
	if (user_client->_public_client)
	{
		MissingCredentials e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	};
}

/**
	A constructor - called directly by the user
	@param client_obj - the exchange client object (constant)
*/
template <typename T>
Client<T>::Wallet::Wallet(const Client<T>& client_obj)
	: user_client{ &client_obj } // snatching pointer and releasing later on to avoid deleting this reference
{
	if (user_client->_public_client)
	{
		MissingCredentials e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	};
}

/**
	A destructor
	since 'user_client' is a reference to an exchange client outside the class
	it should be set to nullptr prior deletion
*/
template <typename T>
Client<T>::Wallet::~Wallet()
{
	user_client = nullptr;
}

// ------ Endpoint methods

/**
	Get all coins
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::Wallet::get_all_coins(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/capital/config/getall";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get a daily wallet snapshot
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::Wallet::daily_snapshot(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/accountSnapshot";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;

	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Turn on / off fast withdrawals
	@param state - 1 to enable, 0 to disable
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::Wallet::fast_withdraw_switch(const bool& state)
{
	try
	{
		Params temp_params;
		std::string endpoint = state ? "/sapi/v1/account/enableFastWithdrawSwitch" : "/sapi/v1/account/disableFastWithdrawSwitch";
		std::string full_path = _BASE_REST_SPOT + endpoint;
		std::string query = user_client->_generate_query(&temp_params, 1);
		Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Submit a withdraw request
	@param params_ptr - a pointer to the request Params object
	@param SAPI - a bool for whether to use SAPI or WAPI endpoint
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::Wallet::withdraw_balances(const Params* params_ptr)
{
	try
	{
		std::string endpoint = "/sapi/v1/capital/withdraw/apply";
		std::string full_path = _BASE_REST_SPOT + endpoint;
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Fetch deposit history
	@param params_ptr - a pointer to the request Params object
	@param network - a bool for whether to use an endpoint that support network
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::Wallet::deposit_history(const Params* params_ptr)
{
	try
	{
		std::string endpoint = "/sapi/v1/capital/deposit/hisrec";
		std::string full_path = _BASE_REST_SPOT + endpoint;
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Fetch withdraw history
	@param params_ptr - a pointer to the request Params object
	@param network - a bool for whether to fetch with network
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::Wallet::withdraw_history(const Params* params_ptr)
{
	try
	{
		std::string endpoint = "/sapi/v1/capital/withdraw/history";
		std::string full_path = _BASE_REST_SPOT + endpoint;
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Fetch deposit address 
	@param params_ptr - a pointer to the request Params object
	@param network - a bool for whether to fetch with network
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::Wallet::deposit_address(const Params* params_ptr)
{
	try
	{
		std::string endpoint = "/sapi/v1/capital/deposit/address";
		std::string full_path = _BASE_REST_SPOT + endpoint;
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Convert dust assets to BNB
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::Wallet::dust_transfer(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/asset/dust";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Query asset dividend record
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::Wallet::asset_dividend_records(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/asset/assetDividend";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Make User Universal Transfer
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::Wallet::make_user_transfer_universal(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/asset/transfer";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Query User Universal Transfer
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::Wallet::query_user_transfer_universal(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/asset/transfer";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Funding Wallet
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::Wallet::funding_wallet(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/asset/get-funding-asset";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get API Key Permission
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::Wallet::get_api_key_permission(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/account/apiRestrictions";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

//  ------------------------------ End | Client Wallet - User Wallet Endpoints

// ***************************************************************************


//  ------------------------------ Start | Client FuturesWallet - User FuturesWallet Endpoints

// ------ Class methods


/**
	A constructor - called directly by the user
	@param client_obj - the exchange client object
*/
template <typename T>
Client<T>::FuturesWallet::FuturesWallet(Client<T>& client_obj)
	: user_client{ &client_obj }
{
	if (user_client->_public_client)
	{
		MissingCredentials e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	};
}

/**
	A constructor - called directly by the user
	@param client_obj - the exchange client object (constant)
*/
template <typename T>
Client<T>::FuturesWallet::FuturesWallet(const Client<T>& client_obj)
	: user_client{ &client_obj }
{
	if (user_client->_public_client)
	{
		MissingCredentials e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	};
}

/**
	Destructor 
	set 'user_client' as nullptr to avoid deleting the exchange client
	object passed from outside the class (reference)
*/
template <typename T>
Client<T>::FuturesWallet::~FuturesWallet()
{
	user_client = nullptr;
}

// ------ Endpoint methods


/**
	Execute transfer between spot account and futures account
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::FuturesWallet::futures_transfer(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/futures/transfer";
		std::string query = this->user_client->_generate_query(params_ptr, 1);
		Json::Value response = (this->user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get Future Account Transaction History List
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::FuturesWallet::futures_transfer_history(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/futures/transfer";
		std::string query = this->user_client->_generate_query(params_ptr, 1);
		Json::Value response = (this->user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Borrow For Cross-Collateral
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_borrow(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/futures/loan/borrow";
		std::string query = this->user_client->_generate_query(params_ptr, 1);
		Json::Value response = (this->user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get Cross-Collateral Borrow History
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_borrow_history(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/futures/loan/borrow/history";
		std::string query = this->user_client->_generate_query(params_ptr, 1);
		Json::Value response = (this->user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Repay For Cross-Collateral 
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_cross_repay(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/futures/loan/repay";
		std::string query = this->user_client->_generate_query(params_ptr, 1);
		Json::Value response = (this->user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get Cross-Collateral Repayment History 
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_repay_history(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/futures/loan/repay/history";
		std::string query = this->user_client->_generate_query(params_ptr, 1);
		Json::Value response = (this->user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get Cross-Collateral Wallet 
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_wallet(const Params* params_ptr, const unsigned int version)
{
	try
	{
		
		std::string full_path = _BASE_REST_SPOT + "/sapi/v" + std::to_string(version) + "/futures/loan/wallet";
		std::string query = this->user_client->_generate_query(params_ptr, 1);
		Json::Value response = (this->user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get Cross-Collateral Information
	all collateral data will be returned if collateralCoin is not sent
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_info(const Params* params_ptr, const unsigned int version)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v" + std::to_string(version) + "/futures/loan/configs";
		std::string query = this->user_client->_generate_query(params_ptr, 1);
		Json::Value response = (this->user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Calculate Rate After Adjust Cross-Collateral LTV
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_adjust_calc_rate(const Params* params_ptr, const unsigned int version)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v" + std::to_string(version) + "/futures/loan/calcAdjustLevel";
		std::string query = this->user_client->_generate_query(params_ptr, 1);
		Json::Value response = (this->user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get Max Amount for Adjust Cross-Collateral LTV
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_adjust_get_max(const Params* params_ptr, const unsigned int version)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v" + std::to_string(version) + "/futures/loan/calcMaxAdjustAmount";
		std::string query = this->user_client->_generate_query(params_ptr, 1);
		Json::Value response = (this->user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Adjust Cross-Collateral LTV
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_adjust(const Params* params_ptr, const unsigned int version)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v" + std::to_string(version) + "/futures/loan/adjustCollateral";
		std::string query = this->user_client->_generate_query(params_ptr, 1);
		Json::Value response = (this->user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get Adjust Cross-Collateral LTV History
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_adjust_history(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/futures/loan/adjustCollateral/history";
		std::string query = this->user_client->_generate_query(params_ptr, 1);
		Json::Value response = (this->user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get Cross-Collateral Liquidation History
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_liquidation_history(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/futures/loan/liquidationHistory";
		std::string query = this->user_client->_generate_query(params_ptr, 1);
		Json::Value response = (this->user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Check the maximum and minimum limit when repay with collateral
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_check_repay_limit(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/futures/loan/collateralRepayLimit";
		std::string query = this->user_client->_generate_query(params_ptr, 1);
		Json::Value response = (this->user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get Collateral Repay Quote
	* the quote will be valid within 25 seconds

	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_get_repay_quote(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/futures/loan/collateralRepay";
		std::string query = this->user_client->_generate_query(params_ptr, 1);
		Json::Value response = (this->user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Repay with Collateral

	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_repay(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/futures/loan/collateralRepay";
		std::string query = this->user_client->_generate_query(params_ptr, 1);
		Json::Value response = (this->user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Check collateral repayment result

	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_repay_result(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/futures/loan/collateralRepayResult";
		std::string query = this->user_client->_generate_query(params_ptr, 1);
		Json::Value response = (this->user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Cross-Collateral Interest History

	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_cross_interest_history(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/futures/loan/interestHistory";
		std::string query = this->user_client->_generate_query(params_ptr, 1);
		Json::Value response = (this->user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

//  ------------------------------ End | Client FuturesWallet - User FuturesWallet Endpoints


// ***************************************************************************


//  ------------------------------ Start | Client SubAccount - User SubAccount Endpoints

// ------ Class methods


/**
	A constructor - called directly by the user
	@param client_obj - the exchange client object
*/
template <typename T>
Client<T>::SubAccount::SubAccount(Client<T>& client_obj)
	: user_client{ &client_obj } // snatching pointer and releasing later on to avoid deleting this reference
{
	if (user_client->_public_client)
	{
		MissingCredentials e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	};
}

/**
	A constructor - called directly by the user
	@param client_obj - the exchange client object (constant)
*/
template <typename T>
Client<T>::SubAccount::SubAccount(const Client<T>& client_obj)
	: user_client{ &client_obj } // snatching pointer and releasing later on to avoid deleting this reference
{
	if (user_client->_public_client)
	{
		MissingCredentials e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	};
}

/**
	Destructor
	set 'user_client' as nullptr to avoid deleting the exchange client
	object passed from outside the class (reference)
*/
template <typename T>
Client<T>::SubAccount::~SubAccount()
{
	user_client = nullptr;
}

// ------ Endpoint methods


/**
	Create a Virtual Sub-account(For Master Account)

	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::SubAccount::create_virtual_subaccount(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/sub-account/virtualSubAccount";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Query Sub-account List SAPI (For Master Account)

	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::SubAccount::query_subaccount_list_sapi(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/sub-account/list";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Query Sub-account Spot Asset Transfer History (SAPI For Master Account)
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::SubAccount::transfer_spot_subaccount_history(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/sub-account/sub/transfer/history";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Query Sub-account Futures Asset Transfer History(For Master Account)
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::SubAccount::futures_transfer_master_history(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/sub-account/futures/internalTransfer";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Sub-account Futures Asset Transfer(For Master Account)
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::SubAccount::futures_transfer_master_to_subaccount(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/sub-account/futures/internalTransfer";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Query Sub-account Spot Assets Summary (For Master Account)
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::SubAccount::get_subaccount_balances_summary(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/sub-account/spotSummary";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get Sub-account Deposit Address (For Master Account)
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::SubAccount::get_subaccount_deposit_address(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/capital/deposit/subAddress";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get Sub-account Deposit History (For Master Account)
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::SubAccount::get_subaccount_deposit_history(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/capital/deposit/subHisrec";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get Sub-account's Status on Margin/Futures(For Master Account)
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::SubAccount::get_subaccount_future_margin_status(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/sub-account/status";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Enable Margin for Sub-account (For Master Account)
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::SubAccount::enable_subaccount_margin(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/sub-account/margin/enable";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get Detail on Sub-account's Margin Account (For Master Account)
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::SubAccount::get_subaccount_margin_status(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/sub-account/margin/account";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get Summary of Sub-account's Margin Account (For Master Account)
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::SubAccount::get_subaccount_margin_summary(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/sub-account/margin/accountSummary";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Enable Futures for Sub-account (For Master Account)
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::SubAccount::enable_subaccount_futures(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/sub-account/futures/enable";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get Detail on Sub-account's Futures Account (For Master Account)
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::SubAccount::get_subaccount_futures_status(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/sub-account/futures/account";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get Summary of Sub-account's Futures Account (For Master Account)
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::SubAccount::get_subaccount_futures_summary(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/sub-account/futures/accountSummary";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get Futures Postion-Risk of Sub-account (For Master Account)
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::SubAccount::get_subaccount_futures_positionrisk(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/sub-account/futures/positionRisk";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Futures Transfer for Sub-account(For Master Account)
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::SubAccount::transfer_to_subaccount_futures(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/sub-account/futures/transfer";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Margin Transfer for Sub-account(For Master Account)
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::SubAccount::transfer_to_subaccount_margin(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/sub-account/margin/transfer";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Transfer to Sub-account of Same Master(For Sub-account)
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::SubAccount::transfer_subaccount_to_subaccount(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/sub-account/transfer/subToSub";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Transfer to Master(For Sub-account)
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::SubAccount::transfer_subaccount_to_master(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/sub-account/transfer/subToMaster";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Sub-account Transfer History (For Sub-account)
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::SubAccount::transfer_subaccount_history(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/sub-account/transfer/subUserHistory";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Universal Transfer (For Master Account)
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::SubAccount::make_universal_transfer(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/sub-account/universalTransfer";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Query Universal Transfer History
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::SubAccount::get_universal_transfer_history(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/sub-account/universalTransfer";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get Detail on Sub-account's Futures Account V2 (For Master Account)
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::SubAccount::get_details_subaccount_futures(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v2/sub-account/futures/account";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get Summary of Sub-account's Futures Account V2 (For Master Account)
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::SubAccount::get_summary_subaccount_futures(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v2/sub-account/futures/accountSummary";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get Futures Position-Risk of Sub-account V2 (For Master Account)
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::SubAccount::get_positionrisk_subaccount_futures(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v2/sub-account/futures/positionRisk";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Enable Leverage Token for Sub-account (For Master Account)

	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::SubAccount::enable_leverage_token_subaccount(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/sub-account/blvt/enable";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Deposit assets into the managed sub-account（For Investor Master Account）

	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::SubAccount::deposit_asset_subaccount(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/managed-subaccount/deposit";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Query managed sub-account asset details（For Investor Master Account）

	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::SubAccount::query_asset_subaccount(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/managed-subaccount/asset";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Withdrawal (assets) from the managed sub-account（For Investor Master Account

	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::SubAccount::withdraw_asset_subaccount(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/managed-subaccount/withdraw";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

//  ------------------------------ End | Client SubAccount - User SubAccount Endpoints

// ***************************************************************************

//  ------------------------------ Start | Client MarginAccount - User MarginAccount Endpoints


/**
	A constructor - called directly by the user
	@param client_obj - the exchange client object
*/
template <typename T>
Client<T>::MarginAccount::MarginAccount(Client<T>& client_obj)
	: user_client{ &client_obj } // snatching pointer and releasing later on to avoid deleting this reference
{
	if (user_client->_public_client)
	{
		MissingCredentials e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	};
}

/**
	A constructor - called directly by the user
	@param client_obj - the exchange client object (constant)
*/
template <typename T>
Client<T>::MarginAccount::MarginAccount(const Client<T>& client_obj)
	: user_client{ &client_obj } // snatching pointer and releasing later on to avoid deleting this reference
{
	if (user_client->_public_client)
	{
		MissingCredentials e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	};
}

/**
	Destructor
	set 'user_client' as nullptr to avoid deleting the exchange client
	object passed from outside the class (reference)
*/
template <typename T>
Client<T>::MarginAccount::~MarginAccount()
{
	user_client = nullptr;
}

// ------ Endpoint methods

/**
	Cross Margin Account Transfer
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_transfer(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/transfer";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Margin Account Borrow - apply for a loan
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_borrow(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/loan";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Margin Account Repay - Repay loan for margin account
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_repay(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/repay";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Query Margin Asset
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_asset_query(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/asset";
		std::string query = params_ptr ? this->user_client->_generate_query(params_ptr) : "";
		Json::Value response = (user_client->_rest_client)->_getreq(full_path);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Query Cross Margin Pair
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_pair_query(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/pair";
		std::string query = params_ptr ? this->user_client->_generate_query(params_ptr) : "";
		Json::Value response = (user_client->_rest_client)->_getreq(full_path);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get All Margin Assets 
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_all_assets_query()
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/allAssets";
		Json::Value response = (user_client->_rest_client)->_getreq(full_path);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get All Cross Margin Pairs 
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_all_pairs_query()
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/allPairs";
		Json::Value response = (user_client->_rest_client)->_getreq(full_path);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Query Margin PriceIndex
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_price_index(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/transfer";
		std::string query = params_ptr ? this->user_client->_generate_query(params_ptr) : "";
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Margin Account New Order - Post a new order for margin account
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_new_order(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/order";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Margin Account Cancel Order - Cancel an active order for margin account
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_cancel_order(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/order";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_deletereq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Margin Account Cancel All Open Orders (On a Symbol)
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_cancel_all_orders(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/openOrders";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_deletereq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get Cross Margin Transfer History
	*Response in descending order
	*Returns data for last 7 days by default
	*Set archived to true to query data from 6 months ago
	
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_transfer_history(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/transfer";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Query Loan Record
	*txId or startTime must be sent. txId takes precedence.
	*Response in descending order
	*If isolatedSymbol is not sent, crossed margin data will be returned
	*Set archived to true to query data from 6 months ago

	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_loan_record(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/loan";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Query Repay Record
	*txId or startTime must be sent. txId takes precedence.
	*Response in descending order
	*If isolatedSymbol is not sent, crossed margin data will be returned
	*Set archived to true to query data from 6 months ago

	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_repay_record(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/repay";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get Interest History
	*Response in descending order
	*If isolatedSymbol is not sent, crossed margin data will be returned
	*Set archived to true to query data from 6 months ago
	*type in response has 4 enums:
		**PERIODIC interest charged per hour
		**ON_BORROW first interest charged on borrow
		**PERIODIC_CONVERTED interest charged per hour converted into BNB
		**ON_BORROW_CONVERTED first interest charged on borrow converted into BNB

	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_interest_history(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/interestHistory";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get Force Liquidation Record
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_liquidations_record(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/forceLiquidationRec";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Query Cross Margin Account Details
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_account_info(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/account";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Query Margin Account's Order
	*Either orderId or origClientOrderId must be sent.
	*For some historical orders cummulativeQuoteQty will be < 0, meaning the data is not available at this time

	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_account_order(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/order";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Query Margin Account's Open Orders
	*If the symbol is not sent, orders for all symbols will be returned in an array.
	*When all symbols are returned, the number of requests counted against the rate limiter is equal to the number of symbols currently trading on the exchange.
	*If isIsolated ="TRUE", symbol must be sent.
	
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_account_open_orders(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/openOrders";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Query Margin Account's Open Orders
	*If orderId is set, it will get orders >= that orderId. Otherwise most recent orders are returned.
	*For some historical orders cummulativeQuoteQty will be < 0, meaning the data is not available at this time.

	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_account_all_orders(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/allOrders";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Margin Account New OCO

	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_new_oco_order(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/order/oco";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Margin Account Cancel OCO
	*Canceling an individual leg will cancel the entire OCO

	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_cancel_oco_order(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/orderList";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_deletereq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Query Margin Account's OCO

	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_query_oco_orders(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/orderList";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Query Margin Account's all OCO

	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_query_oco_all_orders(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/allOrderList";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Query Margin Account's Open OCO

	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_query_oco_open_orders(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/openOrderList";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Query Margin Account's Trade List
	*If fromId is set, it will get trades >= that fromId. Otherwise most recent trades are returned.
	
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_account_trades_list(const Params* params_ptr)
{
	try
	{ 
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/myTrades";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Query Max Borrow
	*If isolatedSymbol is not sent, crossed margin data will be sent.
	*borrowLimit is also available from https://www.binance.com/en/margin-fee

	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_max_borrow(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/maxBorrowable";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Query Max Transfer-Out Amount
	*If isolatedSymbol is not sent, crossed margin data will be sent.

	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_max_transfer(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/maxTransferable";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get Isolated Margin Transfer

	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_isolated_margin_transfer(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/isolated/transfer";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get Isolated Margin Transfer History

	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_isolated_margin_transfer_history(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/isolated/transfer";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Query Isolated Margin Account Info
	*If "symbols" is not sent, all isolated assets will be returned.
	*If "symbols" is sent, only the isolated assets of the sent symbols will be returned.
	
	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_isolated_margin_account_info(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/isolated/account";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Disable Isolated Margin Account

	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_isolated_margin_disable_account(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/isolated/account";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_deletereq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Enable Isolated Margin Account

	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_isolated_margin_enable_account(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/isolated/account";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Query Enabled Isolated Margin Account Limit

	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_isolated_margin_query_account_limits(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/isolated/accountLimit";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Query Isolated Margin Symbol 

	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_isolated_margin_symbol(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/isolated/pair";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get All Isolated Margin Symbol

	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_isolated_margin_symbol_all(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/isolated/allPairs";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Toggle BNB Burn On Spot Trade And Margin Interest

	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::toggle_bnb_burn(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/bnbBurn";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get BNB Burn Status

	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::get_bnb_burn_status(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/bnbBurn";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Query Margin Interest Rate History

	@param params_ptr - a pointer to the request Params object
	@return the json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::query_margin_interest_rate_history(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/interestRateHistory";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Create a ListenKey - Margin
	@return the ListenKey as std::string
*/
template <typename T>
std::string Client<T>::MarginAccount::margin_get_listen_key()
{
	try
	{
		std::string endpoint = "/sapi/v1/userDataStream";
		std::string full_path = _BASE_REST_SPOT + endpoint;
		Json::Value response = (this->user_client->_rest_client)->_postreq(full_path);
		return response["response"]["listenKey"].asString();
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Create a ListenKey - Isolated Margin
	@param symbol - the symbol
	@return the ListenKey as std::string
*/
template <typename T>
std::string Client<T>::MarginAccount::margin_isolated_get_listen_key(const std::string symbol)
{
	try
	{
		std::string endpoint = "/sapi/v1/userDataStream/isolated?symbol=" + symbol;
		std::string full_path = _BASE_REST_SPOT + endpoint;
		Json::Value response = (this->user_client->_rest_client)->_postreq(full_path);

		return response["response"]["listenKey"].asString();
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Ping/Keep-alive a Listen Key - Margin
	@param listen_key - the ListenKey
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_ping_listen_key(const std::string& listen_key)
{
	try
	{
		std::string endpoint = "/sapi/v1/userDataStream";
		std::string full_path = _BASE_REST_SPOT + endpoint + "?listenKey=" + listen_key;
		Json::Value response = listen_key.empty() ? (this->user_client->_rest_client)->_putreq(full_path) : (this->user_client->_rest_client)->_postreq(full_path);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Ping/Keep-alive a Listen Key - Isolated Margin
	@param listen_key - the ListenKey
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_isolated_ping_listen_key(const std::string& listen_key, const std::string symbol)
{
	try
	{
		std::string endpoint = "/sapi/v1/userDataStream/isolated?symbol=" + symbol;
		std::string full_path = _BASE_REST_SPOT + endpoint + "?listenKey=" + listen_key;
		Json::Value response = listen_key.empty() ? (this->user_client->_rest_client)->_putreq(full_path) : (this->user_client->_rest_client)->_postreq(full_path);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Revoke a Listen Key - Margin
	@param listen_key - the ListenKey
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_revoke_listen_key(const std::string& listen_key)
{
	try
	{
		std::string endpoint = "/sapi/v1/userDataStream";
		std::string full_path = _BASE_REST_SPOT + endpoint + "?listenKey=" + listen_key;
		Json::Value response = (this->user_client->_rest_client)->_deletereq(full_path);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Revoke a Listen Key - Isolated Margin
	@param listen_key - the ListenKey
	@param symbol - the symbol
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::MarginAccount::margin_isolated_revoke_listen_key(const std::string& listen_key, const std::string symbol)
{
	try
	{
		std::string endpoint = "/sapi/v1/userDataStream/isolated?symbol=" + symbol;
		std::string full_path = _BASE_REST_SPOT + endpoint + "?listenKey=" + listen_key;
		Json::Value response = (this->user_client->_rest_client)->_deletereq(full_path);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

//  ------------------------------ End | Client MarginAccount - User MarginAccount Endpoints

// ***************************************************************************

//  ------------------------------ Start | Client Savings - User Savings Endpoints

/**
	A constructor - called directly by the user
	@param client_obj - the exchange client object
*/
template <typename T>
Client<T>::Savings::Savings(Client<T>& client_obj)
	: user_client{ &client_obj } // snatching pointer and releasing later on to avoid deleting this reference
{
	if (user_client->_public_client)
	{
		MissingCredentials e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}; 
}

/**
	A constructor - called directly by the user
	@param client_obj - the exchange client object (constant)
*/
template <typename T>
Client<T>::Savings::Savings(const Client<T>& client_obj)
	: user_client{ &client_obj } // snatching pointer and releasing later on to avoid deleting this reference
{
	if (user_client->_public_client)
	{
		MissingCredentials e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	};
}

/**
	Destructor
	set 'user_client' as nullptr to avoid deleting the exchange client
	object passed from outside the class (reference)
*/
template <typename T>
Client<T>::Savings::~Savings()
{
	user_client = nullptr; 
}

// ------ Endpoint methods

/**
	Get Flexible Product List 
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::Savings::get_product_list_flexible(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/lending/daily/product/list";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get Left Daily Purchase Quota of Flexible Product
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::Savings::get_product_daily_quota_purchase_flexible(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/lending/daily/userLeftQuota";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Purchase Flexible Product
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::Savings::purchase_product_flexible(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/lending/daily/purchase";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get Left Daily Redemption Quota of Flexible Product
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::Savings::get_product_daily_quota_redemption_flexible(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/lending/daily/userRedemptionQuota";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Redeem Flexible Product
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::Savings::redeem_product_flexible(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/lending/daily/redeem";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get Flexible Product Position
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::Savings::get_product_position_flexible(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/lending/daily/token/position";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get Fixed and Activity Project List
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::Savings::get_product_list_fixed(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/lending/project/list";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Purchase Fixed/Activity Project
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::Savings::purchase_product_fixed(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/lending/customizedFixed/purchase";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get Fixed/Activity Project Position
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::Savings::get_product_position_fixed(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/lending/project/position/list";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Lending Account
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::Savings::lending_account(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/lending/union/account";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get Purchase Record
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::Savings::get_purchase_record(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/lending/union/purchaseRecord";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get Redemption Record
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::Savings::get_redemption_record(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/lending/union/redemptionRecord";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get Interest History
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::Savings::get_interest_history(const Params* params_ptr)
{
	try
	{ 
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/lending/union/interestHistory";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Change Fixed/Activity Position to Daily Position
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::Savings::change_fixed_pos_to_daily_pos(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/lending/positionChanged";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

//  ------------------------------ End | Client Savings - User Savings Endpoints


// ***************************************************************************

//  ------------------------------ Start | Client Mining - User Mining Endpoints


/**
	A constructor - called directly by the user
	@param client_obj - the exchange client object
*/
template <typename T>
Client<T>::Mining::Mining(Client<T>& client_obj)
	: user_client{ &client_obj }
{
	if (user_client->_public_client)
	{
		MissingCredentials e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}; 
}

/**
	A constructor - called directly by the user
	@param client_obj - the exchange client object (constant)
*/
template <typename T>
Client<T>::Mining::Mining(const Client<T>& client_obj)
	: user_client{ &client_obj } // snatching pointer and releasing later on to avoid deleting this reference
{
	if (user_client->_public_client)
	{
		MissingCredentials e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	};
}

/**
	Destructor
	set 'user_client' as nullptr to avoid deleting the exchange client
	object passed from outside the class (reference)
*/
template <typename T>
Client<T>::Mining::~Mining()
{
	user_client = nullptr; 
}

// ------ Endpoint methods

/**
	Acquiring Algorithm
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::Mining::algo_list()
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/mining/pub/algoList";
		Json::Value response = (user_client->_rest_client)->_getreq(full_path);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Acquiring CoinName
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::Mining::coin_list()
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/mining/pub/coinList";
		Json::Value response = (user_client->_rest_client)->_getreq(full_path);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Request for Detail Miner List
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::Mining::get_miner_list_detail(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/mining/worker/detail";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Request for Miner List 
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::Mining::get_miner_list(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/mining/worker/list";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Revenue List
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::Mining::revenue_list(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/mining/payment/list";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Extra Bonus List
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::Mining::get_extra_bonus_list(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/mining/payment/other";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Hashrate Resale List
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::Mining::get_hashrate_resale_list(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/mining/hash-transfer/config/details/list";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Hashrate Resale Detail
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::Mining::get_hashrate_resale_detail(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/mining/hash-transfer/profit/details";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Hashrate Resale Request
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::Mining::make_hashrate_resale_request(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/mining/hash-transfer/config";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Cancel hashrate resale configuration
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::Mining::cancel_hashrate_resale_config(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/mining/hash-transfer/config/cancel";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Statistic List 
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::Mining::statistic_list(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/mining/statistics/user/status";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Account List
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::Mining::account_list(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/mining/statistics/user/list";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};


//  ------------------------------ End | Client Mining - User Mining Endpoints


// =======================================================================================================


//  ------------------------------ Start | Client BLVT - User Mining Endpoints


/**
	A constructor - called directly by the user
	@param client_obj - the exchange client object
*/
template <typename T>
Client<T>::BLVT::BLVT(Client<T>& client_obj)
	: user_client{ &client_obj }
{
	if (user_client->_public_client)
	{
		MissingCredentials e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	};
}

/**
	A constructor - called directly by the user
	@param client_obj - the exchange client object (constant)
*/
template <typename T>
Client<T>::BLVT::BLVT(const Client<T>& client_obj)
	: user_client{ &client_obj } // snatching pointer and releasing later on to avoid deleting this reference
{
	if (user_client->_public_client)
	{
		MissingCredentials e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	};
}

/**
	Destructor
	set 'user_client' as nullptr to avoid deleting the exchange client
	object passed from outside the class (reference)
*/
template <typename T>
Client<T>::BLVT::~BLVT()
{
	user_client = nullptr;
}

// ------ Endpoint methods

/**
	Get BLVT Info
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::BLVT::get_blvt_info(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/blvt/tokenInfo";
		std::string query = user_client->_generate_query(params_ptr, 0);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Subscribe BLVT
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::BLVT::subscribe_blvt(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/blvt/subscribe";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Query Subscription Record
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::BLVT::get_subscribe_blvt_history(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/blvt/subscribe/record";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Redeem BLVT
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::BLVT::redeem_blvt(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/blvt/redeem";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Query Redemption Record
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::BLVT::get_redeem_blvt_history(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/blvt/redeem/record";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Query Redemption Record
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::BLVT::get_blvt_user_limit(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/blvt/userLimit";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};


//  ------------------------------ End | Client BLVT - User Mining Endpoints


// =======================================================================================================


//  ------------------------------ Start | Client BSwap - User Mining Endpoints


/**
	A constructor - called directly by the user
	@param client_obj - the exchange client object
*/
template <typename T>
Client<T>::BSwap::BSwap(Client<T>& client_obj)
	: user_client{ &client_obj }
{
	if (user_client->_public_client)
	{
		MissingCredentials e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	};
}

/**
	A constructor - called directly by the user
	@param client_obj - the exchange client object (constant)
*/
template <typename T>
Client<T>::BSwap::BSwap(const Client<T>& client_obj)
	: user_client{ &client_obj } // snatching pointer and releasing later on to avoid deleting this reference
{
	if (user_client->_public_client)
	{
		MissingCredentials e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	};
}

/**
	Destructor
	set 'user_client' as nullptr to avoid deleting the exchange client
	object passed from outside the class (reference)
*/
template <typename T>
Client<T>::BSwap::~BSwap()
{
	user_client = nullptr;
}

// ------ Endpoint methods

/**
	List All Swap Pools
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::BSwap::get_all_swap_pools()
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/bswap/pools";
		Json::Value response = (user_client->_rest_client)->_getreq(full_path);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get liquidity information of a pool
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::BSwap::get_liquidity_info_pool(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/bswap/liquidity";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Add liquidity to a pool
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::BSwap::add_liquidity(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/bswap/liquidityAdd";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Remove liquidity from a pool
	* type include SINGLE and COMBINATION
	* asset is mandatory for single asset removal
	
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::BSwap::remove_liquidity(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/bswap/liquidityRemove";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get liquidity operation (add/remove) records
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::BSwap::get_liquidity_ops_record(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/bswap/liquidityOps";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Request a quote for swap quote asset (selling asset) for base asset (buying asset),
	essentially price/exchange rates.

	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::BSwap::request_quote(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/bswap/quote";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Swap quoteAsset for baseAsset
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::BSwap::make_swap(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/bswap/swap";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get swap history
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::BSwap::get_swap_history(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/bswap/swap";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Get Pool Configure
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::BSwap::get_pool_configure(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/bswap/poolConfigure";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Add Liquidity Preview
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::BSwap::add_liquidity_preview(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/bswap/addLiquidityPreview";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Remove Liquidity Preview
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::BSwap::remove_liquidity_preview(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/bswap/removeLiquidityPreview";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

//  ------------------------------ End | Client BSwap - User Mining Endpoints


//  ------------------------------ Start | Client Fiat - User Mining Endpoints


/**
	A constructor - called directly by the user
	@param client_obj - the exchange client object
*/
template <typename T>
Client<T>::Fiat::Fiat(Client<T>& client_obj)
	: user_client{ &client_obj }
{
	if (user_client->_public_client)
	{
		MissingCredentials e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	};
}

/**
	A constructor - called directly by the user
	@param client_obj - the exchange client object (constant)
*/
template <typename T>
Client<T>::Fiat::Fiat(const Client<T>& client_obj)
	: user_client{ &client_obj } // snatching pointer and releasing later on to avoid deleting this reference
{
	if (user_client->_public_client)
	{
		MissingCredentials e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	};
}

/**
	Destructor
	set 'user_client' as nullptr to avoid deleting the exchange client
	object passed from outside the class (reference)
*/
template <typename T>
Client<T>::Fiat::~Fiat()
{
	user_client = nullptr;
}

// ------ Endpoint methods

/**
	Get Fiat Deposit/Withdraw History
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::Fiat::get_fiat_deposit_withdrawal_history(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/fiat/order";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

//  ------------------------------ End | Client Fiat - User Mining Endpoints


//  ------------------------------ Start | Client C2C - User Mining Endpoints


/**
	A constructor - called directly by the user
	@param client_obj - the exchange client object
*/
template <typename T>
Client<T>::C2C::C2C(Client<T>& client_obj)
	: user_client{ &client_obj }
{
	if (user_client->_public_client)
	{
		MissingCredentials e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	};
}

/**
	A constructor - called directly by the user
	@param client_obj - the exchange client object (constant)
*/
template <typename T>
Client<T>::C2C::C2C(const Client<T>& client_obj)
	: user_client{ &client_obj } // snatching pointer and releasing later on to avoid deleting this reference
{
	if (user_client->_public_client)
	{
		MissingCredentials e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	};
}

/**
	Destructor
	set 'user_client' as nullptr to avoid deleting the exchange client
	object passed from outside the class (reference)
*/
template <typename T>
Client<T>::C2C::~C2C()
{
	user_client = nullptr;
}

// ------ Endpoint methods

/**
	Get C2C Trade History
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename T>
Json::Value Client<T>::C2C::get_c2c_trades_history(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/c2c/orderMatch/listUserOrderHistory";
		std::string query = user_client->_generate_query(params_ptr, 1);
		Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

//  ------------------------------ End | Client C2C - User Mining Endpoints


// =======================================================================================================


//  ------------------------------ Start | SpotClient General methods - Infrastructure

/**
	A constructor - called directly by the user
	Public client
*/
SpotClient::SpotClient() : Client(*this)
{};

/**
	A constructor - called directly by the user
	Private client
	@param key - API key
	@param secret - API secret
*/
SpotClient::SpotClient(const std::string key, const std::string secret)
	: Client(*this, key, secret)
{}

/**
	Destructor
	delete Websocket and REST sessions
*/
SpotClient::~SpotClient() 
{
	delete this->_ws_client;
	delete this->_rest_client;
};

//  ------------------------------ End | SpotClient General methods - Infrastructure

//  ------------------------------ Start | SpotClient CRTP methods - Client infrastructure

/**
	Initialize a Websocket session
*/
void SpotClient::v_init_ws_session()
{

	this->_ws_client->set_host_port(_WS_BASE_SPOT, _WS_PORT_SPOT);
}

/**
	Create a ListenKey
	@return an std::string representing the ListenKey
*/
std::string SpotClient::v_get_listen_key()
{
	std::string full_path = _BASE_REST_SPOT + "/api/v3/userDataStream";
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response["response"]["listenKey"].asString();
}

/**
	Ping/Keep-alive a ListenKey
	@param listen_key - the listen_key
	@return json returned by the request
*/
Json::Value SpotClient::v_ping_listen_key(const std::string& listen_key)
{
	std::string full_path = _BASE_REST_SPOT + "/api/v3/userDataStream" + "?listenKey=" + listen_key;
	Json::Value response = listen_key.empty() ? (this->_rest_client)->_putreq(full_path) : (this->_rest_client)->_postreq(full_path);

	return response;
}

/**
	Revoke a ListenKey
	@param listen_key - the listen_key
	@return json returned by the request
*/
Json::Value SpotClient::v_revoke_listen_key(const std::string& listen_key)
{
	std::string full_path = _BASE_REST_SPOT + "/api/v3/userDataStream" + "?listenKey=" + listen_key;
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response;
}

//  ------------------------------ End | SpotClient CRTP methods - Client infrastructure

//  ------------------------------ Start | SpotClient CRTP methods - Market Data Implementations

/**
	CRTP of ping_client()
*/
bool SpotClient::v_ping_client()
{
	std::string full_path = _BASE_REST_SPOT + "/api/v3/ping";
	Json::Value ping_response = (this->_rest_client)->_getreq(full_path)["response"];
	return (ping_response != Json::nullValue);
}

/**
	CRTP of exchange_time()
*/
unsigned long long SpotClient::v_exchange_time()
{
	std::string full_path = _BASE_REST_SPOT + "/api/v3/time";
	std::string ex_time = (this->_rest_client)->_getreq(full_path)["response"]["serverTime"].asString();

	return std::atoll(ex_time.c_str());
}

/**
	CRTP of exchange_info()
*/
Json::Value SpotClient::v_exchange_info() 
{
	std::string full_path = _BASE_REST_SPOT + "/api/v1/exchangeInfo";
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of order_book()
*/
Json::Value SpotClient::v_order_book(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = _BASE_REST_SPOT + "/api/v3/depth" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of public_trades_recent()
*/
Json::Value SpotClient::v_public_trades_recent(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = _BASE_REST_SPOT + "/api/v3/trades" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of public_trades_historical()
*/
Json::Value SpotClient::v_public_trades_historical(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = _BASE_REST_SPOT + "/api/v3/historicalTrades" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of public_trades_agg()
*/
Json::Value SpotClient::v_public_trades_agg(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = _BASE_REST_SPOT + "/api/v3/aggTrades" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of klines()
*/
Json::Value SpotClient::v_klines(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = _BASE_REST_SPOT + "/api/v3/klines" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of daily_ticker_stats()
*/
Json::Value SpotClient::v_daily_ticker_stats(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = _BASE_REST_SPOT + "/api/v3/ticker/24hr" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of get_ticker()
*/
Json::Value SpotClient::v_get_ticker(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = _BASE_REST_SPOT + "/api/v3/ticker/price" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of get_order_book_ticker()
*/
Json::Value SpotClient::v_get_order_book_ticker(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = _BASE_REST_SPOT + "/api/v3/ticker/bookTicker" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}


//  ------------------------------ End | SpotClient CRTP methods - Market Data Implementations


//  ------------------------------ Start | SpotClient CRTP methods - Trade Implementations


// -- Up to 'Client' Level

/**
	CRTP of test_new_order()
*/
Json::Value SpotClient::v_test_new_order(const Params* params_ptr)
{
	std::string full_path = _BASE_REST_SPOT + "/api/v3/order/test";
	std::string query = this->_generate_query(params_ptr, 1);
	Json::Value response = (this->_rest_client)->_postreq(full_path + query);

	return response;
}

/**
	CRTP of new_order()
*/
Json::Value SpotClient::v_new_order(const Params* params_ptr)
{
	std::string full_path = _BASE_REST_SPOT + "/api/v3/order";
	std::string query = this->_generate_query(params_ptr, 1);
	Json::Value response = (this->_rest_client)->_postreq(full_path + query);

	return response;
}

/**
	CRTP of modify_order()
*/
Json::Value SpotClient::v_modify_order(const Params* params_ptr)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}

/**
	CRTP of cancel_order()
*/
Json::Value SpotClient::v_cancel_order(const Params* params_ptr)
{
	std::string full_path = _BASE_REST_SPOT + "/api/v3/order";
	std::string query = this->_generate_query(params_ptr, 1);
	Json::Value response = (this->_rest_client)->_deletereq(full_path + query);

	return response;
}

/**
	CRTP of cancel_all_orders()
*/
Json::Value SpotClient::v_cancel_all_orders(const Params* params_ptr)
{
	std::string full_path = _BASE_REST_SPOT + "api/v3/openOrders";
	std::string query = this->_generate_query(params_ptr, 1);
	Json::Value response = (this->_rest_client)->_deletereq(full_path + query);

	return response;
}

/**
	CRTP of query_order()
*/
Json::Value SpotClient::v_query_order(const Params* params_ptr)
{
	std::string full_path = _BASE_REST_SPOT + "/api/v3/order";
	std::string query = this->_generate_query(params_ptr, 1);
	Json::Value response = (this->_rest_client)->_getreq(full_path + query);

	return response;
}

/**
	CRTP of open_orders()
*/
Json::Value SpotClient::v_open_orders(const Params* params_ptr)
{
	std::string full_path = _BASE_REST_SPOT + "/api/v3/openOrders";
	std::string query = this->_generate_query(params_ptr, 1);
	Json::Value response = (this->_rest_client)->_getreq(full_path + query);

	return response;
}

/**
	CRTP of all_orders()
*/
Json::Value SpotClient::v_all_orders(const Params* params_ptr)
{
	std::string full_path = _BASE_REST_SPOT + "/api/v3/allOrders";
	std::string query = this->_generate_query(params_ptr, 1);
	Json::Value response = (this->_rest_client)->_getreq(full_path + query);

	return response;
}

/**
	CRTP of account_info()
*/
Json::Value SpotClient::v_account_info(const Params* params_ptr)
{
	std::string full_path = _BASE_REST_SPOT + "/api/v3/account";
	std::string query = this->_generate_query(params_ptr, 1);
	Json::Value response = (this->_rest_client)->_getreq(full_path + query);

	return response;
}

/**
	CRTP of account_trades_list()
*/
Json::Value SpotClient::v_account_trades_list(const Params* params_ptr)
{
	std::string full_path = _BASE_REST_SPOT + "/api/v3/myTrades";
	std::string query = this->_generate_query(params_ptr, 1);
	Json::Value response = (this->_rest_client)->_getreq(full_path + query);

	return response;
}

//  ------------------------------ End | SpotClient CRTP methods - Trade Implementations

//  ------------------------------ Start | SpotClient General methods - Trade Implementations 

/**
	Send in a new OCO
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
Json::Value SpotClient::oco_new_order(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/api/v3/order/oco";
		std::string query = this->_generate_query(params_ptr, 1);
		Json::Value response = (this->_rest_client)->_postreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}

}

/**
	Cancel OCO
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
Json::Value SpotClient::oco_cancel_order(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/api/v3/orderList";
		std::string query = this->_generate_query(params_ptr, 1);
		Json::Value response = (this->_rest_client)->_deletereq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Query OCO - Retrieves a specific OCO based on provided optional parameters
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
Json::Value SpotClient::oco_query_order(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/api/v3/orderList";
		std::string query = this->_generate_query(params_ptr, 1);
		Json::Value response = (this->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Query all OCO - Retrieves all OCO based on provided optional parameters
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
Json::Value SpotClient::oco_all_orders(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/api/v3/allOrderList";
		std::string query = this->_generate_query(params_ptr, 1);
		Json::Value response = (this->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Query Open OCO 
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
Json::Value SpotClient::oco_open_orders(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/api/v3/openOrderList";
		std::string query = this->_generate_query(params_ptr, 1);
		Json::Value response = (this->_rest_client)->_getreq(full_path + query);

		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

//  ------------------------------ End | SpotClient General methods - Trade Implementations 


//  ------------------------------ Start | SpotClient General methods - WS Streams



//  ------------------------------ End | SpotClient General methods - WS Streams


// =======================================================================================================


//  ------------------------------ Start | FuturesClient General methods - Infrastructure

/**
	A constructor - called directly by the user
	Public client - testnet disabled by default
*/
template <typename CT>
FuturesClient<CT>::FuturesClient(CT& exchange_client)
	: Client<FuturesClient<CT>>(*this), _testnet_mode{ 0 }
{};

/**
	A constructor - called directly by the user
	Private client - testnet disabled by default
	@param key - API key
	@param secret - API secret
*/
template <typename CT>
FuturesClient<CT>::FuturesClient(CT& exchange_client, std::string key, std::string secret)
	: Client<FuturesClient<CT>>(*this, key, secret), _testnet_mode{ 0 }
{}

/**
	Destructor
*/
template <typename CT>
FuturesClient<CT>::~FuturesClient()
{}


//  ------------------------------ End | FuturesClient General methods - Infrastructure


//  ------------------------------ Start | FuturesClient CRTP methods - Client infrastructure


/**
	CRTP of init_ws_session()
*/
template <typename CT>
void FuturesClient<CT>::v_init_ws_session() { static_cast<CT*>(this)->v__init_ws_session(); }

//  ------------------------------ End | FuturesClient CRTP methods - Client infrastructure

//  ------------------------------ Start | FuturesClient CRTP methods - Market Data Implementations

/**
	CRTP of exchange_time()
*/
template<typename CT>
unsigned long long FuturesClient<CT>::v_exchange_time() { return static_cast<CT*>(this)->v__exchange_time(); }

/**
	CRTP of ping_client()
*/
template<typename CT>
bool FuturesClient<CT>::v_ping_client() { return static_cast<CT*>(this)->v__ping_client(); }

/**
	CRTP of exchange_info()
*/
template<typename CT>
Json::Value FuturesClient<CT>::v_exchange_info() { return static_cast<CT*>(this)->v__exchange_info(); }

/**
	CRTP of order_book()
*/
template<typename CT>
Json::Value FuturesClient<CT>::v_order_book(const Params* params_ptr) { return static_cast<CT*>(this)->v__order_book(params_ptr); }

/**
	CRTP of trades_recent()
*/
template<typename CT>
Json::Value FuturesClient<CT>::v_public_trades_recent(const Params* params_ptr) { return static_cast<CT*>(this)->v__public_trades_recent(params_ptr); }

/**
	CRTP of public_trades_historical()
*/
template<typename CT>
Json::Value FuturesClient<CT>::v_public_trades_historical(const Params* params_ptr) { return static_cast<CT*>(this)->v__public_trades_historical(params_ptr); }

/**
	CRTP of public_trades_agg()
*/
template<typename CT>
Json::Value FuturesClient<CT>::v_public_trades_agg(const Params* params_ptr) { return static_cast<CT*>(this)->v__public_trades_agg(params_ptr); }

/**
	CRTP of klines()
*/
template<typename CT>
Json::Value FuturesClient<CT>::v_klines(const Params* params_ptr) { return static_cast<CT*>(this)->v__klines(params_ptr); }

/**
	CRTP of daily_ticker_stats()
*/
template<typename CT>
Json::Value FuturesClient<CT>::v_daily_ticker_stats(const Params* params_ptr) { return static_cast<CT*>(this)->v__daily_ticker_stats(params_ptr); }

/**
	CRTP of get_ticker()
*/
template<typename CT>
Json::Value FuturesClient<CT>::v_get_ticker(const Params* params_ptr) { return static_cast<CT*>(this)->v__get_ticker(params_ptr); }

/**
	CRTP of get_order_book_ticker()
*/
template<typename CT>
Json::Value FuturesClient<CT>::v_get_order_book_ticker(const Params* params_ptr) { return static_cast<CT*>(this)->v__get_order_book_ticker(params_ptr); }

//  ------------------------------ End | FuturesClient CRTP methods - Market Data Implementations


//  ------------------------------ Start | FuturesClient CRTP methods - Unique Endpoints

/**
	Get Mark Price - Mark Price and Funding Rate
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template<typename CT>
Json::Value FuturesClient<CT>::mark_price(const Params* params_ptr) 
{ 
	try
	{
		return static_cast<CT*>(this)->v_mark_price(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Open Interest - Get present open interest of a specific symbol
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template<typename CT>
Json::Value FuturesClient<CT>::open_interest(const Params* params_ptr) 
{
	try
	{
		return static_cast<CT*>(this)->v_open_interest(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Composite Index Symbol Information
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template<typename CT>
Json::Value FuturesClient<CT>::composite_index_symbol_info(const Params* params_ptr)
{
	try
	{
		return static_cast<CT*>(this)->v_open_interest(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Continues Contract Kline/Candlestick Data
	*If startTime and endTime are not sent, the most recent klines are returned
	* 
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template<typename CT>
Json::Value FuturesClient<CT>::continues_klines(const Params* params_ptr) 
{
	try
	{
		return static_cast<CT*>(this)->v_continues_klines(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get Index Klines
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template<typename CT>
Json::Value FuturesClient<CT>::index_klines(const Params* params_ptr)
{
	try
	{
		return static_cast<CT*>(this)->v_index_klines(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get Mark Klines
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template<typename CT>
Json::Value FuturesClient<CT>::mark_klines(const Params* params_ptr)
{
	try
	{
		return static_cast<CT*>(this)->v_get_ticker(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get Funding Rate History
	*If startTime and endTime are not sent, the most recent limit datas are returned.
	*If the number of data between startTime and endTime is larger than limit, return as startTime + limit.

	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template<typename CT>
Json::Value FuturesClient<CT>::funding_rate_history(const Params* params_ptr)
{
	try
	{
		return static_cast<CT*>(this)->v_funding_rate_history(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Change Multi-Assets Mode

	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template<typename CT>
Json::Value FuturesClient<CT>::change_multiasset_margin_mode(const Params* params_ptr)
{
	try
	{
		return static_cast<CT*>(this)->v_change_multiasset_margin_mode(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Check Multi-Assets Mode

	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template<typename CT>
Json::Value FuturesClient<CT>::check_multiasset_margin_mode(const Params* params_ptr)
{
	try
	{
		return static_cast<CT*>(this)->v_change_multiasset_margin_mode(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

//  ------------------------------ End | FuturesClient CRTP methods - Unique Endpoints


//  ------------------------------ Start | FuturesClient CRTP methods - Trade Implementations 

// -- Up to 'Client' Level

/**
	CRTP of test_new_order()
*/
template<typename CT>
Json::Value FuturesClient<CT>::v_test_new_order(const Params* params_ptr)
{ 
	std::string error_msg{ "use_tesnet_instead" };
	CustomException e(error_msg);
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}

/**
	CRTP of new_order()
*/
template<typename CT>
Json::Value FuturesClient<CT>::v_new_order(const Params* params_ptr) { return static_cast<CT*>(this)->v__new_order(params_ptr); }

/**
	CRTP of modify_order()
*/
template<typename CT>
Json::Value FuturesClient<CT>::v_modify_order(const Params* params_ptr) { return static_cast<CT*>(this)->v__modify_order(params_ptr); }

/**
	CRTP of cancel_order()
*/
template<typename CT>
Json::Value FuturesClient<CT>::v_cancel_order(const Params* params_ptr) { return static_cast<CT*>(this)->v__cancel_order(params_ptr); }

/**
	CRTP of cancel_all_orders()
*/
template<typename CT>
Json::Value FuturesClient<CT>::v_cancel_all_orders(const Params* params_ptr) { return static_cast<CT*>(this)->v__cancel_all_orders(params_ptr); }

/**
	CRTP of query_order()
*/
template<typename CT>
Json::Value FuturesClient<CT>::v_query_order(const Params* params_ptr) { return static_cast<CT*>(this)->v__query_order(params_ptr); }

/**
	CRTP of open_orders()
*/
template<typename CT>
Json::Value FuturesClient<CT>::v_open_orders(const Params* params_ptr) { return static_cast<CT*>(this)->v__open_orders(params_ptr); }

/**
	CRTP of all_orders()
*/
template<typename CT>
Json::Value FuturesClient<CT>::v_all_orders(const Params* params_ptr) { return static_cast<CT*>(this)->v__all_orders(params_ptr); }

/**
	CRTP of account_info()
*/
template<typename CT>
Json::Value FuturesClient<CT>::v_account_info(const Params* params_ptr) { return static_cast<CT*>(this)->v__account_info(params_ptr); }

/**
	CRTP of account_trades_list()
*/
template<typename CT>
Json::Value FuturesClient<CT>::v_account_trades_list(const Params* params_ptr) { return static_cast<CT*>(this)->v__account_trades_list(params_ptr); }

// -- Up to 'FuturesClient' (this) Level

/**
	Change Position Mode - Change user's position mode (Hedge Mode or One-way Mode ) on EVERY symbol
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template<typename CT>
Json::Value FuturesClient<CT>::change_position_mode(const Params* params_ptr) 
{
	try
	{
		return static_cast<CT*>(this)->v_change_position_mode(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get Position Mode - Get user's position mode (Hedge Mode or One-way Mode ) on EVERY symbol
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template<typename CT>
Json::Value FuturesClient<CT>::get_position_mode(const Params* params_ptr)
{
	try
	{
		return static_cast<CT*>(this)->v_get_position_mode(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Place Multiple Orders
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template<typename CT>
Json::Value FuturesClient<CT>::batch_orders(const Params* params_ptr) 
{
	try
	{
		return static_cast<CT*>(this)->v_batch_orders(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Modify Multiple Order
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template<typename CT>
Json::Value FuturesClient<CT>::modify_batch_orders(const Params* params_ptr)
{
	try
	{
		return static_cast<CT*>(this)->v_modify_batch_orders(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}


/**
	Get Order Modify History
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template<typename CT>
Json::Value FuturesClient<CT>::modify_batch_orders_history(const Params* params_ptr)
{
	try
	{
		return static_cast<CT*>(this)->v_modify_batch_orders_history(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Cancel Multiple Orders - Cancel all open orders of the specified symbol at the end of the specified countdown.
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template<typename CT>
Json::Value FuturesClient<CT>::cancel_batch_orders(const Params* params_ptr)
{
	try
	{
		return static_cast<CT*>(this)->v_cancel_batch_orders(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Auto-Cancel All Open Orders - Cancel all open orders of the specified symbol at the end of the specified countdown.
	*This rest endpoint means to ensure your open orders are canceled in case of an outage.
	 The endpoint should be called repeatedly as heartbeats so that the existing countdown time can be canceled and replaced by a new one.

	*Example usage:
	 Call this endpoint at 30s intervals with an countdownTime of 120000 (120s).
	 If this endpoint is not called within 120 seconds, all your orders of the specified symbol will be automatically canceled.
	 If this endpoint is called with an countdownTime of 0, the countdown timer will be stopped.

	*The system will check all countdowns approximately every 10 milliseconds, so please note that sufficient redundancy should be considered when using this function.
	 We do not recommend setting the countdown time to be too precise or too small.

	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template<typename CT>
Json::Value FuturesClient<CT>::cancel_all_orders_timer(const Params* params_ptr)
{
	try
	{
		return static_cast<CT*>(this)->v_cancel_all_orders_timer(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Query an open order - Check an order's status
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template<typename CT>
Json::Value FuturesClient<CT>::query_open_order(const Params* params_ptr)
{
	try
	{
		return static_cast<CT*>(this)->v_query_open_order(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get Futures Account Balance
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template<typename CT>
Json::Value FuturesClient<CT>::account_balances(const Params* params_ptr)
{
	try
	{
		return static_cast<CT*>(this)->v_account_balances(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
	}

/**
	Change Initial Leverage - Change user's initial leverage of specific symbol market
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template<typename CT>
Json::Value FuturesClient<CT>::change_leverage(const Params* params_ptr) 
{
	try
	{
		return static_cast<CT*>(this)->v_change_leverage(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Change Margin Type
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template<typename CT>
Json::Value FuturesClient<CT>::change_margin_type(const Params* params_ptr)
{
	try
	{
		return static_cast<CT*>(this)->v_change_margin_type(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Modify Isolated Position Margin
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template<typename CT>
Json::Value FuturesClient<CT>::change_position_margin(const Params* params_ptr) 
{
	try
	{
		return static_cast<CT*>(this)->v_change_position_margin(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get Position Margin Change History
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template<typename CT>
Json::Value FuturesClient<CT>::change_position_margin_history(const Params* params_ptr) 
{
	try
	{
		return static_cast<CT*>(this)->v_change_position_margin_history(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get Position Information V2
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template<typename CT>
Json::Value FuturesClient<CT>::position_info(const Params* params_ptr)
{
	try
	{
		return static_cast<CT*>(this)->v_position_info(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get Income History
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template<typename CT>
Json::Value FuturesClient<CT>::get_income_history(const Params* params_ptr)
{
	try
	{
		return static_cast<CT*>(this)->v_get_income_history(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get Notional and Leverage Brackets
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template<typename CT>
Json::Value FuturesClient<CT>::get_leverage_bracket(const Params* params_ptr) 
{
	try
	{
		return static_cast<CT*>(this)->v_get_leverage_bracket(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	User's Force Orders (USER_DATA)
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template<typename CT>
Json::Value FuturesClient<CT>::users_force_orders(const Params* params_ptr)
{
	try
	{
		return static_cast<CT*>(this)->v_users_force_orders(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Position ADL Quantile Estimation
	*Values update every 30s.
	*Values 0, 1, 2, 3, 4 shows the queue position and possibility of ADL from low to high.
	*For positions of the symbol are in One-way Mode or isolated margined in Hedge Mode, "LONG", "SHORT", and "BOTH"
	 will be returned to show the positions' adl quantiles of different position sides.
	*If the positions of the symbol are crossed margined in Hedge Mode:
		**"HEDGE" as a sign will be returned instead of "BOTH";
		**A same value caculated on unrealized pnls on long and short sides' positions will be shown 
		  for "LONG" and "SHORT" when there are positions in both of long and short sides.
	
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template<typename CT>
Json::Value FuturesClient<CT>::pos_adl_quantile_est(const Params* params_ptr)
{
	try
	{
		return static_cast<CT*>(this)->v_pos_adl_quantile_est(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	User Commission Rate
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template<typename CT>
Json::Value FuturesClient<CT>::get_user_comission_rate(const Params* params_ptr)
{
	try
	{
		return static_cast<CT*>(this)->v_get_user_comission_rate(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	User API Trading Quantitative Rules Indicators
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template<typename CT>
Json::Value FuturesClient<CT>::api_quant_trading_rules(const Params* params_ptr)
{
	try
	{
		return static_cast<CT*>(this)->v_api_quant_trading_rules(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Historical BLVT NAV Kline/Candlestick
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template<typename CT>
Json::Value FuturesClient<CT>::get_blvt_kline_history(const Params* params_ptr)
{
	try
	{
		return static_cast<CT*>(this)->v_get_blvt_kline_history(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Composite Index Symbol Information
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template<typename CT>
Json::Value FuturesClient<CT>::get_composite_index_symbol_info(const Params* params_ptr)
{
	try
	{
		return static_cast<CT*>(this)->v_get_composite_index_symbol_info(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

//  ------------------------------ End | FuturesClient CRTP methods - Trade Implementations


//  ------------------------------ Start | FuturesClient Global + CRTP methods - WS Streams 

/**
	CRTP of get_listen_key()
*/
template <typename CT>
std::string FuturesClient<CT>::v_get_listen_key() { return static_cast<CT*>(this)->v__get_listen_key(); }

/**
	CRTP of ping_listen_key()
*/
template <typename CT>
Json::Value FuturesClient<CT>::v_ping_listen_key(const std::string& listen_key) { return static_cast<CT*>(this)->v__ping_listen_key(); }

/**
	CRTP of revoke_listen_key()
*/
template <typename CT>
Json::Value FuturesClient<CT>::v_revoke_listen_key(const std::string& listen_key) { return static_cast<CT*>(this)->v__revoke_listen_key(); }

//  ------------------------------ End | FuturesClient Global + CRTP methods - WS Streams 


//  ------------------------------ Start | FuturesClient General methods - Markets Stats

/**
	Get Open Interest Statistics
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename CT>
Json::Value FuturesClient<CT>::open_interest_stats(const Params* params_ptr)
{
	try
	{
		std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
		std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
		full_path += "/futures/data/openInterestHist" + query;
		Json::Value response = (this->_rest_client)->_getreq(full_path);
		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get Top Trader Long/Short Ratio (Accounts / Positions)
	@param params_ptr - a pointer to the request Params object
	@param accounts - true to 'Accounts', false for 'Positions'
	@return json returned by the request
*/
template <typename CT>
Json::Value FuturesClient<CT>::top_long_short_ratio(const Params* params_ptr, bool accounts)
{
	try
	{
		std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
		std::string endpoint = accounts ? "/futures/data/topLongShortAccountRatio" : "/futures/data/topLongShortPositionRatio";
		std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
		full_path += (endpoint + query);
		Json::Value response = (this->_rest_client)->_getreq(full_path);
		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get Long/Short Ratio
	*If startTime and endTime are not sent, the most recent data is returned.
	*Only the data of the latest 30 days is available.
	
	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename CT>
Json::Value FuturesClient<CT>::global_long_short_ratio(const Params* params_ptr)
{
	try
	{
		std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
		std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
		full_path += ("/futures/data/globalLongShortAccountRatio" + query);
		Json::Value response = (this->_rest_client)->_getreq(full_path);
		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Get Taker Buy/Sell Volume
	*If startTime and endTime are not sent, the most recent data is returned.
	*Only the data of the latest 30 days is available.

	@param params_ptr - a pointer to the request Params object
	@return json returned by the request
*/
template <typename CT>
Json::Value FuturesClient<CT>::taker_long_short_ratio(const Params* params_ptr)
{
	try
	{
		std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
		std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
		full_path += ("/futures/data/takerlongshortRatio" + query);
		Json::Value response = (this->_rest_client)->_getreq(full_path);
		return response;
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}


//  ------------------------------ End | FuturesClient General methods - Markets Stats


// =======================================================================================================


//  ------------------------------ Start | FuturesClientUSDT General methods - Infrastructure


/**
	A constructor - called directly by the user
	Public client
*/
FuturesClientUSDT::FuturesClientUSDT()
	: FuturesClient(*this)
{};

/**
	A constructor - called directly by the user
	Private client
	@param key - API key
	@param secret - API secret
*/
FuturesClientUSDT::FuturesClientUSDT(const std::string key, const std::string secret)
	: FuturesClient(*this, key, secret)
{}

/**
	Destructor
	Delete Websocket and REST sessions
*/
FuturesClientUSDT::~FuturesClientUSDT()
{
	delete this->_ws_client;
	delete this->_rest_client;
}

/**
	CRTP of v_init_ws_session()
*/
void FuturesClientUSDT::v__init_ws_session()
{
	this->_ws_client->set_host_port(_WS_BASE_FUTURES_USDT, _WS_PORT_FUTURES);
}

/**
	CRTP of set_testnet_mode()
*/
void FuturesClientUSDT::v_set_testnet_mode(const bool status)
{
	if (status) this->_ws_client->set_host_port(_WS_BASE_FUTURES_USDT_TESTNET, _WS_PORT_FUTURES);
	else this->_ws_client->set_host_port(_WS_BASE_FUTURES_USDT, _WS_PORT_FUTURES);
	this->_testnet_mode = status;

}

//  ------------------------------ Start | FuturesClientUSDT CRTP methods - Market Data Implementations

/**
	CRTP of v_ping_client()
*/
bool FuturesClientUSDT::v__ping_client()
{
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += "/fapi/v1/ping";
	Json::Value ping_response = (this->_rest_client)->_getreq(full_path)["response"];
	return (ping_response != Json::nullValue);
}

/**
	CRTP of v_exchange_time()
*/
unsigned long long FuturesClientUSDT::v__exchange_time()
{
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += "/fapi/v1/time";
	std::string ex_time = (this->_rest_client)->_getreq(full_path)["response"]["serverTime"].asString();

	return std::atoll(ex_time.c_str());
}

/**
	CRTP of v_exchange_info()
*/
Json::Value FuturesClientUSDT::v__exchange_info()
{
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += "/fapi/v1/exchangeInfo";
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of v_order_book()
*/
Json::Value FuturesClientUSDT::v__order_book(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/depth" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of v_public_trades_recent()
*/
Json::Value FuturesClientUSDT::v__public_trades_recent(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/trades" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of v_public_trades_historical()
*/
Json::Value FuturesClientUSDT::v__public_trades_historical(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/historicalTrades" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of v_public_trades_agg()
*/
Json::Value FuturesClientUSDT::v__public_trades_agg(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/aggTrades" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of v_klines()
*/
Json::Value FuturesClientUSDT::v__klines(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/klines" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of v_daily_ticker_stats()
*/
Json::Value FuturesClientUSDT::v__daily_ticker_stats(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/ticker/24hr" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of v_get_ticker()
*/
Json::Value FuturesClientUSDT::v__get_ticker(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/ticker/price" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of v_get_order_book_ticker()
*/
Json::Value FuturesClientUSDT::v__get_order_book_ticker(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/ticker/bookTicker" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

//  ------------------------------ End | FuturesClientUSDT CRTP methods - Market Data Implementations


//  ------------------------------ Start | FuturesClientUSDT CRTP methods - Unique Endpoints

/**
	CRTP of mark_price()
*/
Json::Value FuturesClientUSDT::v_mark_price(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/premiumIndex" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of open_interest()
*/
Json::Value FuturesClientUSDT::v_open_interest(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/openInterest" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of composite_index_symbol_info()
*/
Json::Value FuturesClientUSDT::v_composite_index_symbol_info(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/indexInfo" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

// ~~~ Do not exist for this client

/**
	CRTP of continues_klines()
*/
Json::Value FuturesClientUSDT::v_continues_klines(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/continuousKlines" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of index_klines()
*/
Json::Value FuturesClientUSDT::v_index_klines(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/indexPriceKlines" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of mark_klines()
*/
Json::Value FuturesClientUSDT::v_mark_klines(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/markPriceKlines" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}


// ~~~ Unique for this Client 

/**
	CRTP of funding_rate_history() - Unique for USDT margined client
*/
Json::Value FuturesClientUSDT::v_funding_rate_history(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/fundingRate" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of change_multiasset_margin_mode() - Unique for USDT margined client
*/
Json::Value FuturesClientUSDT::v_change_multiasset_margin_mode(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/multiAssetsMargin" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path);
	return response;
}

/**
	CRTP of check_multiasset_margin_mode() - Unique for USDT margined client
*/
Json::Value FuturesClientUSDT::v_check_multiasset_margin_mode(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/multiAssetsMargin" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

//  ------------------------------ End | FuturesClientUSDT CRTP methods - Unique Endpoints

//  ------------------------------ Start | FuturesClientUSDT CRTP methods - Trade Implementations 


// -- Up to 'Client' Level

/**
	CRTP of v_new_order()
*/
Json::Value FuturesClientUSDT::v__new_order(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/order" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response;
}

/**
	CRTP of v_modify_order()
*/
Json::Value FuturesClientUSDT::v__modify_order(const Params* params_ptr)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}

/**
	CRTP of v_cancel_order()
*/
Json::Value FuturesClientUSDT::v__cancel_order(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += "/fapi/v1/order";
	Json::Value response = (this->_rest_client)->_deletereq(full_path);

	return response;
}

/**
	CRTP of v_cancel_all_orders()
*/
Json::Value FuturesClientUSDT::v__cancel_all_orders(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += "/fapi/v1/allOpenOrders";
	Json::Value response = (this->_rest_client)->_deletereq(full_path);

	return response;
}

/**
	CRTP of v_query_order()
*/
Json::Value FuturesClientUSDT::v__query_order(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/order" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

/**
	CRTP of v_open_orders()
*/
Json::Value FuturesClientUSDT::v__open_orders(const Params* params_ptr)
{

	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/openOrders" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

/**
	CRTP of v_all_orders()
*/
Json::Value FuturesClientUSDT::v__all_orders(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/allOrders" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

/**
	CRTP of v_account_info()
*/
Json::Value FuturesClientUSDT::v__account_info(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v2/account" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

/**
	CRTP of v_account_trades_list()
*/
Json::Value FuturesClientUSDT::v__account_trades_list(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/userTrades" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

// -- Up to 'FuturesClient' (this) Level

/**
	CRTP of change_position_mode()
*/
Json::Value FuturesClientUSDT::v_change_position_mode(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/positionSide/dual" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path); // should be spot?

	return response;
}

/**
	CRTP of get_position_mode()
*/
Json::Value FuturesClientUSDT::v_get_position_mode(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/positionSide/dual" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path); // should be spot?

	return response;
}

/**
	CRTP of batch_orders()
*/
Json::Value FuturesClientUSDT::v_batch_orders(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/batchOrders" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path); // should be spot?

	return response;
}

/**
	CRTP of modify_batch_orders()
*/
Json::Value FuturesClientUSDT::v_modify_batch_orders(const Params* params_ptr)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}

/**
	CRTP of modify_batch_orders()
*/
Json::Value FuturesClientUSDT::v_modify_batch_orders_history(const Params* params_ptr)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}

/**
	CRTP of cancel_batch_orders()
*/
Json::Value FuturesClientUSDT::v_cancel_batch_orders(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/batchOrders" + query);
	Json::Value response = (this->_rest_client)->_deletereq(full_path); // should be spot?

	return response;
}

/**
	CRTP of cancel_all_orders_timer()
*/
Json::Value FuturesClientUSDT::v_cancel_all_orders_timer(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/countdownCancelAll" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path); // should be spot?

	return response;
}

/**
	CRTP of query_open_order()
*/
Json::Value FuturesClientUSDT::v_query_open_order(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/openOrder" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path); // should be spot?

	return response;
}

/**
	CRTP of account_balances()
*/
Json::Value FuturesClientUSDT::v_account_balances(const Params* params_ptr)
{

	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v2/balance" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path); // should be spot?

	return response;
}

/**
	CRTP of change_leverage()
*/
Json::Value FuturesClientUSDT::v_change_leverage(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/leverage" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path); // should be spot?

	return response;
}

/**
	CRTP of change_margin_type()
*/
Json::Value FuturesClientUSDT::v_change_margin_type(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/marginType" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path); // should be spot?

	return response;
}

/**
	CRTP of change_position_margin()
*/
Json::Value FuturesClientUSDT::v_change_position_margin(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/positionMargin" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path); // should be spot?

	return response;
}

/**
	CRTP of change_position_margin_history()
*/
Json::Value FuturesClientUSDT::v_change_position_margin_history(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/positionMargin/history" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path); // should be spot?

	return response;
}

/**
	CRTP of position_info()
*/
Json::Value FuturesClientUSDT::v_position_info(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v2/positionRisk" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

/**
	CRTP of get_income_history()
*/
Json::Value FuturesClientUSDT::v_get_income_history(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/income" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

/**
	CRTP of get_leverage_bracket()
*/
Json::Value FuturesClientUSDT::v_get_leverage_bracket(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/leverageBracket" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

/**
	CRTP of users_force_orders()
*/
Json::Value FuturesClientUSDT::v_users_force_orders(const Params* params_ptr)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}

/**
	CRTP of pos_adl_quantile_est()
*/
Json::Value FuturesClientUSDT::v_pos_adl_quantile_est(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/adlQuantile" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

/**
	CRTP of get_user_comission_rate() - Does not exist for Coin
*/
Json::Value FuturesClientUSDT::v_get_user_comission_rate(const Params* params_ptr)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);

}

/**
	CRTP of api_quant_trading_rules() - Does not exist for Coin
*/
Json::Value FuturesClientUSDT::v_api_quant_trading_rules(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 0);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/apiTradingStatus" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

/**
	CRTP of get_blvt_kline_history() - Does not exist for Coin
*/
Json::Value FuturesClientUSDT::v_get_blvt_kline_history(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 0);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/lvtKlines" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

/**
	CRTP of get_composite_index_symbol_info() - Does not exist for Coin
*/
Json::Value FuturesClientUSDT::v_get_composite_index_symbol_info(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 0);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/indexInfo" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

//  ------------------------------ End | FuturesClientUSDT CRTP methods - Trade Implementations 

//  ------------------------------ Start | FuturesClientUSDT CRTP methods - WS Streams

/**
	CRTP of v_get_listen_key()
*/
std::string FuturesClientUSDT::v__get_listen_key()
{
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += "/fapi/v1/listenKey";
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response["response"]["listenKey"].asString();
}

/**
	CRTP of v_ping_listen_key()
*/
Json::Value FuturesClientUSDT::v__ping_listen_key()
{
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += "/fapi/v1/listenKey";

	Json::Value response = (this->_rest_client)->_putreq(full_path);

	return response;
}

/**
	CRTP of v_revoke_listen_key()
*/
Json::Value FuturesClientUSDT::v__revoke_listen_key()
{

	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += "/fapi/v1/listenKey";

	Json::Value response = (this->_rest_client)->_deletereq(full_path);

	return response;
}

//  ------------------------------ End | FuturesClientUSDT CRTP methods - WS Streams


// =======================================================================================================


//  ------------------------------ Start | FuturesClientCoin General methods - Infrastructure


/**
	A constructor - called directly by the user
	Public client
*/
FuturesClientCoin::FuturesClientCoin()
	: FuturesClient(*this)
{};

/**
	A constructor - called directly by the user
	Private client
	@param key - API key
	@param secret - API secret
*/
FuturesClientCoin::FuturesClientCoin(const std::string key, const std::string secret)
	: FuturesClient(*this, key, secret)
{}

/**
	Destructor
	Delete Websocket and REST sessions
*/
FuturesClientCoin::~FuturesClientCoin()
{
	delete this->_ws_client;
	delete this->_rest_client;
}

/**
	CRTP of v_init_ws_session()
*/
void FuturesClientCoin::v__init_ws_session()
{

	this->_ws_client->set_host_port(_WS_BASE_FUTURES_COIN, _WS_PORT_FUTURES);
}

/**
	CRTP of set_testnet_mode()
*/
void FuturesClientCoin::v_set_testnet_mode(const bool status)
{
	if (status) this->_ws_client->set_host_port(_WS_BASE_FUTURES_COIN_TESTNET, _WS_PORT_FUTURES);
	else this->_ws_client->set_host_port(_WS_BASE_FUTURES_COIN, _WS_PORT_FUTURES);
	this->_testnet_mode = status;
}

//  ------------------------------ End | FuturesClientCoin General methods - Infrastructure

//  ------------------------------ Start | FuturesClientCoin CRTP methods - Market Data Implementations

/**
	CRTP of v_ping_client()
*/
bool FuturesClientCoin::v__ping_client()
{
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += "/dapi/v1/ping";
	Json::Value ping_response = (this->_rest_client)->_getreq(full_path)["response"];
	return (ping_response != Json::nullValue);
}

/**
	CRTP of v_exchange_time()
*/
unsigned long long FuturesClientCoin::v__exchange_time()
{
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += "/dapi/v1/time";
	std::string ex_time = (this->_rest_client)->_getreq(full_path)["response"]["serverTime"].asString();

	return std::atoll(ex_time.c_str());
}

/**
	CRTP of v_exchange_info()
*/
Json::Value FuturesClientCoin::v__exchange_info()
{
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += "/dapi/v1/exchangeInfo";
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of v_order_book()
*/
Json::Value FuturesClientCoin::v__order_book(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/depth" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of v_public_trades_recent()
*/
Json::Value FuturesClientCoin::v__public_trades_recent(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/trades" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of v_public_trades_historical()
*/
Json::Value FuturesClientCoin::v__public_trades_historical(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/historicalTrades" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of v_public_trades_agg()
*/
Json::Value FuturesClientCoin::v__public_trades_agg(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/aggTrades" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of v_klines()
*/
Json::Value FuturesClientCoin::v__klines(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/klines" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of v_daily_ticker_stats()
*/
Json::Value FuturesClientCoin::v__daily_ticker_stats(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/ticker/24hr" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of v_get_ticker()
*/
Json::Value FuturesClientCoin::v__get_ticker(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/ticker/price" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of v_get_order_book_ticker()
*/
Json::Value FuturesClientCoin::v__get_order_book_ticker(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/ticker/bookTicker" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

//  ------------------------------ End | FuturesClientCoin CRTP methods - Market Data Implementations

//  ------------------------------ Start | FuturesClientUSDT CRTP methods - Trade Implementations 


// -- Up to 'Client' Level


/**
	CRTP of v_new_order()
*/
Json::Value FuturesClientCoin::v__new_order(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/order" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response;
}

/**
	CRTP of v_modify_order()
*/
Json::Value FuturesClientCoin::v__modify_order(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/order" + query);
	Json::Value response = (this->_rest_client)->_putreq(full_path);

	return response;
}

/**
	CRTP of v_cancel_order()
*/
Json::Value FuturesClientCoin::v__cancel_order(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/order" + query);
	Json::Value response = (this->_rest_client)->_deletereq(full_path);

	return response;
}

/**
	CRTP of v_cancel_all_orders()
*/
Json::Value FuturesClientCoin::v__cancel_all_orders(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/allOpenOrders" + query);
	Json::Value response = (this->_rest_client)->_deletereq(full_path);

	return response;
}

/**
	CRTP of v_query_order()
*/
Json::Value FuturesClientCoin::v__query_order(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/order" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

/**
	CRTP of v_open_orders()
*/
Json::Value FuturesClientCoin::v__open_orders(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/openOrders" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

/**
	CRTP of v_all_orders()
*/
Json::Value FuturesClientCoin::v__all_orders(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/allOrders" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

/**
	CRTP of v_account_info()
*/
Json::Value FuturesClientCoin::v__account_info(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/account" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

/**
	CRTP of v_account_trades_list()
*/
Json::Value FuturesClientCoin::v__account_trades_list(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/userTrades" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

// -- Up to 'FuturesClient' (this) Level


/**
	CRTP of change_position_mode()
*/
Json::Value FuturesClientCoin::v_change_position_mode(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/positionSide/dual" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response;
}

/**
	CRTP of get_position_mode()
*/
Json::Value FuturesClientCoin::v_get_position_mode(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/positionSide/dual" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

/**
	CRTP of batch_orders()
*/
Json::Value FuturesClientCoin::v_batch_orders(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/batchOrders" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response;
}

/**
	CRTP of modify_batch_orders()
*/
Json::Value FuturesClientCoin::v_modify_batch_orders(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/batchOrders" + query);
	Json::Value response = (this->_rest_client)->_putreq(full_path);

	return response;
}

/**
	CRTP of modify_batch_orders()
*/
Json::Value FuturesClientCoin::v_modify_batch_orders_history(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/orderAmendment" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

/**
	CRTP of cancel_batch_orders()
*/
Json::Value FuturesClientCoin::v_cancel_batch_orders(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/batchOrders" + query);
	Json::Value response = (this->_rest_client)->_deletereq(full_path);

	return response;
}

/**
	CRTP of cancel_all_orders_timer()
*/
Json::Value FuturesClientCoin::v_cancel_all_orders_timer(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/countdownCancelAll" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response;
}

/**
	CRTP of query_open_order()
*/
Json::Value FuturesClientCoin::v_query_open_order(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/openOrder" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

/**
	CRTP of account_balances()
*/
Json::Value FuturesClientCoin::v_account_balances(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/balance" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

/**
	CRTP of change_leverage()
*/
Json::Value FuturesClientCoin::v_change_leverage(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/leverage" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response;
}

/**
	CRTP of change_margin_type()
*/
Json::Value FuturesClientCoin::v_change_margin_type(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/marginType" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response;
}

/**
	CRTP of change_position_margin()
*/
Json::Value FuturesClientCoin::v_change_position_margin(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/positionMargin" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response;
}

/**
	CRTP of change_position_margin_history()
*/
Json::Value FuturesClientCoin::v_change_position_margin_history(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/positionMargin/history" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

/**
	CRTP of position_info()
*/
Json::Value FuturesClientCoin::v_position_info(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/positionRisk" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

/**
	CRTP of get_income_history()
*/
Json::Value FuturesClientCoin::v_get_income_history(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/income" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

/**
	CRTP of get_leverage_bracket()
*/
Json::Value FuturesClientCoin::v_get_leverage_bracket(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/leverageBracket" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

/**
	CRTP of users_force_orders()
*/
Json::Value FuturesClientCoin::v_users_force_orders(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/forceOrders" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

/**
	CRTP of pos_adl_quantile_est()
*/
Json::Value FuturesClientCoin::v_pos_adl_quantile_est(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/adlQuantile" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

/**
	CRTP of get_user_comission_rate()
*/
Json::Value FuturesClientCoin::v_get_user_comission_rate(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/commissionRate" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}


/**
	CRTP of api_quant_trading_rules() - Does not exist for Coin
*/
Json::Value FuturesClientCoin::v_api_quant_trading_rules(const Params* params_ptr)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}

/**
	CRTP of get_blvt_kline_history() - Does not exist for Coin
*/
Json::Value FuturesClientCoin::v_get_blvt_kline_history(const Params* params_ptr)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}

/**
	CRTP of get_composite_index_symbol_info() - Does not exist for Coin
*/
Json::Value FuturesClientCoin::v_get_composite_index_symbol_info(const Params* params_ptr)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}


//  ------------------------------ End | FuturesClientCoin CRTP methods - Trade Implementations 

//  ------------------------------ Start | FuturesClientCoin CRTP methods - WS Streams


/**
	CRTP of stream_markprice_all()
*/
template <typename FT>
unsigned int FuturesClientCoin::v_stream_markprice_all(const std::string& symbol, std::string& buffer, FT& functor) // here
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}

/**
	CRTP of v_get_listen_key()
*/
std::string FuturesClientCoin::v__get_listen_key()
{
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += "/dapi/v1/listenKey";
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response["response"]["listenKey"].asString();
}

/**
	CRTP of v_ping_listen_key()
*/
Json::Value FuturesClientCoin::v__ping_listen_key()
{
	// no signature is needed here
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += "/dapi/v1/listenKey";

	Json::Value response = (this->_rest_client)->_putreq(full_path);

	return response;
}

/**
	CRTP of v_revoke_listen_key()
*/
Json::Value FuturesClientCoin::v__revoke_listen_key()
{
	// no signature is needed here
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += "/dapi/v1/listenKey";

	Json::Value response = (this->_rest_client)->_deletereq(full_path);

	return response;
}

//  ------------------------------ End | FuturesClientCoin CRTP methods - WS Streams


//  ------------------------------ Start | FuturesClientCoin CRTP methods - Unique Endpoints

/**
	CRTP of mark_price()
*/
Json::Value FuturesClientCoin::v_mark_price(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/premiumIndex" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of open_interest()
*/
Json::Value FuturesClientCoin::v_open_interest(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/openInterest" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of composite_index_symbol_info() - Missing endpoint for Coin margined
*/
Json::Value FuturesClientCoin::v_composite_index_symbol_info(const Params* params_ptr)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}

// ~~~ Unique for this Client 


/**
	CRTP of continues_klines() - Unique for Coin Margined client
*/
Json::Value FuturesClientCoin::v_continues_klines(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/continuousKlines" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of index_klines() - Unique for Coin Margined client
*/
Json::Value FuturesClientCoin::v_index_klines(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/indexPriceKlines" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of mark_klines() - Unique for Coin Margined client
*/
Json::Value FuturesClientCoin::v_mark_klines(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/markPriceKlines" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}


// ~~~ Missing endpoints for this client


/**
	CRTP of funding_rate_history() - Missing endpoint for this client
*/
Json::Value FuturesClientCoin::v_funding_rate_history(const Params* params_ptr)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}

/**
	CRTP of change_multiasset_margin_mode() - Missing endpoint for this client
*/
Json::Value FuturesClientCoin::v_change_multiasset_margin_mode(const Params* params_ptr)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}

/**
	CRTP of check_multiasset_margin_mode() - Missing endpoint for this client
*/
Json::Value FuturesClientCoin::v_check_multiasset_margin_mode(const Params* params_ptr)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}

//  ------------------------------ End | FuturesClientCoin CRTP methods - Unique Endpoints


// =======================================================================================================

//  ------------------------------ Start | OpsClient General methods - Infrastructure

/**
	A constructor - called directly by the user
	Public client
*/
OpsClient::OpsClient() : Client(*this), _testnet_mode{ 0 }
{};

/**
	A constructor - called directly by the user
	Private client
	@param key - API key
	@param secret - API secret
*/
OpsClient::OpsClient(const std::string key, const std::string secret)
	: Client(*this, key, secret), _testnet_mode{ 0 }
{}

/**
	Destructor
	delete Websocket and REST sessions
*/
OpsClient::~OpsClient()
{
	delete this->_ws_client;
	delete this->_rest_client;
};

//  ------------------------------ End | OpsClient General methods - Infrastructure

//  ------------------------------ Start | OpsClient CRTP methods - Client infrastructure

/**
	Initialize a Websocket session
*/
void OpsClient::v_init_ws_session()
{
	this->_ws_client->set_host_port(_WS_BASE_OPS, _WS_PORT_OPS);
	this->_ws_client->set_gzip_conversion(1);
}

/**
	Enables / disables testnet mode
	@param status - a boolean for enabling / disabling
*/
void OpsClient::set_testnet_mode(const bool status)
{
	if (status) this->_ws_client->set_host_port(_WS_BASE_OPS_TESTNET, _WS_PORT_OPS);
	else this->_ws_client->set_host_port(_WS_BASE_OPS, _WS_PORT_OPS);
	this->_testnet_mode = status;
}

/**
	Get current testnet mode status
	@return 1 if enabled, 0 if disabled
*/
bool OpsClient::get_testnet_mode()
{
	return this->_testnet_mode;
}

/**
	Create a ListenKey
	@return an std::string representing the ListenKey
*/
std::string OpsClient::v_get_listen_key()
{
	std::string full_path = !this->_testnet_mode ? _BASE_REST_OPS : _BASE_REST_OPS_TESTNET;
	full_path += "/vapi/v1/userDataStream";
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response["response"]["listenKey"].asString();
}

/**
	Ping/Keep-alive a ListenKey
	@param listen_key - the listen_key
	@return json returned by the request
*/
Json::Value OpsClient::v_ping_listen_key(const std::string& listen_key)
{
	std::string full_path = !this->_testnet_mode ? _BASE_REST_OPS : _BASE_REST_OPS_TESTNET;
	full_path += ("/vapi/v1/userDataStream?listenKey=" + listen_key);
	Json::Value response = listen_key.empty() ? (this->_rest_client)->_putreq(full_path) : (this->_rest_client)->_postreq(full_path);

	return response;
}

/**
	Revoke a ListenKey
	@param listen_key - the listen_key
	@return json returned by the request
*/
Json::Value OpsClient::v_revoke_listen_key(const std::string& listen_key)
{
	std::string full_path = !this->_testnet_mode ? _BASE_REST_OPS : _BASE_REST_OPS_TESTNET;
	full_path += ("/vapi/v1/userDataStream?listenKey=" + listen_key);
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response;
}

//  ------------------------------ End | OpsClient CRTP methods - Client infrastructure

//  ------------------------------ Start | OpsClient CRTP methods - Market Data Implementations

/**
	CRTP of ping_client()
*/
bool OpsClient::v_ping_client()
{
	std::string full_path = !this->_testnet_mode ? _BASE_REST_OPS : _BASE_REST_OPS_TESTNET;
	full_path += "/vapi/v1/ping";
	Json::Value ping_response = (this->_rest_client)->_getreq(full_path)["response"];
	return (ping_response != Json::nullValue);
}

/**
	CRTP of exchange_time()
*/
unsigned long long OpsClient::v_exchange_time()
{
	std::string full_path = !this->_testnet_mode ? _BASE_REST_OPS : _BASE_REST_OPS_TESTNET;
	full_path += "/vapi/v1/time";
	std::string ex_time = (this->_rest_client)->_getreq(full_path)["response"]["serverTime"].asString();

	return std::atoll(ex_time.c_str());
}

/**
	CRTP of exchange_info()
*/
Json::Value OpsClient::v_exchange_info()
{
	std::string full_path = !this->_testnet_mode ? _BASE_REST_OPS : _BASE_REST_OPS_TESTNET;
	full_path += "/vapi/v1/exchangeInfo";
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of public_trades_recent()
*/
Json::Value OpsClient::v_get_ticker(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_OPS : _BASE_REST_OPS_TESTNET;
	full_path += "/vapi/v1/ticker" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**

*/
Json::Value OpsClient::get_spot_index_price(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_OPS : _BASE_REST_OPS_TESTNET;
	full_path += "/vapi/v1/index" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**

*/
Json::Value OpsClient::get_mark_price(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_OPS : _BASE_REST_OPS_TESTNET;
	full_path += "/vapi/v1/mark" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}



/**
	CRTP of order_book()
*/
Json::Value OpsClient::v_order_book(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_OPS : _BASE_REST_OPS_TESTNET;
	full_path += "/vapi/v1/depth" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of klines()
*/
Json::Value OpsClient::v_klines(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_OPS : _BASE_REST_OPS_TESTNET;
	full_path += "/vapi/v1/klines" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of public_trades_recent()
*/
Json::Value OpsClient::v_public_trades_recent(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_OPS : _BASE_REST_OPS_TESTNET;
	full_path += "/vapi/v1/trades" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**
	CRTP of public_trades_historical()
*/
Json::Value OpsClient::v_public_trades_historical(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_OPS : _BASE_REST_OPS_TESTNET;
	full_path += "/vapi/v1/historicalTrades" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

/**

*/
Json::Value OpsClient::funds_transfer(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_OPS : _BASE_REST_OPS_TESTNET;
	full_path += "/vapi/v1/transfer" + query;
	Json::Value response = (this->_rest_client)->_postreq(full_path);
	return response;
}

/**

*/
Json::Value OpsClient::holding_info(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_OPS : _BASE_REST_OPS_TESTNET;
	full_path += "/vapi/v1/position" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}
/**

*/
Json::Value OpsClient::account_funding_flow(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_OPS : _BASE_REST_OPS_TESTNET;
	full_path += "/vapi/v1/bill" + query;
	Json::Value response = (this->_rest_client)->_postreq(full_path);
	return response;
}
/**

*/
Json::Value OpsClient::batch_orders(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_OPS : _BASE_REST_OPS_TESTNET;
	full_path += "/vapi/v1/batchOrders" + query;
	Json::Value response = (this->_rest_client)->_postreq(full_path);
	return response;
}
/**

*/
Json::Value OpsClient::cancel_batch_orders(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_OPS : _BASE_REST_OPS_TESTNET;
	full_path += "/vapi/v1/batchOrders" + query;
	Json::Value response = (this->_rest_client)->_deletereq(full_path);
	return response;
}
//  ------------------------------ End | OpsClient CRTP methods - Market Data Implementations


//  ------------------------------ Start | OpsClient CRTP methods - Trade Implementations


// -- Up to 'Client' Level


/**
	CRTP of account_info()
*/
Json::Value OpsClient::v_account_info(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_OPS : _BASE_REST_OPS_TESTNET;
	full_path += "/vapi/v1/account" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}


/**
	CRTP of test_new_order()
*/
Json::Value OpsClient::v_test_new_order(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_OPS : _BASE_REST_OPS_TESTNET;
	full_path += "/vapi/v1/order" + query;
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response;
}

/**
	CRTP of new_order()
*/
Json::Value OpsClient::v_new_order(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_OPS : _BASE_REST_OPS_TESTNET;
	full_path += "/vapi/v1/order" + query;
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response;
}

/**
	CRTP of modify_order()
*/
Json::Value OpsClient::v_modify_order(const Params* params_ptr)
{
    MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}

/**
	CRTP of cancel_order()
*/
Json::Value OpsClient::v_cancel_order(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_OPS : _BASE_REST_OPS_TESTNET;
	full_path += "/vapi/v1/order" + query;
	Json::Value response = (this->_rest_client)->_deletereq(full_path);

	return response;
}

/**
	CRTP of cancel_all_orders()
*/
Json::Value OpsClient::v_cancel_all_orders(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_OPS : _BASE_REST_OPS_TESTNET;
	full_path += "/vapi/v1/allOpenOrders" + query;
	Json::Value response = (this->_rest_client)->_deletereq(full_path);

	return response;
}

/**
	CRTP of query_order()
*/
Json::Value OpsClient::v_query_order(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_OPS : _BASE_REST_OPS_TESTNET;
	full_path += " /vapi/v1/order" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

/**
	CRTP of open_orders()
*/
Json::Value OpsClient::v_open_orders(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_OPS : _BASE_REST_OPS_TESTNET;
	full_path += "/vapi/v1/openOrders" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

/**
	CRTP of all_orders()
*/
Json::Value OpsClient::v_all_orders(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_OPS : _BASE_REST_OPS_TESTNET;
	full_path += "/vapi/v1/historyOrders" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

/**
	CRTP of account_trades_list()
*/
Json::Value OpsClient::v_account_trades_list(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_OPS : _BASE_REST_OPS_TESTNET;
	full_path += "/vapi/v1/userTrades" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

/**
	CRTP of public_trades_agg()
*/
Json::Value OpsClient::v_public_trades_agg(const Params* params_ptr)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}

/**
	CRTP of get_order_book_ticker()
*/
Json::Value OpsClient::v_get_order_book_ticker(const Params* params_ptr)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}
/**
	CRTP of daily_ticker_stats()
*/
Json::Value OpsClient::v_daily_ticker_stats(const Params* params_ptr)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}


// WIP

//  ------------------------------ End | SpotClient CRTP methods - Trade Implementations

//  ------------------------------ Start | SpotClient General methods - Trade Implementations 


//  ------------------------------ End | OpsClient General methods - Trade Implementations 


//  ------------------------------ Start | OpsClient General methods - WS Streams



//  ------------------------------ End | OpsClient General methods - WS Streams


// =======================================================================================================



//  ------------------------------ Start | Params methods

/**
	Default constructor
*/
Params::Params()
	: default_recv{ 0 }, default_recv_amt{ 0 }
{};

/**
	Copy constructor
*/
Params::Params(Params& params_obj)
{
	this->param_map = params_obj.param_map;
	this->default_recv = params_obj.default_recv;
	this->default_recv_amt = params_obj.default_recv_amt;
}

/**
	Copy constructor (constant)
*/
Params::Params(const Params& params_obj)
{
	this->param_map = params_obj.param_map;
	this->default_recv = params_obj.default_recv;
	this->default_recv_amt = params_obj.default_recv_amt;
}

/**
	Copy assignment
*/
Params& Params::operator=(Params& params_obj)
{
	this->param_map = params_obj.param_map;
	this->default_recv = params_obj.default_recv;
	this->default_recv_amt = params_obj.default_recv_amt;

	return *this;
}

/**
	Copy assignment (constant)
*/
Params& Params::operator=(const Params& params_obj)
{
	this->param_map = params_obj.param_map;
	this->default_recv = params_obj.default_recv;
	this->default_recv_amt = params_obj.default_recv_amt;

	return *this;
}

/**
	Move assignment
*/
Params& Params::operator=(Params&& params_obj)
{
	this->param_map = std::move(params_obj.param_map);
	this->default_recv = std::move(params_obj.default_recv);
	this->default_recv_amt = std::move(params_obj.default_recv_amt);

	return *this;
}

/**
	Set a new parameter
	@typename PT - Type of value
	@param key - name of parameter
	@param value - value of parameter to set
*/
template <typename PT>
void Params::set_param(const std::string& key, const PT& value)
{
	param_map[key] = std::to_string(value);
}

/**
	Set a new parameter - type of value = std::string
	@param key - name of parameter
	@param value - value of parameter to set
*/
template <> // do not call to_string on a string
void Params::set_param<std::string>(const std::string& key, const std::string& value)
{
	param_map[key] = value;
}

/**
	Set a new parameter
	@typename PT - Type of value
	@param key - name of parameter
	@param value - value of parameter to set (Move)
*/
template <typename PT>
void Params::set_param(const std::string& key, PT&& value)
{
	param_map[key] = std::to_string(std::move(value));
}

/**
	Set a new parameter - type of value = std::string
	@param key - name of parameter
	@param value - value of parameter to set (Move)
*/
template <>
void Params::set_param<std::string>(const std::string& key, std::string&& value)
{
	param_map[key] = std::move(value);
}

/**
	Delete a parameter
	@param key - name of parameter
	@return - bool for success
*/
bool Params::delete_param(const std::string& key)
{
	std::unordered_map<std::string, std::string>::iterator itr;

	for (itr = this->param_map.begin(); itr != this->param_map.end(); itr++)
	{
		if (itr->first == key)
		{
			this->param_map.erase(itr);
			return 1;
		}
	}
	return 0;
}

/**
	Set a default RecvWindow value
	@param set_always - a bool for whether to keep after flush
	@param recv_val - recvWindow value
*/
void Params::set_recv(const bool& set_always, const unsigned int& recv_val)
{
	this->default_recv = set_always;
	if (set_always)
	{
		this->default_recv_amt = recv_val;
		this->set_param<unsigned int>("recvWindow", recv_val);
	}
	else
	{
		this->default_recv_amt = 0;
		this->delete_param("recvWindow");
	}

}

/**
	Flush params - erase all keys
	if default recvWindow is set, it will be kept
*/
void Params::flush_params()
{
	this->param_map.clear();
	if(this->default_recv)
	{
		this->set_param<unsigned int>("recvWindow", this->default_recv_amt);
	}
}


//  ------------------------------ End | Params methods

//  ------------------------------ Start | Explicit template instantiations

template class Client<SpotClient>;

template class Client<FuturesClient<FuturesClientUSDT>>;
template Json::Value FuturesClient<FuturesClientUSDT>::get_position_mode(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientUSDT>::change_position_mode(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientUSDT>::batch_orders(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientUSDT>::modify_batch_orders(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientUSDT>::modify_batch_orders_history(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientUSDT>::cancel_batch_orders(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientUSDT>::cancel_all_orders_timer(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientUSDT>::query_open_order(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientUSDT>::account_balances(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientUSDT>::change_leverage(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientUSDT>::change_margin_type(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientUSDT>::change_position_margin(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientUSDT>::change_position_margin_history(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientUSDT>::position_info(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientUSDT>::get_income_history(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientUSDT>::get_leverage_bracket(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientUSDT>::users_force_orders(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientUSDT>::pos_adl_quantile_est(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientUSDT>::get_user_comission_rate(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientUSDT>::api_quant_trading_rules(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientUSDT>::get_blvt_kline_history(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientUSDT>::get_composite_index_symbol_info(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientUSDT>::mark_price(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientUSDT>::open_interest(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientUSDT>::composite_index_symbol_info(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientUSDT>::continues_klines(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientUSDT>::index_klines(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientUSDT>::mark_klines(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientUSDT>::funding_rate_history(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientUSDT>::open_interest_stats(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientUSDT>::top_long_short_ratio(const Params* params_ptr, bool accounts);
template Json::Value FuturesClient<FuturesClientUSDT>::global_long_short_ratio(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientUSDT>::taker_long_short_ratio(const Params* params_ptr);

template class Client<FuturesClient<FuturesClientCoin>>;
template Json::Value FuturesClient<FuturesClientCoin>::get_position_mode(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientCoin>::change_position_mode(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientCoin>::batch_orders(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientCoin>::modify_batch_orders(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientCoin>::modify_batch_orders_history(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientCoin>::cancel_batch_orders(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientCoin>::cancel_all_orders_timer(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientCoin>::query_open_order(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientCoin>::account_balances(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientCoin>::change_leverage(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientCoin>::change_margin_type(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientCoin>::change_position_margin(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientCoin>::change_position_margin_history(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientCoin>::position_info(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientCoin>::get_income_history(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientCoin>::get_leverage_bracket(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientCoin>::users_force_orders(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientCoin>::pos_adl_quantile_est(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientCoin>::get_user_comission_rate(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientCoin>::api_quant_trading_rules(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientCoin>::get_blvt_kline_history(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientCoin>::get_composite_index_symbol_info(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientCoin>::mark_price(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientCoin>::open_interest(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientCoin>::composite_index_symbol_info(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientCoin>::continues_klines(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientCoin>::index_klines(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientCoin>::mark_klines(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientCoin>::funding_rate_history(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientCoin>::open_interest_stats(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientCoin>::top_long_short_ratio(const Params* params_ptr, bool accounts);
template Json::Value FuturesClient<FuturesClientCoin>::global_long_short_ratio(const Params* params_ptr);
template Json::Value FuturesClient<FuturesClientCoin>::taker_long_short_ratio(const Params* params_ptr);

template class Client<OpsClient>;

template void Params::set_param<int>(const std::string& key, const int& value);
template void Params::set_param<float>(const std::string& key, const float& value);
template void Params::set_param<double>(const std::string& key, const double& value);

template void Params::set_param<int>(const std::string& key, int&& value);
template void Params::set_param<float>(const std::string& key, float&& value);
template void Params::set_param<double>(const std::string& key, double&& value);

