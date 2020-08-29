
#include "../include/Binance_Client.h"

//  ------------------------------ Start Client General methods - Infrastructure

const std::string _BASE_REST_FUTURES_USDT{ "https://fapi.binance.com" };
const std::string _BASE_REST_FUTURES_COIN{ "https://dapi.binance.com" };
const std::string _BASE_REST_FUTURES_TESTNET{ "https://testnet.binancefuture.com" };
const std::string _BASE_REST_SPOT{ "https://api.binance.com" };
const std::string _WS_BASE_FUTURES_USDT{ "fstream.binance.com" };
const std::string _WS_BASE_FUTURES_USDT_TESTNET{ "stream.binancefuture.com" };
const std::string _WS_BASE_FUTURES_COIN{ "dstream.binance.com" };
const std::string _WS_BASE_FUTURES_COIN_TESTNET{ "dstream.binancefuture.com" };
const std::string _WS_BASE_SPOT{ "stream.binance.com" };
const unsigned int _WS_PORT_SPOT{ 9443 };
const unsigned int _WS_PORT_FUTURES{ 443 };

template<typename T>
Client<T>::Client(T& exchange_client) : _public_client{ 1 }, refresh_listenkey_interval{ 1800 }
{
	try
	{
		this->init_rest_session(); // important to init rest first - ws is dependant on it
		this->_ws_client = new WebsocketClient<T>{ exchange_client, "", 0 };
		this->init_ws_session();
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

template<typename T>
Client<T>::Client(T& exchange_client, std::string key, std::string secret) : _public_client{ 0 }, _api_key{ key }, _api_secret{ secret }, refresh_listenkey_interval{ 1800 }
{
	try
	{
		this->init_rest_session();
		this->_ws_client = new WebsocketClient<T>{exchange_client, "", 0 }; 
		this->init_ws_session();
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

template <typename T>
Client<T>::~Client()
{

	delete _rest_client;
	delete _ws_client;
};

//  ------------------------------ End | Client General methods - Infrastructure

//  ------------------------------ Start | Client CRTP methods - Infrastructure

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

template<typename T>
void Client<T>::close_stream(const std::string& symbol, const std::string& stream_name) 
{
	try
	{
		static_cast<T*>(this)->v_close_stream(symbol, stream_name);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

template<typename T>
bool Client<T>::is_stream_open(const std::string& symbol, const std::string& stream_name)
{
	try
	{
		return static_cast<T*>(this)->v_is_stream_open(symbol, stream_name);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

template<typename T>
std::vector<std::string> Client<T>::get_open_streams() 
{
	try
	{
		return static_cast<T*>(this)->v_get_open_streams();
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}


//  ------------------------------ End | Client CRTP methods - Infrastructure

//  ------------------------------ Start | Client CRTP methods - Market Data Endpoints 

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


template<typename T>
template <typename FT>
unsigned int  Client<T>::stream_Trade(const std::string& symbol, std::string& buffer, FT& functor)
{
	try
	{
	return static_cast<T*>(this)->v_stream_Trade(symbol, buffer, functor); 
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}


//  ------------------------------ End | Client Global + CRTP methods - WS Streams


//  ------------------------------ Start | Client General methods - Infrastructure

template <typename T>
void Client<T>::set_refresh_key_interval(const unsigned int val)
{
	this->refresh_listenkey_interval = val;
}

template <typename T>
void Client<T>::set_max_reconnect_count(const unsigned int val)
{
	this->_ws_client->_max_reconnect_count = val;
}

template<typename T>
void Client<T>::ws_auto_reconnect(const bool reconnect)
{
	this->_ws_client->_set_reconnect(reconnect);
}

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

	return 0;
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
	}; 

}

template <typename T>
void Client<T>::rest_set_verbose(const bool& state)
{
	if (state) this->_rest_client->set_verbose(1L);
	else this->_rest_client->set_verbose(0);
}

template <typename T>
std::string Client<T>::_generate_query(const Params* params_ptr, const bool& sign_query) const
{
	try
	{
		std::string query;
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
			query = no_params ? "timestamp=" : "&timestamp=";

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
	}
}

template <typename T>
bool Client<T>::exchange_status() // todo: is this abstract?
{
	try
	{ 
		std::string full_path = _BASE_REST_SPOT + "/wapi/v3/systemStatus.html";
		return this->_rest_client->_getreq(full_path)["response"]["status"].asBool();
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}


//  ------------------------------ End | Client General methods - Infrastructure


// ***************************************************************************


//  ------------------------------ Start | Client Wallet - User Wallet Endpoints

// ------ Class methods

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

template <typename T>
Client<T>::Wallet::~Wallet()
{
	user_client = nullptr;
}

// ------ Endpoint methods


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

template <typename T>
Json::Value Client<T>::Wallet::withdraw_balances(const Params* params_ptr, const bool& SAPI)
{
	try
	{
		std::string endpoint = SAPI ? "/sapi/v1/capital/withdraw/apply" : "/wapi/v3/withdraw.html";
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

template <typename T>
Json::Value Client<T>::Wallet::deposit_history(const Params* params_ptr, const bool& network)
{
	try
	{
		std::string endpoint = network ? "/sapi/v1/capital/deposit/hisrec" : "/wapi/v3/depositHistory.html";
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

template <typename T>
Json::Value Client<T>::Wallet::withdraw_history(const Params* params_ptr, const bool& network)
{
	try
	{
		std::string endpoint = network ? "/sapi/v1/capital/withdraw/history" : "/wapi/v3/withdrawHistory.html";
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

template <typename T>
Json::Value Client<T>::Wallet::deposit_address(const Params* params_ptr, const bool& network)
{
	try
	{
		std::string endpoint = network ? "/sapi/v1/capital/deposit/address" : "/wapi/v3/depositAddress.html";
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

template <typename T>
Json::Value Client<T>::Wallet::account_status(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/wapi/v3/accountStatus.html";
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

template <typename T>
Json::Value Client<T>::Wallet::account_status_api(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/wapi/v3/apiTradingStatus.html";
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

template <typename T>
Json::Value Client<T>::Wallet::dust_log(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/wapi/v3/userAssetDribbletLog.html";
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

template <typename T>
Json::Value Client<T>::Wallet::asset_details(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/wapi/v3/assetDetail.html";
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

template <typename T>
Json::Value Client<T>::Wallet::trading_fees(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/wapi/v3/tradeFee.html";
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

template <typename T>
Client<T>::FuturesWallet::~FuturesWallet()
{
	user_client = nullptr;
}

// ------ Endpoint methods


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

template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_repay(const Params* params_ptr)
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

template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_wallet(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/futures/loan/wallet";
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

template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_info(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/futures/loan/configs";
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

template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_adjust_calc_rate(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/futures/loan/calcAdjustLevel";
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

template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_adjust_get_max(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/futures/loan/calcMaxAdjustAmount";
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

template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_adjust(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/futures/loan/adjustCollateral";
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

//  ------------------------------ End | Client FuturesWallet - User FuturesWallet Endpoints


// ***************************************************************************


//  ------------------------------ Start | Client SubAccount - User SubAccount Endpoints

// ------ Class methods

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

template <typename T>
Client<T>::SubAccount::~SubAccount()
{
	user_client = nullptr;
}

// ------ Endpoint methods

template <typename T>
Json::Value Client<T>::SubAccount::get_all_subaccounts(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/wapi/v3/sub-account/list.html";
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

template <typename T>
Json::Value Client<T>::SubAccount::transfer_master_history(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/wapi/v3/sub-account/transfer/history.html";
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

template <typename T>
Json::Value Client<T>::SubAccount::transfer_master_to_subaccount(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/wapi/v3/sub-account/transfer.html";
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

template <typename T>
Json::Value Client<T>::SubAccount::get_subaccount_balances(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/wapi/v3/sub-account/assets.html";
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

//  ------------------------------ End | Client SubAccount - User SubAccount Endpoints

// ***************************************************************************

//  ------------------------------ Start | Client MarginAccount - User MarginAccount Endpoints

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

template <typename T>
Client<T>::MarginAccount::~MarginAccount()
{
	user_client = nullptr;
}

// ------ Endpoint methods


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

template <typename T>
Json::Value Client<T>::MarginAccount::margin_isolated_margin_create(const Params* params_ptr)
{
	try
	{
		std::string full_path = _BASE_REST_SPOT + "/sapi/v1/margin/isolated/create";
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

template <typename T>
std::string Client<T>::MarginAccount::margin_get_listen_key(const bool& isolated_margin_type)
{
	try
	{
		std::string endpoint = isolated_margin_type ? "/sapi/v1/userDataStream/isolated" : "/sapi/v1/userDataStream";
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

template <typename T>
Json::Value Client<T>::MarginAccount::margin_ping_listen_key(const std::string& listen_key, const bool& isolated_margin_type)
{
	try
	{
		std::string endpoint = isolated_margin_type ? "/sapi/v1/userDataStream/isolated" : "/sapi/v1/userDataStream";
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

template <typename T>
Json::Value Client<T>::MarginAccount::margin_revoke_listen_key(const std::string& listen_key, const bool& isolated_margin_type)
{
	try
	{
		std::string endpoint = isolated_margin_type ? "/sapi/v1/userDataStream/isolated" : "/sapi/v1/userDataStream";
		std::string full_path = _BASE_REST_SPOT + endpoint + "?listenKey=" + listen_key;
		Json::Value response = (this->user_client->_rest_client)->_postreq(full_path);

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

template <typename T>
Client<T>::Savings::~Savings()
{
	user_client = nullptr; 
}

// ------ Endpoint methods


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

//  ------------------------------ End | Client Savings - User Savings Endpoints


// ***************************************************************************

//  ------------------------------ Start | Client Mining - User Mining Endpoints

template <typename T>
Client<T>::Mining::Mining(Client<T>& client_obj)
	: user_client{ &client_obj } // snatching pointer and releasing later on to avoid deleting this reference
{
	if (user_client->_public_client)
	{
		MissingCredentials e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}; 
}

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

template <typename T>
Client<T>::Mining::~Mining()
{
	user_client = nullptr; 
}

// ------ Endpoint methods


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


//  ------------------------------ Start | SpotClient General methods - Infrastructure

SpotClient::SpotClient() : Client(*this)
{};

SpotClient::SpotClient(std::string key, std::string secret)
	: Client(*this, key, secret)
{}


SpotClient::~SpotClient() 
{};

//  ------------------------------ End | SpotClient General methods - Infrastructure

//  ------------------------------ Start | SpotClient CRTP methods - Client infrastructure

void SpotClient::v_init_ws_session()
{

	this->_ws_client->set_host_port(_WS_BASE_SPOT, _WS_PORT_SPOT);
}

std::string SpotClient::v_get_listen_key()
{
	std::string full_path = _BASE_REST_SPOT + "/api/v3/userDataStream";
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response["response"]["listenKey"].asString();
}

Json::Value SpotClient::v_ping_listen_key(const std::string& listen_key)
{
	std::string full_path = _BASE_REST_SPOT + "/api/v3/userDataStream" + "?listenKey=" + listen_key;
	Json::Value response = listen_key.empty() ? (this->_rest_client)->_putreq(full_path) : (this->_rest_client)->_postreq(full_path);

	return response;
}

Json::Value SpotClient::v_revoke_listen_key(const std::string& listen_key)
{
	std::string full_path = _BASE_REST_SPOT + "/api/v3/userDataStream" + "?listenKey=" + listen_key;
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response;
}


void SpotClient::v_close_stream(const std::string& symbol, const std::string& stream_name)
{
	try
	{
		this->_ws_client->close_stream(symbol + "@" + stream_name);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}


bool SpotClient::v_is_stream_open(const std::string& symbol, const std::string& stream_name)
{
	std::string full_stream_name = symbol + '@' + stream_name;
	return this->_ws_client->is_open(full_stream_name);
}

std::vector<std::string> SpotClient::v_get_open_streams()
{
	return this->_ws_client->open_streams();
}


//  ------------------------------ End | SpotClient CRTP methods - Client infrastructure

//  ------------------------------ Start | SpotClient CRTP methods - Market Data Implementations

bool SpotClient::v_ping_client()
{
	std::string full_path = _BASE_REST_SPOT + "/api/v3/ping";
	Json::Value ping_response = (this->_rest_client)->_getreq(full_path)["response"];
	return (ping_response != Json::nullValue);
}

unsigned long long SpotClient::v_exchange_time()
{
	std::string full_path = _BASE_REST_SPOT + "/api/v3/time";
	std::string ex_time = (this->_rest_client)->_getreq(full_path)["response"]["serverTime"].asString();

	return std::atoll(ex_time.c_str());
}
Json::Value SpotClient::v_exchange_info() 
{
	std::string full_path = _BASE_REST_SPOT + "/api/v1/exchangeInfo";
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value SpotClient::v_order_book(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = _BASE_REST_SPOT + "/api/v3/depth" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value SpotClient::v_public_trades_recent(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = _BASE_REST_SPOT + "/api/v3/trades" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value SpotClient::v_public_trades_historical(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = _BASE_REST_SPOT + "/api/v3/historicalTrades" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value SpotClient::v_public_trades_agg(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = _BASE_REST_SPOT + "/api/v3/aggTrades" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value SpotClient::v_klines(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = _BASE_REST_SPOT + "/api/v3/klines" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value SpotClient::v_daily_ticker_stats(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = _BASE_REST_SPOT + "/api/v3/ticker/24hr" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value SpotClient::v_get_ticker(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = _BASE_REST_SPOT + "/api/v3/ticker/price" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

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

Json::Value SpotClient::v_test_new_order(const Params* params_ptr)
{
	std::string full_path = _BASE_REST_SPOT + "/api/v3/order/test";
	std::string query = this->_generate_query(params_ptr, 1);
	Json::Value response = (this->_rest_client)->_postreq(full_path + query);

	return response;
}

Json::Value SpotClient::v_new_order(const Params* params_ptr)
{
	std::string full_path = _BASE_REST_SPOT + "/api/v3/order";
	std::string query = this->_generate_query(params_ptr, 1);
	Json::Value response = (this->_rest_client)->_postreq(full_path + query);

	return response;
}

Json::Value SpotClient::v_cancel_order(const Params* params_ptr)
{
	std::string full_path = _BASE_REST_SPOT + "/api/v3/order";
	std::string query = this->_generate_query(params_ptr, 1);
	Json::Value response = (this->_rest_client)->_deletereq(full_path + query);

	return response;
}

Json::Value SpotClient::v_cancel_all_orders(const Params* params_ptr)
{
	std::string full_path = _BASE_REST_SPOT + "api/v3/openOrders";
	std::string query = this->_generate_query(params_ptr, 1);
	Json::Value response = (this->_rest_client)->_deletereq(full_path + query);

	return response;
}

Json::Value SpotClient::v_query_order(const Params* params_ptr)
{
	std::string full_path = _BASE_REST_SPOT + "/api/v3/order";
	std::string query = this->_generate_query(params_ptr, 1);
	Json::Value response = (this->_rest_client)->_getreq(full_path + query);

	return response;
}

Json::Value SpotClient::v_open_orders(const Params* params_ptr)
{
	std::string full_path = _BASE_REST_SPOT + "/api/v3/openOrders";
	std::string query = this->_generate_query(params_ptr, 1);
	Json::Value response = (this->_rest_client)->_getreq(full_path + query);

	return response;
}

Json::Value SpotClient::v_all_orders(const Params* params_ptr)
{
	std::string full_path = _BASE_REST_SPOT + "/api/v3/allOrders";
	std::string query = this->_generate_query(params_ptr, 1);
	Json::Value response = (this->_rest_client)->_getreq(full_path + query);

	return response;
}

Json::Value SpotClient::v_account_info(const Params* params_ptr)
{
	std::string full_path = _BASE_REST_SPOT + "/api/v3/account";
	std::string query = this->_generate_query(params_ptr, 1);
	Json::Value response = (this->_rest_client)->_getreq(full_path + query);

	return response;
}

Json::Value SpotClient::v_account_trades_list(const Params* params_ptr)
{
	std::string full_path = _BASE_REST_SPOT + "/api/v3/myTrades";
	std::string query = this->_generate_query(params_ptr, 1);
	Json::Value response = (this->_rest_client)->_getreq(full_path + query);

	return response;
}

//  ------------------------------ End | SpotClient CRTP methods - Trade Implementations

//  ------------------------------ Start | SpotClient General methods - Trade Implementations 

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

template <typename CT>
FuturesClient<CT>::FuturesClient(CT& exchange_client)
	: Client<FuturesClient<CT>>(*this), _testnet_mode{ 0 }
{};


template <typename CT>
FuturesClient<CT>::FuturesClient(CT& exchange_client, std::string key, std::string secret)
	: Client<FuturesClient<CT>>(*this, key, secret), _testnet_mode{ 0 }
{}

template <typename CT>
FuturesClient<CT>::~FuturesClient()
{}


template <typename CT>
bool FuturesClient<CT>::get_testnet_mode()
{
	return this->_testnet_mode;
}

//  ------------------------------ End | FuturesClient General methods - Infrastructure


//  ------------------------------ Start | FuturesClient CRTP methods - Client infrastructure


template <typename CT>
void FuturesClient<CT>::v_init_ws_session() { static_cast<CT*>(this)->v__init_ws_session(); }

template <typename CT>
void FuturesClient<CT>::set_testnet_mode(const bool& status) { return static_cast<CT*>(this)->v_set_testnet_mode(status); }


template <typename CT>
void FuturesClient<CT>::v_close_stream(const std::string& symbol, const std::string& stream_name)
{
	try
	{
		this->_ws_client->close_stream(symbol + "@" + stream_name);
	}
	catch (...)
	{
		BadStreamCloseWS e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

template <typename CT>
bool FuturesClient<CT>::v_is_stream_open(const std::string& symbol, const std::string& stream_name)
{
	std::string full_stream_name = symbol + '@' + stream_name;
	return this->_ws_client->is_open(full_stream_name);
}


template <typename CT>
std::vector<std::string> FuturesClient<CT>::v_get_open_streams()
{
	return this->_ws_client->open_streams();
}


//  ------------------------------ End | FuturesClient CRTP methods - Client infrastructure

//  ------------------------------ Start | FuturesClient CRTP methods - Market Data Implementations

template<typename CT>
unsigned long long FuturesClient<CT>::v_exchange_time() { return static_cast<CT*>(this)->v__exchange_time(); }

template<typename CT>
bool FuturesClient<CT>::v_ping_client() { return static_cast<CT*>(this)->v__ping_client(); }

template<typename CT>
Json::Value FuturesClient<CT>::v_exchange_info() { return static_cast<CT*>(this)->v__exchange_info(); }

template<typename CT>
Json::Value FuturesClient<CT>::v_order_book(const Params* params_ptr) { return static_cast<CT*>(this)->v__order_book(params_ptr); }

template<typename CT>
Json::Value FuturesClient<CT>::v_public_trades_recent(const Params* params_ptr) { return static_cast<CT*>(this)->v__public_trades_recent(params_ptr); }

template<typename CT>
Json::Value FuturesClient<CT>::v_public_trades_historical(const Params* params_ptr) { return static_cast<CT*>(this)->v__public_trades_historical(params_ptr); }

template<typename CT>
Json::Value FuturesClient<CT>::v_public_trades_agg(const Params* params_ptr) { return static_cast<CT*>(this)->v__public_trades_agg(params_ptr); }

template<typename CT>
Json::Value FuturesClient<CT>::v_klines(const Params* params_ptr) { return static_cast<CT*>(this)->v__klines(params_ptr); }

template<typename CT>
Json::Value FuturesClient<CT>::v_daily_ticker_stats(const Params* params_ptr) { return static_cast<CT*>(this)->v__daily_ticker_stats(params_ptr); }

template<typename CT>
Json::Value FuturesClient<CT>::v_get_ticker(const Params* params_ptr) { return static_cast<CT*>(this)->v__get_ticker(params_ptr); }

template<typename CT>
Json::Value FuturesClient<CT>::v_get_order_book_ticker(const Params* params_ptr) { return static_cast<CT*>(this)->v__get_order_book_ticker(params_ptr); }

//  ------------------------------ End | FuturesClient CRTP methods - Market Data Implementations


//  ------------------------------ Start | FuturesClient CRTP methods - Unique Endpoints

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

template<typename CT>
Json::Value FuturesClient<CT>::public_liquidation_orders(const Params* params_ptr) 
{
	try
	{
		return static_cast<CT*>(this)->v_public_liquidation_orders(params_ptr);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

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

//  ------------------------------ End | FuturesClient CRTP methods - Unique Endpoints


//  ------------------------------ Start | FuturesClient CRTP methods - Trade Implementations 

// -- Up to 'Client' Level

template<typename CT>
Json::Value FuturesClient<CT>::v_test_new_order(const Params* params_ptr) { throw("please use testnet instead!"); } // todo: exceptions

template<typename CT>
Json::Value FuturesClient<CT>::v_new_order(const Params* params_ptr) { return static_cast<CT*>(this)->v__new_order(params_ptr); }

template<typename CT>
Json::Value FuturesClient<CT>::v_cancel_order(const Params* params_ptr) { return static_cast<CT*>(this)->v__cancel_order(params_ptr); }

template<typename CT>
Json::Value FuturesClient<CT>::v_cancel_all_orders(const Params* params_ptr) { return static_cast<CT*>(this)->v__cancel_all_orders(params_ptr); }

template<typename CT>
Json::Value FuturesClient<CT>::v_query_order(const Params* params_ptr) { return static_cast<CT*>(this)->v__query_order(params_ptr); }

template<typename CT>
Json::Value FuturesClient<CT>::v_open_orders(const Params* params_ptr) { return static_cast<CT*>(this)->v__open_orders(params_ptr); }

template<typename CT>
Json::Value FuturesClient<CT>::v_all_orders(const Params* params_ptr) { return static_cast<CT*>(this)->v__all_orders(params_ptr); }

template<typename CT>
Json::Value FuturesClient<CT>::v_account_info(const Params* params_ptr) { return static_cast<CT*>(this)->v__account_info(params_ptr); }

template<typename CT>
Json::Value FuturesClient<CT>::v_account_trades_list(const Params* params_ptr) { return static_cast<CT*>(this)->v__account_trades_list(params_ptr); }

// -- Up to 'FuturesClient' (this) Level

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


//  ------------------------------ End | FuturesClient CRTP methods - Trade Implementations


//  ------------------------------ Start | FuturesClient Global + CRTP methods - WS Streams 

template <typename CT>
std::string FuturesClient<CT>::v_get_listen_key() { return static_cast<CT*>(this)->v__get_listen_key(); }

template <typename CT>
Json::Value FuturesClient<CT>::v_ping_listen_key(const std::string& listen_key) { return static_cast<CT*>(this)->v__ping_listen_key(); }

template <typename CT>
Json::Value FuturesClient<CT>::v_revoke_listen_key(const std::string& listen_key) { return static_cast<CT*>(this)->v__revoke_listen_key(); }

//  ------------------------------ End | FuturesClient Global + CRTP methods - WS Streams 


//  ------------------------------ Start | FuturesClient General methods - Markets Stats

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

template <typename CT>
Json::Value FuturesClient<CT>::basis_data(const Params* params_ptr)
{
	try
	{
		std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
		std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
		full_path += ("/futures/data/basis" + query);
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

FuturesClientUSDT::FuturesClientUSDT()
	: FuturesClient(*this)
{};

FuturesClientUSDT::FuturesClientUSDT(std::string key, std::string secret)
	: FuturesClient(*this, key, secret)
{}

FuturesClientUSDT::~FuturesClientUSDT()
{}

void FuturesClientUSDT::v__init_ws_session()
{
	this->_ws_client->set_host_port(_WS_BASE_FUTURES_USDT, _WS_PORT_FUTURES);
}

void FuturesClientUSDT::v_set_testnet_mode(const bool& status)
{
	if (status) this->_ws_client->set_host_port(_WS_BASE_FUTURES_USDT_TESTNET, _WS_PORT_FUTURES);
	else this->_ws_client->set_host_port(_WS_BASE_FUTURES_USDT, _WS_PORT_FUTURES);
	this->_testnet_mode = status;

}

//  ------------------------------ Start | FuturesClientUSDT CRTP methods - Market Data Implementations

inline bool FuturesClientUSDT::v__ping_client()
{
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += "/fapi/v1/ping";
	Json::Value ping_response = (this->_rest_client)->_getreq(full_path)["response"];
	return (ping_response != Json::nullValue);
}

inline unsigned long long FuturesClientUSDT::v__exchange_time()
{
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += "/fapi/v1/time";
	std::string ex_time = (this->_rest_client)->_getreq(full_path)["response"]["serverTime"].asString();

	return std::atoll(ex_time.c_str());
}

Json::Value FuturesClientUSDT::v__exchange_info()
{
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += "/fapi/v1/exchangeInfo";
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientUSDT::v__order_book(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/depth" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientUSDT::v__public_trades_recent(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/trades" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientUSDT::v__public_trades_historical(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/historicalTrades" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientUSDT::v__public_trades_agg(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/aggTrades" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientUSDT::v__klines(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/klines" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientUSDT::v__daily_ticker_stats(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/ticker/24hr" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientUSDT::v__get_ticker(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/ticker/price" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

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

Json::Value FuturesClientUSDT::v_mark_price(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/premiumIndex" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientUSDT::v_public_liquidation_orders(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/allForceOrders" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}
Json::Value FuturesClientUSDT::v_open_interest(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/openInterest" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

// ~~~ Do not exist for this client

Json::Value FuturesClientUSDT::v_continues_klines(const Params* params_ptr)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}
Json::Value FuturesClientUSDT::v_index_klines(const Params* params_ptr)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}
Json::Value FuturesClientUSDT::v_mark_klines(const Params* params_ptr)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}


// ~~~ Unique for this Client 

Json::Value FuturesClientUSDT::v_funding_rate_history(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/fundingRate" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

//  ------------------------------ End | FuturesClientUSDT CRTP methods - Unique Endpoints

//  ------------------------------ Start | FuturesClientUSDT CRTP methods - Trade Implementations 


// -- Up to 'Client' Level


Json::Value FuturesClientUSDT::v__new_order(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/order" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response;
}

Json::Value FuturesClientUSDT::v__cancel_order(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += "/fapi/v1/order";
	Json::Value response = (this->_rest_client)->_deletereq(full_path);

	return response;
}

Json::Value FuturesClientUSDT::v__cancel_all_orders(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += "/fapi/v1/allOpenOrders";
	Json::Value response = (this->_rest_client)->_deletereq(full_path);

	return response;
}


Json::Value FuturesClientUSDT::v__query_order(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/order" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

Json::Value FuturesClientUSDT::v__open_orders(const Params* params_ptr)
{

	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/openOrders" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

Json::Value FuturesClientUSDT::v__all_orders(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/allOrders" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

Json::Value FuturesClientUSDT::v__account_info(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v2/account" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

Json::Value FuturesClientUSDT::v__account_trades_list(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/userTrades" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

// -- Up to 'FuturesClient' (this) Level


Json::Value FuturesClientUSDT::v_change_position_mode(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/positionSide/dual" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path); // should be spot?

	return response;
}

Json::Value FuturesClientUSDT::v_get_position_mode(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/positionSide/dual" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path); // should be spot?

	return response;
}

Json::Value FuturesClientUSDT::v_batch_orders(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/batchOrders" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path); // should be spot?

	return response;
}

Json::Value FuturesClientUSDT::v_cancel_batch_orders(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/batchOrders" + query);
	Json::Value response = (this->_rest_client)->_deletereq(full_path); // should be spot?

	return response;
}

Json::Value FuturesClientUSDT::v_cancel_all_orders_timer(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/countdownCancelAll" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path); // should be spot?

	return response;
}

Json::Value FuturesClientUSDT::v_query_open_order(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/openOrder" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path); // should be spot?

	return response;
}

Json::Value FuturesClientUSDT::v_account_balances(const Params* params_ptr)
{

	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v2/balance" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path); // should be spot?

	return response;
}

Json::Value FuturesClientUSDT::v_change_leverage(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/leverage" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path); // should be spot?

	return response;
}

Json::Value FuturesClientUSDT::v_change_margin_type(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/marginType" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path); // should be spot?

	return response;
}

Json::Value FuturesClientUSDT::v_change_position_margin(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/positionMargin" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path); // should be spot?

	return response;
}

Json::Value FuturesClientUSDT::v_change_position_margin_history(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/positionMargin/history" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path); // should be spot?

	return response;
}

Json::Value FuturesClientUSDT::v_position_info(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v2/positionRisk" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

Json::Value FuturesClientUSDT::v_get_income_history(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/income" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

Json::Value FuturesClientUSDT::v_get_leverage_bracket(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/leverageBracket" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

Json::Value FuturesClientUSDT::v_pos_adl_quantile_est(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/adlQuantile" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

//  ------------------------------ End | FuturesClientUSDT CRTP methods - Trade Implementations 

//  ------------------------------ Start | FuturesClientUSDT CRTP methods - WS Streams


std::string FuturesClientUSDT::v__get_listen_key()
{
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += "/fapi/v1/listenKey";
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response["response"]["listenKey"].asString();
}

Json::Value FuturesClientUSDT::v__ping_listen_key()
{
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_USDT : _BASE_REST_FUTURES_TESTNET;
	full_path += "/fapi/v1/listenKey";

	Json::Value response = (this->_rest_client)->_putreq(full_path);

	return response;
}

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

FuturesClientCoin::FuturesClientCoin()
	: FuturesClient(*this)
{};

FuturesClientCoin::FuturesClientCoin(std::string key, std::string secret)
	: FuturesClient(*this, key, secret)
{}

FuturesClientCoin::~FuturesClientCoin()
{}

void FuturesClientCoin::v__init_ws_session()
{

	this->_ws_client->set_host_port(_WS_BASE_FUTURES_COIN, _WS_PORT_FUTURES);

}

void FuturesClientCoin::v_set_testnet_mode(const bool& status)
{
	if (status) this->_ws_client->set_host_port(_WS_BASE_FUTURES_COIN_TESTNET, _WS_PORT_FUTURES);
	else this->_ws_client->set_host_port(_WS_BASE_FUTURES_COIN, _WS_PORT_FUTURES);
	this->_testnet_mode = status;
}

//  ------------------------------ End | FuturesClientCoin General methods - Infrastructure

//  ------------------------------ Start | FuturesClientCoin CRTP methods - Market Data Implementations

inline bool FuturesClientCoin::v__ping_client()
{
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += "/dapi/v1/ping";
	Json::Value ping_response = (this->_rest_client)->_getreq(full_path)["response"];
	return (ping_response != Json::nullValue);
}

inline unsigned long long FuturesClientCoin::v__exchange_time()
{
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += "/dapi/v1/time";
	std::string ex_time = (this->_rest_client)->_getreq(full_path)["response"]["serverTime"].asString();

	return std::atoll(ex_time.c_str());
}

Json::Value FuturesClientCoin::v__exchange_info()
{
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += "/dapi/v1/exchangeInfo";
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientCoin::v__order_book(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/depth" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientCoin::v__public_trades_recent(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/trades" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientCoin::v__public_trades_historical(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/historicalTrades" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientCoin::v__public_trades_agg(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/aggTrades" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientCoin::v__klines(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/klines" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientCoin::v__daily_ticker_stats(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/ticker/24hr" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientCoin::v__get_ticker(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/ticker/price" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

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


Json::Value FuturesClientCoin::v__new_order(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/order" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response;
}

Json::Value FuturesClientCoin::v__cancel_order(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/order" + query);
	Json::Value response = (this->_rest_client)->_deletereq(full_path);

	return response;
}

Json::Value FuturesClientCoin::v__cancel_all_orders(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/allOpenOrders" + query);
	Json::Value response = (this->_rest_client)->_deletereq(full_path);

	return response;
}

Json::Value FuturesClientCoin::v__query_order(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/order" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

Json::Value FuturesClientCoin::v__open_orders(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/openOrders" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

Json::Value FuturesClientCoin::v__all_orders(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/allOrders" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

Json::Value FuturesClientCoin::v__account_info(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/account" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

Json::Value FuturesClientCoin::v__account_trades_list(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/userTrades" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

// -- Up to 'FuturesClient' (this) Level


Json::Value FuturesClientCoin::v_change_position_mode(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/positionSide/dual" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response;
}


Json::Value FuturesClientCoin::v_get_position_mode(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/positionSide/dual" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

Json::Value FuturesClientCoin::v_batch_orders(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/batchOrders" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response;
}

Json::Value FuturesClientCoin::v_cancel_batch_orders(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/batchOrders" + query);
	Json::Value response = (this->_rest_client)->_deletereq(full_path);

	return response;
}

Json::Value FuturesClientCoin::v_cancel_all_orders_timer(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/countdownCancelAll" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response;
}

Json::Value FuturesClientCoin::v_query_open_order(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/openOrder" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

Json::Value FuturesClientCoin::v_account_balances(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/balance" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

Json::Value FuturesClientCoin::v_change_leverage(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/leverage" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response;
}

Json::Value FuturesClientCoin::v_change_margin_type(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/marginType" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response;
}

Json::Value FuturesClientCoin::v_change_position_margin(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/positionMargin" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response;
}

Json::Value FuturesClientCoin::v_change_position_margin_history(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/positionMargin/history" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

Json::Value FuturesClientCoin::v_position_info(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/positionRisk" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

Json::Value FuturesClientCoin::v_get_income_history(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/income" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

Json::Value FuturesClientCoin::v_get_leverage_bracket(const Params* params_ptr)
{
	std::string query = this->_generate_query(params_ptr, 1);
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/leverageBracket" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

Json::Value FuturesClientCoin::v_pos_adl_quantile_est(const Params* params_ptr)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}

//  ------------------------------ End | FuturesClientUSDT CRTP methods - Trade Implementations 

//  ------------------------------ Start | FuturesClientUSDT CRTP methods - WS Streams


template <typename FT>
unsigned int FuturesClientCoin::v_stream_markprice_all(const std::string& symbol, std::string& buffer, FT& functor) // here
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}


std::string FuturesClientCoin::v__get_listen_key()
{
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += "/dapi/v1/listenKey";
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response["response"]["listenKey"].asString();
}

Json::Value FuturesClientCoin::v__ping_listen_key()
{
	// no signature is needed here
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += "/dapi/v1/listenKey";

	Json::Value response = (this->_rest_client)->_putreq(full_path);

	return response;
}

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

Json::Value FuturesClientCoin::v_mark_price(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/premiumIndex" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientCoin::v_public_liquidation_orders(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/allForceOrders" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}
Json::Value FuturesClientCoin::v_open_interest(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/openInterest" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}


// ~~~ Unique for this Client 

Json::Value FuturesClientCoin::v_continues_klines(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/continuousKlines" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}
Json::Value FuturesClientCoin::v_index_klines(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/indexPriceKlines" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}
Json::Value FuturesClientCoin::v_mark_klines(const Params* params_ptr)
{
	std::string query = params_ptr ? this->_generate_query(params_ptr) : "";
	std::string full_path = !this->_testnet_mode ? _BASE_REST_FUTURES_COIN : _BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/markPriceKlines" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}


// ~~~ Don't exist for this client

Json::Value FuturesClientCoin::v_funding_rate_history(const Params* params_ptr)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}

//  ------------------------------ End | FuturesClientCoin CRTP methods - Unique Endpoints

//  ------------------------------ Start | FuturesClientCoin CRTP methods - WS Streams

// -- Global (up to Client level)
// todo: if testnet


//  ------------------------------ End | FuturesClientCoin CRTP methods - WS Streams


// =======================================================================================================


//  ------------------------------ Start | Params methods

Params::Params()
	: default_recv{ 0 }, default_recv_amt{ 0 }
{};

Params::Params(Params& params_obj)
{
	this->param_map = params_obj.param_map;
	this->default_recv = params_obj.default_recv;
	this->default_recv_amt = params_obj.default_recv_amt;
}

Params::Params(const Params& params_obj)
{
	this->param_map = params_obj.param_map;
	this->default_recv = params_obj.default_recv;
	this->default_recv_amt = params_obj.default_recv_amt;
}

Params& Params::operator=(Params& params_obj)
{
	this->param_map = params_obj.param_map;
	this->default_recv = params_obj.default_recv;
	this->default_recv_amt = params_obj.default_recv_amt;

	return *this;
}

Params& Params::operator=(const Params& params_obj)
{
	this->param_map = params_obj.param_map;
	this->default_recv = params_obj.default_recv;
	this->default_recv_amt = params_obj.default_recv_amt;

	return *this;
}

Params& Params::operator=(Params&& params_obj)
{
	this->param_map = std::move(params_obj.param_map);
	this->default_recv = std::move(params_obj.default_recv);
	this->default_recv_amt = std::move(params_obj.default_recv_amt);

	return *this;
}

template <typename PT>
void Params::set_param(const std::string& key, const PT& value)
{
	param_map[key] = std::to_string(value);
}
template <> // do not call to_string on a string
void Params::set_param<std::string>(const std::string& key, const std::string& value)
{
	param_map[key] = value;
}

template <typename PT>
void Params::set_param(const std::string& key, PT&& value)
{
	param_map[key] = std::to_string(std::move(value));
}
template <>
void Params::set_param<std::string>(const std::string& key, std::string&& value)
{
	param_map[key] = std::move(value);
}


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

bool Params::empty() const
{
	return this->param_map.empty();
}

//  ------------------------------ End | Params methods

template class Client<SpotClient>;
template class Client<FuturesClient<FuturesClientUSDT>>;
template class Client<FuturesClient<FuturesClientCoin>>;
