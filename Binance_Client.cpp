
#include "CryptoExtensions.h"
#include "Websocket_Client.cpp" // because of templates

//  ------------------------------ Start Client General methods - Infrastructure

template<typename T>
Client<T>::Client() : _public_client{ 1 }
{};

template<typename T>
Client<T>::Client(std::string key, std::string secret) : _public_client{ 0 }, _api_key{ key }, _api_secret{ secret }
{};

template <typename T>
Client<T>::~Client()
{
	delete _rest_client;
	delete _ws_client;
};

//  ------------------------------ End | Client General methods - Infrastructure

//  ------------------------------ Start | Client CRTP methods - Infrastructure

template<typename T>
bool Client<T>::init_ws_session() { return static_cast<T*>(this)->v_init_ws_session(); }

template<typename T>
std::string Client<T>::_get_listen_key() { return static_cast<T*>(this)->v__get_listen_key(); }

template<typename T>
void Client<T>::close_stream(const std::string& symbol, const std::string& stream_name) { static_cast<T*>(this)->v_close_stream(symbol, stream_name); }

template<typename T>
bool Client<T>::is_stream_open(const std::string& symbol, const std::string& stream_name) { return static_cast<T*>(this)->v_is_stream_open(symbol, stream_name); }

template<typename T>
std::vector<std::string> Client<T>::get_open_streams() { return static_cast<T*>(this)->v_get_open_streams(); }

template<typename T>
void Client<T>::ws_auto_reconnect(const bool& reconnect) { static_cast<T*>(this)->v_ws_auto_reconnect(reconnect); }

template<typename T>
void Client<T>::set_refresh_key_interval(const bool val) { static_cast<T*>(this)->v_set_refresh_key_interval(val); }

//  ------------------------------ End | Client CRTP methods - Infrastructure

//  ------------------------------ Start | Client CRTP methods - Market Data Endpoints 

template<typename T>
unsigned long long Client<T>::exchange_time() { return static_cast<T*>(this)->v_exchange_time(); }

template<typename T>
bool Client<T>::ping_client() { return static_cast<T*>(this)->v_ping_client(); }

template<typename T>
Json::Value Client<T>::exchange_info() { return static_cast<T*>(this)->v_exchange_info(); }

template<typename T>
Json::Value Client<T>::order_book(Params* params_obj) { return static_cast<T*>(this)->v_order_book(params_obj); }

template<typename T>
Json::Value Client<T>::public_trades_recent(Params* params_obj) { return static_cast<T*>(this)->v_public_trades_recent(params_obj); }

template<typename T>
Json::Value Client<T>::public_trades_historical(Params* params_obj) { return static_cast<T*>(this)->v_public_trades_historical(params_obj); }

template<typename T>
Json::Value Client<T>::public_trades_agg(Params* params_obj) { return static_cast<T*>(this)->v_public_trades_agg(params_obj); }

template<typename T>
Json::Value Client<T>::klines(Params* params_obj) { return static_cast<T*>(this)->v_klines(params_obj); }

template<typename T>
Json::Value Client<T>::daily_ticker_stats(Params* params_obj) { return static_cast<T*>(this)->v_daily_ticker_stats(params_obj); }

template<typename T>
Json::Value Client<T>::get_ticker(Params* params_obj) { return static_cast<T*>(this)->v_get_ticker(params_obj); }

template<typename T>
Json::Value Client<T>::get_order_book_ticker(Params* params_obj) { return static_cast<T*>(this)->v_get_order_book_ticker(params_obj); }

//  ------------------------------ End | Client CRTP methods - Market Data Endpoints 


//  ------------------------------ Start | Client CRTP methods - Trade Endpoints

template<typename T>
Json::Value Client<T>::test_new_order(Params* params_obj) { return static_cast<T*>(this)->v_test_new_order(); }

template<typename T>
Json::Value Client<T>::new_order(Params* params_obj) { return static_cast<T*>(this)->v_new_order(params_obj); }

template<typename T>
Json::Value Client<T>::cancel_order(Params* params_obj) { return static_cast<T*>(this)->v_cancel_order(params_obj); }

template<typename T>
Json::Value Client<T>::cancel_all_orders(Params* params_obj) { return static_cast<T*>(this)->v_cancel_all_orders(params_obj); }

template<typename T>
Json::Value Client<T>::query_order(Params* params_obj) { return static_cast<T*>(this)->v_query_order(params_obj); }

template<typename T>
Json::Value Client<T>::open_orders(Params* params_obj) { return static_cast<T*>(this)->v_open_orders(params_obj); }

template<typename T>
Json::Value Client<T>::all_orders(Params* params_obj) { return static_cast<T*>(this)->v_all_orders(params_obj); }

template<typename T>
Json::Value Client<T>::account_info(Params* params_obj) { return static_cast<T*>(this)->v_account_info(params_obj); }

template<typename T>
Json::Value Client<T>::account_trades_list(Params* params_obj) { return static_cast<T*>(this)->v_account_trades_list(params_obj); }


//  ------------------------------ End | Client CRTP methods - Trade Endpoints

//  ------------------------------ Start | Client global + CRTP methods - WS Streams


template<typename T>
template <class FT>
unsigned int  Client<T>::stream_Trade(std::string symbol, std::string& buffer, FT& functor) { return static_cast<T*>(this)->v_stream_Trade(symbol, buffer, functor); }


template<typename T>
template <class FT>
unsigned int  Client<T>::stream_aggTrade(std::string symbol, std::string& buffer, FT& functor)
{
	std::string full_stream_name = "/ws/" + symbol + '@' + "aggTrade";
	if (this->_ws_client->is_open(full_stream_name))
	{
		std::cout << "already exists";
		return 0;
	}
	else
	{
		this->_ws_client->_stream_manager<FT>(full_stream_name, buffer, functor);
		return this->_ws_client->running_streams[full_stream_name];
	}
}


template<typename T>
template <class FT>
unsigned int Client<T>::stream_kline(std::string symbol, std::string& buffer, FT& functor, std::string interval)
{
	std::string full_stream_name = "/ws/" + symbol + '@' + "kline_" + interval;
	if (this->_ws_client->is_open(full_stream_name))
	{
		std::cout << "already exists";
		return 0;
	}
	else
	{
		this->_ws_client->_stream_manager<FT>(full_stream_name, buffer, functor);
		return this->_ws_client->running_streams[full_stream_name];
	}
}

template<typename T>
template <class FT>
unsigned int Client<T>::stream_ticker_ind_mini(std::string symbol, std::string& buffer, FT& functor)
{
	std::string full_stream_name = "/ws/" + symbol + '@' + "miniTicker";
	if (this->_ws_client->is_open(full_stream_name))
	{
		std::cout << "already exists";
		return 0;
	}
	else
	{
		this->_ws_client->_stream_manager<FT>(full_stream_name, buffer, functor);
		return this->_ws_client->running_streams[full_stream_name];
	}
}

template<typename T>
template <class FT>
unsigned int Client<T>::stream_ticker_all_mini(std::string& buffer, FT& functor)
{
	std::string full_stream_name = "/ws/!miniTicker@arr";
	if (this->_ws_client->is_open(full_stream_name))
	{
		std::cout << "already exists";
		return 0;
	}
	else
	{
		this->_ws_client->_stream_manager<FT>(full_stream_name, buffer, functor);
		return this->_ws_client->running_streams[full_stream_name];
	}
}

template<typename T>
template <class FT>
unsigned int Client<T>::stream_ticker_ind(std::string symbol, std::string& buffer, FT& functor)
{
	std::string full_stream_name = "/ws/" + symbol + "@" + "ticker";
	if (this->_ws_client->is_open(full_stream_name))
	{
		std::cout << "already exists";
		return 0;
	}
	else
	{
		this->_ws_client->_stream_manager<FT>(full_stream_name, buffer, functor);
		return this->_ws_client->running_streams[full_stream_name];
	}
}

template<typename T>
template <class FT>
unsigned int Client<T>::stream_ticker_all(std::string& buffer, FT& functor)
{
	std::string full_stream_name = "/ws/!ticker@arr";
	if (this->_ws_client->is_open(full_stream_name))
	{
		std::cout << "already exists";
		return 0;
	}
	else
	{
		this->_ws_client->_stream_manager<FT>(full_stream_name, buffer, functor);
		return this->_ws_client->running_streams[full_stream_name];
	}
}

template<typename T>
template <class FT>
unsigned int Client<T>::stream_ticker_ind_book(std::string symbol, std::string& buffer, FT& functor)
{
	std::string full_stream_name = "/ws/" + symbol + "@" + "bookTicker";
	if (this->_ws_client->is_open(full_stream_name))
	{
		std::cout << "already exists";
		return 0;
	}
	else
	{
		this->_ws_client->_stream_manager<FT>(full_stream_name, buffer, functor);
		return this->_ws_client->running_streams[full_stream_name];
	}
}

template<typename T>
template <class FT>
unsigned int Client<T>::stream_ticker_all_book(std::string& buffer, FT& functor)
{
	std::string full_stream_name = "/ws/!bookTicker";
	if (this->_ws_client->is_open(full_stream_name))
	{
		std::cout << "already exists";
		return 0;
	}
	else
	{
		this->_ws_client->_stream_manager<FT>(full_stream_name, buffer, functor);
		return this->_ws_client->running_streams[full_stream_name];
	}
}

template<typename T>
template <class FT>
unsigned int Client<T>::stream_depth_partial(std::string symbol, std::string& buffer, FT& functor, unsigned int levels, unsigned int interval)
{
	std::string full_stream_name = "/ws/" + symbol + "@" + "depth" + std::to_string(levels) + "@" + std::to_string(interval) + "ms";
	if (this->_ws_client->is_open(full_stream_name))
	{
		std::cout << "already exists";
		return 0;
	}
	else
	{
		this->_ws_client->_stream_manager<FT>(full_stream_name, buffer, functor);
		return this->_ws_client->running_streams[full_stream_name];
	}
}

template<typename T>
template <class FT>
unsigned int Client<T>::stream_depth_diff(std::string symbol, std::string& buffer, FT& functor, unsigned int interval)
{
	std::string full_stream_name = "/ws/" + symbol + '@' + "depth" + "@" + std::to_string(interval) + "ms";
	if (this->_ws_client->is_open(full_stream_name))
	{
		std::cout << "already exists";
		return 0;
	}
	else
	{
		this->_ws_client->_stream_manager<FT>(full_stream_name, buffer, functor);
		return this->_ws_client->running_streams[full_stream_name];
	}
}


template<typename T>
template <class FT>
unsigned int Client<T>::stream_userStream(std::string& buffer, FT& functor)
{
	RestSession* keep_alive_session = new RestSession{};
	try
	{
		this->set_headers(keep_alive_session);
		std::string full_stream_name = "/ws/" + this->_get_listen_key();

		std::string renew_key_path = this->_BASE_REST_SPOT + "/api/v3/userDataStream" + "?" + "listenKey=" + full_stream_name;

		std::pair<RestSession*, std::string> user_stream_pair = std::make_pair(keep_alive_session, renew_key_path);

		if (this->_ws_client->is_open(full_stream_name))
		{
			std::cout << "already exists";
			return 0;
		}
		else
		{
			this->_ws_client->_stream_manager<FT>(full_stream_name, buffer, functor, user_stream_pair);
			return this->_ws_client->running_streams[full_stream_name];
		}
	}
	catch (...)
	{
		delete keep_alive_session;
		throw("bad_stream");
	}
}


//  ------------------------------ End | Client Global + CRTP methods - WS Streams


//  ------------------------------ Start | Client General methods - Infrastructure


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
		delete this->_rest_client;
		throw("bad_init_rest");
	}

}

template <typename T>
Json::Value Client<T>::custom_get_req(const std::string& base, const std::string& endpoint, Params* params_obj, bool signature)
{
	std::string query = this->_generate_query(*params_obj, signature);
	std::string full_path = base + endpoint + query;
	return this->_rest_client->_getreq(full_path);
}

template <typename T>
Json::Value Client<T>::custom_post_req(const std::string& base, const std::string& endpoint, Params* params_obj, bool signature)
{
	std::string query = this->_generate_query(*params_obj, signature);
	std::string full_path = base + endpoint + query;
	return this->_rest_client->_postreq(full_path);
}

template <typename T>
Json::Value Client<T>::custom_put_req(const std::string& base, const std::string& endpoint, Params* params_obj, bool signature) 
{
	std::string query = this->_generate_query(*params_obj, signature);
	std::string full_path = base + endpoint + query;
	return this->_rest_client->_putreq(full_path);
}

template <typename T>
Json::Value Client<T>::custom_delete_req(const std::string& base, const std::string& endpoint, Params* params_obj, bool signature)
{
	std::string query = this->_generate_query(*params_obj, signature);
	std::string full_path = base + endpoint + query;
	return this->_rest_client->_deletereq(full_path);
}

template <typename T>
bool Client<T>::set_headers(RestSession* rest_client)
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

template <typename T>
void Client<T>::rest_set_verbose(bool state)
{
	if (state) this->_rest_client->set_verbose(1L);
	else this->_rest_client->set_verbose(0);
}

template <typename T>
template <typename FT>
unsigned int Client<T>::custom_stream(std::string stream_query, std::string buffer, FT functor)
{
	stream_query = "/stream?streams=" + stream_query;
	if (this->_ws_client->is_open(stream_query))
	{
		std::cout << "already exists";
		return 0;
	}
	else
	{
		this->_ws_client->_stream_manager<FT>(stream_query, buffer, functor);
		return this->_ws_client->running_streams[stream_query];
	}
}

template <typename T>
std::string Client<T>::_generate_query(Params& params_obj, bool sign_query)
{
	params_obj.set_param<unsigned long long>("timestamp", local_timestamp());
	std::unordered_map<std::string, std::string> params = params_obj.param_map;
	std::string query;

	for (std::unordered_map<std::string, std::string>::iterator itr = params.begin();
		itr != params.end();
		itr++)
	{
		if (itr != params.begin()) query += "&";

		query += (itr->first + "=" + itr->second);
	}

	if (sign_query)
	{
		std::string signature = HMACsha256(query, this->_api_secret);
		query += "&signature=" + signature;
		params_obj.delete_param("timestamp");
	}
	if (params_obj.flush_params) params_obj.clear_params();

	query = "?" + query;

	return query;
}

template <typename T>
bool Client<T>::exchange_status() // todo: is this abstract?
{
	std::string full_path = this->_BASE_REST_SPOT + "/wapi/v3/systemStatus.html";
	return this->_rest_client->_getreq(full_path)["response"]["status"].asBool();
}


//  ------------------------------ End | Client General methods - Infrastructure


// ***************************************************************************


//  ------------------------------ Start | Client Wallet - User Wallet Endpoints

// ------ Class methods

template <typename T>
Client<T>::Wallet::Wallet(Client<T>& client_obj)
	: user_client{ &client_obj } // snatching pointer and releasing later on to avoid deleting this reference
{
	if (user_client->_public_client) throw("public client");
}

template <typename T>
Client<T>::Wallet::Wallet(const Client<T>& client_obj)
	: user_client{ &client_obj } 
{
	if (user_client->_public_client) throw("public client");
}

template <typename T>
Client<T>::Wallet::~Wallet()
{
	user_client = nullptr;
}

// ------ Endpoint methods


template <typename T>
Json::Value Client<T>::Wallet::get_all_coins(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}

	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/capital/config/getall";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
}; 

template <typename T>
Json::Value Client<T>::Wallet::daily_snapshot(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/accountSnapshot";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::Wallet::fast_withdraw_switch(bool state)
{
	Params temp_params;
	std::string endpoint = state ? "/sapi/v1/account/enableFastWithdrawSwitch" : "/sapi/v1/account/disableFastWithdrawSwitch";
	std::string full_path = user_client->_BASE_REST_SPOT + endpoint;
	std::string query = user_client->_generate_query(temp_params, 1);
	Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

	return response;
}; 

template <typename T>
Json::Value Client<T>::Wallet::withdraw_balances(Params* params_obj, bool SAPI)
{
	std::string endpoint = SAPI ? "/sapi/v1/capital/withdraw/apply" : "/wapi/v3/withdraw.html";
	std::string full_path = user_client->_BASE_REST_SPOT + endpoint;
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::Wallet::deposit_history(Params* params_obj, bool network)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string endpoint = network ? "/sapi/v1/capital/deposit/hisrec" : "/wapi/v3/depositHistory.html";
	std::string full_path = user_client->_BASE_REST_SPOT + endpoint;
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
}; 

template <typename T>
Json::Value Client<T>::Wallet::withdraw_history(Params* params_obj, bool network) 
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string endpoint = network ? "/sapi/v1/capital/withdraw/history" : "/wapi/v3/withdrawHistory.html";
	std::string full_path = user_client->_BASE_REST_SPOT + endpoint;
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::Wallet::deposit_address(Params* params_obj, bool network)
{
	std::string endpoint = network ? "/sapi/v1/capital/deposit/address" : "/wapi/v3/depositAddress.html";
	std::string full_path = user_client->_BASE_REST_SPOT + endpoint;
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
}; 

template <typename T>
Json::Value Client<T>::Wallet::account_status(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/wapi/v3/accountStatus.html";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};  

template <typename T>
Json::Value Client<T>::Wallet::account_status_api(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string full_path = user_client->_BASE_REST_SPOT + "/wapi/v3/apiTradingStatus.html";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
}; 

template <typename T>
Json::Value Client<T>::Wallet::dust_log(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string full_path = user_client->_BASE_REST_SPOT + "/wapi/v3/userAssetDribbletLog.html";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::Wallet::dust_transfer(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/asset/dust";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::Wallet::asset_dividend_records(Params* params_obj) 
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/asset/assetDividend";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

	return response;
};  

template <typename T>
Json::Value Client<T>::Wallet::asset_details(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string full_path = user_client->_BASE_REST_SPOT + "/wapi/v3/assetDetail.html";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
}; 

template <typename T>
Json::Value Client<T>::Wallet::trading_fees(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string full_path = user_client->_BASE_REST_SPOT + "/wapi/v3/tradeFee.html";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
}; 

//  ------------------------------ End | Client Wallet - User Wallet Endpoints

// ***************************************************************************


//  ------------------------------ Start | Client FuturesWallet - User FuturesWallet Endpoints

// ------ Class methods

template <typename T>
Client<T>::FuturesWallet::FuturesWallet(Client<T>& client_obj)
	: user_client{ &client_obj } 
{
	if (user_client->_public_client) throw("public client");
}

template <typename T>
Client<T>::FuturesWallet::FuturesWallet(const Client<T>& client_obj)
	: user_client{ &client_obj }
{
	if (user_client->_public_client) throw("public client");
}

template <typename T>
Client<T>::FuturesWallet::~FuturesWallet()
{
	user_client = nullptr;
}

// ------ Endpoint methods


template <typename T>
Json::Value Client<T>::FuturesWallet::futures_transfer(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = this->_BASE_REST_SPOT + ("/sapi/v1/futures/transfer" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response;
}

template <typename T>
Json::Value Client<T>::FuturesWallet::futures_transfer_history(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = this->_BASE_REST_SPOT + ("/sapi/v1/futures/transfer" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_borrow(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = this->_BASE_REST_SPOT + ("/sapi/v1/futures/loan/borrow" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response;
}

template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_borrow_history(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = this->_BASE_REST_SPOT + ("/sapi/v1/futures/loan/borrow/history" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_repay(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = this->_BASE_REST_SPOT + ("/sapi/v1/futures/loan/repay" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response;
}

template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_repay_history(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = this->_BASE_REST_SPOT + ("/sapi/v1/futures/loan/repay/history" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_wallet(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = this->_BASE_REST_SPOT + ("/sapi/v1/futures/loan/wallet" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_info(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = this->_BASE_REST_SPOT + ("/sapi/v1/futures/loan/configs" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_adjust_calc_rate(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = this->_BASE_REST_SPOT + ("/sapi/v1/futures/loan/calcAdjustLevel" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_adjust_get_max(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = this->_BASE_REST_SPOT + ("/sapi/v1/futures/loan/calcMaxAdjustAmount" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_adjust(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = this->_BASE_REST_SPOT + ("/sapi/v1/futures/loan/adjustCollateral" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response;
}

template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_adjust_history(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = this->_BASE_REST_SPOT + ("/sapi/v1/futures/loan/adjustCollateral/history" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

template <typename T>
Json::Value Client<T>::FuturesWallet::collateral_liquidation_history(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = this->_BASE_REST_SPOT + ("/sapi/v1/futures/loan/liquidationHistory" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

//  ------------------------------ End | Client FuturesWallet - User FuturesWallet Endpoints


// ***************************************************************************


//  ------------------------------ Start | Client SubAccount - User SubAccount Endpoints

// ------ Class methods

template <typename T>
Client<T>::SubAccount::SubAccount(Client<T>& client_obj)
	: user_client{ &client_obj } // snatching pointer and releasing later on to avoid deleting this reference
{
	if (user_client->_public_client) throw("public client");
}

template <typename T>
Client<T>::SubAccount::SubAccount(const Client<T>& client_obj)
	: user_client{ &client_obj }
{
	if (user_client->_public_client) throw("public client");
}

template <typename T>
Client<T>::SubAccount::~SubAccount()
{
	user_client = nullptr;
}

// ------ Endpoint methods

template <typename T>
Json::Value Client<T>::SubAccount::get_all_subaccounts(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string full_path = user_client->_BASE_REST_SPOT + "/wapi/v3/sub-account/list.html";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::SubAccount::transfer_master_history(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/wapi/v3/sub-account/transfer/history.html";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::SubAccount::transfer_master_to_subaccount(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/wapi/v3/sub-account/transfer.html";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::SubAccount::get_subaccount_balances(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/wapi/v3/sub-account/assets.html";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::SubAccount::get_subaccount_deposit_address(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/capital/deposit/subAddress";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::SubAccount::get_subaccount_deposit_history(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/capital/deposit/subHisrec";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::SubAccount::get_subaccount_future_margin_status(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/sub-account/status";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::SubAccount::enable_subaccount_margin(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/sub-account/margin/enable";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::SubAccount::get_subaccount_margin_status(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/sub-account/margin/account";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::SubAccount::get_subaccount_margin_summary(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/sub-account/margin/accountSummary";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::SubAccount::enable_subaccount_futures(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/sub-account/futures/enable";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::SubAccount::get_subaccount_futures_status(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/sub-account/futures/account";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::SubAccount::get_subaccount_futures_summary(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/sub-account/futures/accountSummary";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::SubAccount::get_subaccount_futures_positionrisk(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/sub-account/futures/positionRisk";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::SubAccount::transfer_to_subaccount_futures(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/sub-account/futures/transfer";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::SubAccount::transfer_to_subaccount_margin(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/sub-account/margin/transfer";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::SubAccount::transfer_subaccount_to_subaccount(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/sub-account/transfer/subToSub";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::SubAccount::transfer_subaccount_to_master(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/sub-account/transfer/subToMaster";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::SubAccount::transfer_subaccount_history(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/sub-account/transfer/subUserHistory";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

//  ------------------------------ End | Client SubAccount - User SubAccount Endpoints

// ***************************************************************************

//  ------------------------------ Start | Client MarginAccount - User MarginAccount Endpoints

template <typename T>
Client<T>::MarginAccount::MarginAccount(Client<T>& client_obj)
	: user_client{ &client_obj } // snatching pointer and releasing later on to avoid deleting this reference
{
	if (user_client->_public_client) throw("public client");
}

template <typename T>
Client<T>::MarginAccount::MarginAccount(const Client<T>& client_obj)
	: user_client{ &client_obj }
{
	if (user_client->_public_client) throw("public client");
}

template <typename T>
Client<T>::MarginAccount::~MarginAccount()
{
	user_client = nullptr;
}

// ------ Endpoint methods


template <typename T>
Json::Value Client<T>::MarginAccount::margin_transfer(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/margin/transfer";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::MarginAccount::margin_borrow(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/margin/loan";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::MarginAccount::margin_repay(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/margin/repay";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::MarginAccount::margin_asset_query(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/margin/asset";
	std::string query = params_obj ? this->_generate_query(*params_obj) : "";
	Json::Value response = (user_client->_rest_client)->_getreq(full_path);

	return response;
};

template <typename T>
Json::Value Client<T>::MarginAccount::margin_pair_query(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/margin/pair";
	std::string query = params_obj ? this->_generate_query(*params_obj) : "";
	Json::Value response = (user_client->_rest_client)->_getreq(full_path);

	return response;
};

template <typename T>
Json::Value Client<T>::MarginAccount::margin_all_assets_query()
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/margin/allAssets";
	Json::Value response = (user_client->_rest_client)->_getreq(full_path);

	return response;
};

template <typename T>
Json::Value Client<T>::MarginAccount::margin_all_pairs_query()
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/margin/allPairs";
	Json::Value response = (user_client->_rest_client)->_getreq(full_path);

	return response;
};

template <typename T>
Json::Value Client<T>::MarginAccount::margin_price_index(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/margin/transfer";
	std::string query = params_obj ? this->_generate_query(*params_obj) : "";
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::MarginAccount::margin_new_order(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/margin/order";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::MarginAccount::margin_cancel_order(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/margin/order";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_deletereq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::MarginAccount::margin_transfer_history(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}

	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/margin/transfer";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::MarginAccount::margin_loan_record(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/margin/loan";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::MarginAccount::margin_repay_record(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/margin/repay";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::MarginAccount::margin_interest_history(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}

	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/margin/interestHistory";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::MarginAccount::margin_liquidations_record(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}

	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/margin/forceLiquidationRec";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::MarginAccount::margin_account_info(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}

	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/margin/account";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::MarginAccount::margin_account_order(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/margin/order";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::MarginAccount::margin_account_open_orders(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}

	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/margin/openOrders";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::MarginAccount::margin_account_all_orders(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/margin/allOrders";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::MarginAccount::margin_account_trades_list(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/margin/myTrades";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::MarginAccount::margin_max_borrow(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/margin/maxBorrowable";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::MarginAccount::margin_max_transfer(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/margin/maxTransferable";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::MarginAccount::margin_isolated_margin_create(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/margin/isolated/create";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::MarginAccount::margin_isolated_margin_transfer(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/margin/isolated/transfer";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::MarginAccount::margin_isolated_margin_transfer_history(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/margin/isolated/transfer";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::MarginAccount::margin_isolated_margin_account_info(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}

	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/margin/isolated/account";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::MarginAccount::margin_isolated_margin_symbol(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/margin/isolated/pair";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::MarginAccount::margin_isolated_margin_symbol_all(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}

	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/margin/isolated/allPairs";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

//  ------------------------------ End | Client MarginAccount - User MarginAccount Endpoints

// ***************************************************************************

//  ------------------------------ Start | Client Savings - User Savings Endpoints

template <typename T>
Client<T>::Savings::Savings(Client<T>& client_obj)
	: user_client{ &client_obj } // snatching pointer and releasing later on to avoid deleting this reference
{
	if (user_client->_public_client) throw("public client");
}

template <typename T>
Client<T>::Savings::Savings(const Client<T>& client_obj)
	: user_client{ &client_obj }
{
	if (user_client->_public_client) throw("public client");
}

template <typename T>
Client<T>::Savings::~Savings()
{
	user_client = nullptr;
}

// ------ Endpoint methods


template <typename T>
Json::Value Client<T>::Savings::get_product_list_flexible(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/lending/daily/product/list";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::Savings::get_product_daily_quota_purchase_flexible(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/lending/daily/userLeftQuota";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::Savings::purchase_product_flexible(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/lending/daily/purchase";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::Savings::get_product_daily_quota_redemption_flexible(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/lending/daily/userRedemptionQuota";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::Savings::redeem_product_flexible(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/lending/daily/redeem";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::Savings::get_product_position_flexible(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/lending/daily/token/position";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::Savings::get_product_list_fixed(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/lending/project/list";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::Savings::purchase_product_fixed(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/lending/customizedFixed/purchase";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_postreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::Savings::get_product_position_fixed(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/lending/project/position/list";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::Savings::lending_account(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/lending/union/account";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::Savings::get_purchase_record(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/lending/union/purchaseRecord";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::Savings::get_redemption_record(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/lending/union/redemptionRecord";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::Savings::get_interest_history(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/lending/union/interestHistory";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

//  ------------------------------ End | Client Savings - User Savings Endpoints


// ***************************************************************************

//  ------------------------------ Start | Client Mining - User Mining Endpoints

template <typename T>
Client<T>::Mining::Mining(Client<T>& client_obj)
	: user_client{ &client_obj } // snatching pointer and releasing later on to avoid deleting this reference
{
	if (user_client->_public_client) throw("public client");
}

template <typename T>
Client<T>::Mining::Mining(const Client<T>& client_obj)
	: user_client{ &client_obj }
{
	if (user_client->_public_client) throw("public client");
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

	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/mining/pub/algoList";
	Json::Value response = (user_client->_rest_client)->_getreq(full_path);

	return response;
};

template <typename T>
Json::Value Client<T>::Mining::coin_list()
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/mining/pub/coinList";
	Json::Value response = (user_client->_rest_client)->_getreq(full_path);

	return response;
};

template <typename T>
Json::Value Client<T>::Mining::get_miner_list_detail(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/mining/worker/detail";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::Mining::get_miner_list(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/mining/worker/list";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::Mining::revenue_list(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/mining/payment/list";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::Mining::statistic_list(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/mining/statistics/user/status";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};

template <typename T>
Json::Value Client<T>::Mining::account_list(Params* params_obj)
{
	std::string full_path = user_client->_BASE_REST_SPOT + "/sapi/v1/mining/statistics/user/list";
	std::string query = user_client->_generate_query(*params_obj, 1);
	Json::Value response = (user_client->_rest_client)->_getreq(full_path + query);

	return response;
};


//  ------------------------------ End | Client Mining - User Mining Endpoints


// =======================================================================================================


//  ------------------------------ Start | SpotClient General methods - Infrastructure

SpotClient::SpotClient() : Client()
{
	this->init_ws_session();
	this->init_rest_session();
};

SpotClient::SpotClient(std::string key, std::string secret)
	: Client(key, secret)
{
	this->init_rest_session();
	this->init_ws_session();
}


SpotClient::~SpotClient() // todo: is delete restand ws client needed ? ? ?
{};

//  ------------------------------ End | SpotClient General methods - Infrastructure

//  ------------------------------ Start | SpotClient CRTP methods - Client infrastructure

bool SpotClient::v_init_ws_session()
{
	try
	{
		if (this->_ws_client) delete this->_ws_client;
		this->_ws_client = new WebsocketClient{ this->_WS_BASE_SPOT, this->_WS_PORT_SPOT };
		return 1;
	}
	catch (...)
	{
		throw("bad_init_ws");
	}
}

std::string SpotClient::v__get_listen_key()
{
	// no signature is needed here
	std::string full_path = this->_BASE_REST_SPOT + "/api/v3/userDataStream";
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response["response"]["listenKey"].asString();
}


void SpotClient::v_close_stream(const std::string& symbol, const std::string& stream_name)
{
	try
	{
		this->_ws_client->close_stream(symbol + "@" + stream_name);
	}
	catch (...)
	{
		throw("stream_close_exc");
	}
}
void SpotClient::v_set_refresh_key_interval(const bool val)
{
	this->_ws_client->refresh_listenkey_interval = val;
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

void SpotClient::v_ws_auto_reconnect(const bool& reconnect)
{
	this->_ws_client->_set_reconnect(reconnect);
}

//  ------------------------------ End | SpotClient CRTP methods - Client infrastructure

//  ------------------------------ Start | SpotClient CRTP methods - Market Data Implementations

bool SpotClient::v_ping_client()
{
	try
	{
		std::string full_path = this->_BASE_REST_SPOT + "/api/v3/ping";
		Json::Value ping_response = (this->_rest_client)->_getreq(full_path)["response"];
		return (ping_response != Json::nullValue);
	}
	catch (...)
	{
		throw("bad_ping");
	}
}

unsigned long long SpotClient::v_exchange_time()
{
	std::string full_path = this->_BASE_REST_SPOT + "/api/v3/time";
	std::string ex_time = (this->_rest_client)->_getreq(full_path)["response"]["serverTime"].asString();

	return std::atoll(ex_time.c_str());
}
Json::Value SpotClient::v_exchange_info() // todo: define
{
	std::string full_path = this->_BASE_REST_SPOT + "/api/v1/exchangeInfo";
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value SpotClient::v_order_book(Params* params_obj)
{
	std::string query = params_obj ? this->_generate_query(*params_obj) : "";
	std::string full_path = this->_BASE_REST_SPOT + "/api/v3/depth" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value SpotClient::v_public_trades_recent(Params* params_obj)
{
	std::string query = params_obj ? this->_generate_query(*params_obj) : "";
	std::string full_path = this->_BASE_REST_SPOT + "/api/v3/trades" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value SpotClient::v_public_trades_historical(Params* params_obj)
{
	std::string query = params_obj ? this->_generate_query(*params_obj) : "";
	std::string full_path = this->_BASE_REST_SPOT + "/api/v3/historicalTrades" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value SpotClient::v_public_trades_agg(Params* params_obj)
{
	std::string query = params_obj ? this->_generate_query(*params_obj) : "";
	std::string full_path = this->_BASE_REST_SPOT + "/api/v3/aggTrades" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value SpotClient::v_klines(Params* params_obj)
{
	std::string query = params_obj ? this->_generate_query(*params_obj) : "";
	std::string full_path = this->_BASE_REST_SPOT + "/api/v3/klines" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value SpotClient::v_daily_ticker_stats(Params* params_obj)
{
	std::string query = params_obj ? this->_generate_query(*params_obj) : "";
	std::string full_path = this->_BASE_REST_SPOT + "/api/v3/ticker/24hr" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value SpotClient::v_get_ticker(Params* params_obj)
{
	std::string query = params_obj ? this->_generate_query(*params_obj) : "";
	std::string full_path = this->_BASE_REST_SPOT + "/api/v3/ticker/price" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value SpotClient::v_get_order_book_ticker(Params* params_obj)
{
	std::string query = params_obj ? this->_generate_query(*params_obj) : "";
	std::string full_path = this->_BASE_REST_SPOT + "/api/v3/ticker/bookTicker" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

//  ------------------------------ End | SpotClient CRTP methods - Market Data Implementations


//  ------------------------------ Start | SpotClient CRTP methods - Trade Implementations


// -- Up to 'Client' Level

Json::Value SpotClient::v_test_new_order(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string full_path = this->_BASE_REST_SPOT + "/api/v3/order/test";
	std::string query = this->_generate_query(*params_obj, 1);
	Json::Value response = (this->_rest_client)->_postreq(full_path + query);

	return response;
}

Json::Value SpotClient::v_new_order(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string full_path = this->_BASE_REST_SPOT + "/api/v3/order";
	std::string query = this->_generate_query(*params_obj, 1);
	Json::Value response = (this->_rest_client)->_postreq(full_path + query);

	return response;
}

Json::Value SpotClient::v_cancel_order(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string full_path = this->_BASE_REST_SPOT + "/api/v3/order";
	std::string query = this->_generate_query(*params_obj, 1);
	Json::Value response = (this->_rest_client)->_deletereq(full_path + query);

	return response;
}

Json::Value SpotClient::v_cancel_all_orders(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string full_path = this->_BASE_REST_SPOT + "api/v3/openOrders";
	std::string query = this->_generate_query(*params_obj, 1);
	Json::Value response = (this->_rest_client)->_deletereq(full_path + query);

	return response;
}

Json::Value SpotClient::v_query_order(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string full_path = this->_BASE_REST_SPOT + "/api/v3/order";
	std::string query = this->_generate_query(*params_obj, 1);
	Json::Value response = (this->_rest_client)->_getreq(full_path + query);

	return response;
}

Json::Value SpotClient::v_open_orders(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string full_path = this->_BASE_REST_SPOT + "/api/v3/openOrders";
	std::string query = this->_generate_query(*params_obj, 1);
	Json::Value response = (this->_rest_client)->_getreq(full_path + query);

	return response;
}

Json::Value SpotClient::v_all_orders(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string full_path = this->_BASE_REST_SPOT + "/api/v3/allOrders";
	std::string query = this->_generate_query(*params_obj, 1);
	Json::Value response = (this->_rest_client)->_getreq(full_path + query);

	return response;
}

Json::Value SpotClient::v_account_info(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string full_path = this->_BASE_REST_SPOT + "/api/v3/account";
	std::string query = this->_generate_query(*params_obj, 1);
	Json::Value response = (this->_rest_client)->_getreq(full_path + query);

	return response;
}

Json::Value SpotClient::v_account_trades_list(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string full_path = this->_BASE_REST_SPOT + "/api/v3/myTrades";
	std::string query = this->_generate_query(*params_obj, 1);
	Json::Value response = (this->_rest_client)->_getreq(full_path + query);

	return response;
}

//  ------------------------------ End | SpotClient CRTP methods - Trade Implementations

//  ------------------------------ Start | SpotClient General methods - Trade Implementations 

Json::Value SpotClient::oco_new_order(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string full_path = this->_BASE_REST_SPOT + "/api/v3/order/oco";
	std::string query = this->_generate_query(*params_obj, 1);
	Json::Value response = (this->_rest_client)->_postreq(full_path + query);

	return response;
}

Json::Value SpotClient::oco_cancel_order(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string full_path = this->_BASE_REST_SPOT + "/api/v3/orderList";
	std::string query = this->_generate_query(*params_obj, 1);
	Json::Value response = (this->_rest_client)->_deletereq(full_path + query);

	return response;
}

Json::Value SpotClient::oco_query_order(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string full_path = this->_BASE_REST_SPOT + "/api/v3/orderList";
	std::string query = this->_generate_query(*params_obj, 1);
	Json::Value response = (this->_rest_client)->_getreq(full_path + query);

	return response;
}

Json::Value SpotClient::oco_all_orders(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string full_path = this->_BASE_REST_SPOT + "/api/v3/allOrderList";
	std::string query = this->_generate_query(*params_obj, 1);
	Json::Value response = (this->_rest_client)->_getreq(full_path + query);

	return response;
}

Json::Value SpotClient::oco_open_orders(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string full_path = this->_BASE_REST_SPOT + "/api/v3/openOrderList";
	std::string query = this->_generate_query(*params_obj, 1);
	Json::Value response = (this->_rest_client)->_getreq(full_path + query);

	return response;
}

//  ------------------------------ End | SpotClient General methods - Trade Implementations 


//  ------------------------------ Start | SpotClient General methods - WS Streams

template <class FT>
unsigned int SpotClient::v_stream_Trade(std::string symbol, std::string& buffer, FT& functor)
{
	std::string full_stream_name = "/ws/" + symbol + '@' + "trade";
	if (this->_ws_client->is_open(full_stream_name))
	{
		std::cout << "already exists";
		return 0;
	}
	else
	{
		this->_ws_client->_stream_manager<FT>(full_stream_name, buffer, functor);
		return this->_ws_client->running_streams[full_stream_name];
	}
}

//  ------------------------------ End | SpotClient General methods - WS Streams


// =======================================================================================================


//  ------------------------------ Start | FuturesClient General methods - Infrastructure

template <typename CT>
FuturesClient<CT>::FuturesClient()
	: Client<FuturesClient<CT>>(), _testnet_mode{ 0 }
{
	this->init_ws_session();
	this->init_rest_session();
};

template <typename CT>
FuturesClient<CT>::FuturesClient(std::string key, std::string secret)
	: Client<FuturesClient<CT>>(key, secret), _testnet_mode{ 0 }
{
	this->init_rest_session();
	this->init_ws_session();
}

template <typename CT> 
FuturesClient<CT>::~FuturesClient()
{}

template <typename CT>
void FuturesClient<CT>::set_testnet_mode(bool status)
{
	this->_testnet_mode = status;
}

template <typename CT>
bool FuturesClient<CT>::get_testnet_mode()
{
	return this->_testnet_mode;
}

//  ------------------------------ End | FuturesClient General methods - Infrastructure


//  ------------------------------ Start | FuturesClient CRTP methods - Client infrastructure

template <typename CT>
bool FuturesClient<CT>::v_init_ws_session() { return static_cast<CT*>(this)->v__init_ws_session(); }


template <typename CT>
std::string FuturesClient<CT>::v__get_listen_key()
{
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += "/fapi/v1/listenKey";
	Params temp_params;
	std::string query = Client::_generate_query(temp_params, 1);

	Json::Value response = (this->_rest_client)->_postreq(full_path + query);

	return response["response"]["listenKey"].asString();
}


template <typename CT>
void FuturesClient<CT>::v_close_stream(const std::string& symbol, const std::string& stream_name)
{
	try
	{
		this->_ws_client->close_stream(symbol + "@" + stream_name);
	}
	catch (...)
	{
		throw("stream_close_exc");
	}
}

template <typename CT>
void FuturesClient<CT>::v_set_refresh_key_interval(const bool val)
{
	this->_ws_client->refresh_listenkey_interval = val;
}

template <typename CT>
bool FuturesClient<CT>::v_is_stream_open(const std::string& symbol, const std::string& stream_name)
{
	std::string full_stream_name = symbol + '@' + stream_name;
	return this->_ws_client->is_open(full_stream_name);
}

template <typename CT>
void FuturesClient<CT>::v_ws_auto_reconnect(const bool& reconnect)
{
	this->_ws_client->_set_reconnect(reconnect);
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
Json::Value FuturesClient<CT>::v_order_book(Params* params_obj) { return static_cast<CT*>(this)->v__order_book(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::v_public_trades_recent(Params* params_obj) { return static_cast<CT*>(this)->v__public_trades_recent(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::v_public_trades_historical(Params* params_obj) { return static_cast<CT*>(this)->v__public_trades_historical(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::v_public_trades_agg(Params* params_obj) { return static_cast<CT*>(this)->v__public_trades_agg(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::v_klines(Params* params_obj) { return static_cast<CT*>(this)->v__klines(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::v_daily_ticker_stats(Params* params_obj) { return static_cast<CT*>(this)->v_daily__ticker_stats(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::v_get_ticker(Params* params_obj) { return static_cast<CT*>(this)->v__get_ticker(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::v_get_order_book_ticker(Params* params_obj) { return static_cast<CT*>(this)->v__get_order_book_ticker(params_obj); }

//  ------------------------------ End | FuturesClient CRTP methods - Market Data Implementations


//  ------------------------------ Start | FuturesClient CRTP methods - Unique Endpoints

template<typename CT>
Json::Value FuturesClient<CT>::mark_price(Params* params_obj) { return static_cast<CT*>(this)->v_mark_price(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::public_liquidation_orders(Params* params_obj) { return static_cast<CT*>(this)->v_public_liquidation_orders(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::open_interest(Params* params_obj) { return static_cast<CT*>(this)->v_open_interest(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::continues_klines(Params* params_obj) { return static_cast<CT*>(this)->v_continues_klines(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::index_klines(Params* params_obj) { return static_cast<CT*>(this)->v_index_klines(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::mark_klines(Params* params_obj) { return static_cast<CT*>(this)->v_get_ticker(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::funding_rate_history(Params* params_obj) { return static_cast<CT*>(this)->v_funding_rate_history(params_obj); }

//  ------------------------------ End | FuturesClient CRTP methods - Unique Endpoints


//  ------------------------------ Start | FuturesClient CRTP methods - Trade Implementations 

// -- Up to 'Client' Level

template<typename CT>
Json::Value FuturesClient<CT>::v_test_new_order(Params* params_obj) { throw("please use testnet instead!"); } // todo: exceptions

template<typename CT>
Json::Value FuturesClient<CT>::v_new_order(Params* params_obj) { return static_cast<CT*>(this)->v__new_order(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::v_cancel_order(Params* params_obj) { return static_cast<CT*>(this)->v__cancel_order(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::v_cancel_all_orders(Params* params_obj) { return static_cast<CT*>(this)->v__cancel_all_orders(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::v_query_order(Params* params_obj) { return static_cast<CT*>(this)->v__query_order(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::v_open_orders(Params* params_obj) { return static_cast<CT*>(this)->v__open_orders(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::v_all_orders(Params* params_obj) { return static_cast<CT*>(this)->v__all_orders(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::v_account_info(Params* params_obj) { return static_cast<CT*>(this)->v__account_info(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::v_account_trades_list(Params* params_obj) { return static_cast<CT*>(this)->v__account_trades_list(params_obj); }

// -- Up to 'FuturesClient' (this) Level

template<typename CT>
Json::Value FuturesClient<CT>::change_position_mode(Params* params_obj) { return static_cast<CT*>(this)->v_change_position_mode(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::get_position_mode(Params* params_obj) { return static_cast<CT*>(this)->v_get_position_mode(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::batch_orders(Params* params_obj) { return static_cast<CT*>(this)->v_batch_orders(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::cancel_batch_orders(Params* params_obj) { return static_cast<CT*>(this)->v_cancel_batch_orders(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::cancel_all_orders_timer(Params* params_obj) { return static_cast<CT*>(this)->v_cancel_all_orders_timer(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::query_open_order(Params* params_obj) { return static_cast<CT*>(this)->v_query_open_order(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::account_balances(Params* params_obj) { return static_cast<CT*>(this)->v_account_balances(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::change_leverage(Params* params_obj) { return static_cast<CT*>(this)->v_change_leverage(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::change_margin_type(Params* params_obj) { return static_cast<CT*>(this)->v_change_margin_type(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::change_position_margin(Params* params_obj) { return static_cast<CT*>(this)->v_change_position_margin(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::change_position_margin_history(Params* params_obj) { return static_cast<CT*>(this)->v_change_position_margin_history(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::position_info(Params* params_obj) { return static_cast<CT*>(this)->v_position_info(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::get_income_history(Params* params_obj) { return static_cast<CT*>(this)->v_get_income_history(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::get_leverage_bracket(Params* params_obj) { return static_cast<CT*>(this)->v_get_leverage_bracket(params_obj); }

template<typename CT>
Json::Value FuturesClient<CT>::pos_adl_quantile_est(Params* params_obj) { return static_cast<CT*>(this)->v_pos_adl_quantile_est(params_obj); }


//  ------------------------------ End | FuturesClient CRTP methods - Trade Implementations


//  ------------------------------ Start | FuturesClient Global + CRTP methods - WS Streams 


template <typename CT>
template <class FT>
unsigned int FuturesClient<CT>::v_stream_Trade(std::string symbol, std::string& buffer, FT& functor)
{
	throw("does not exist for futures");
}


template<typename CT>
template <class FT>
unsigned int FuturesClient<CT>::v_stream_markprice_all(std::string pair, std::string& buffer, FT& functor) { return static_cast<CT*>(this)->v__stream_markprice_all(pair, buffer, functor); }  // only USDT

template<typename CT>
template <class FT>
unsigned int FuturesClient<CT>::v_stream_indexprice(std::string pair, std::string& buffer, FT& functor, unsigned int interval) { return static_cast<CT*>(this)->v__stream_indexprice(pair, buffer, functor, interval); } // only Coin

template<typename CT>
template <class FT>
unsigned int FuturesClient<CT>::v_stream_markprice_by_pair(std::string& pair, std::string& buffer, FT& functor, unsigned int interval) { return static_cast<CT*>(this)->v__stream_markprice_by_pair(pair, buffer, functor, interval); } // only coin

template<typename CT>
template <class FT>
unsigned int FuturesClient<CT>::v_stream_kline_contract(std::string pair_and_type, std::string& buffer, FT& functor, std::string interval) { return static_cast<CT*>(this)->v__stream_kline_contract(pair_and_type, buffer, functor, interval); } // only coin

template<typename CT>
template <class FT>
unsigned int FuturesClient<CT>::v_stream_kline_index(std::string pair, std::string& buffer, FT& functor, std::string interval) { return static_cast<CT*>(this)->v__stream_kline_index(pair, buffer, functor, interval); } // only coin

template<typename CT>
template <class FT>
unsigned int FuturesClient<CT>::v_stream_kline_markprice(std::string symbol, std::string& buffer, FT& functor, std::string interval) { return static_cast<CT*>(this)->v__stream_kline_markprice(symbol, buffer, functor, interval); } // only coin


template<typename CT>
template <class FT>
unsigned int FuturesClient<CT>::stream_markprice(std::string symbol, std::string& buffer, FT& functor, unsigned int interval)
{
	std::string full_stream_name = "/ws/" + symbol + '@' + "markPrice" + std::to_string(interval) + "ms";
	if (this->_ws_client->is_open(full_stream_name))
	{
		std::cout << "already exists";
		return 0;
	}
	else
	{
		this->_ws_client->_stream_manager<FT>(full_stream_name, buffer, functor);
		return this->_ws_client->running_streams[full_stream_name];
	}
}
																																																							   
template<typename CT>
template <class FT>
unsigned int FuturesClient<CT>::stream_liquidation_orders(std::string symbol, std::string& buffer, FT& functor) 
{
	std::string full_stream_name = "/ws/" + symbol + "@" + "forceOrder";
	if (this->_ws_client->is_open(full_stream_name))
	{
		std::cout << "already exists";
		return 0;
	}
	else
	{
		this->_ws_client->_stream_manager<FT>(full_stream_name, buffer, functor);
		return this->_ws_client->running_streams[full_stream_name];
	}
}

template<typename CT>
template <class FT>
unsigned int FuturesClient<CT>::stream_liquidation_orders_all(std::string& buffer, FT& functor)
{
	std::string full_stream_name = "/ws/!forceOrder@arr";
	if (this->_ws_client->is_open(full_stream_name))
	{
		std::cout << "already exists";
		return 0;
	}
	else
	{
		this->_ws_client->_stream_manager<FT>(full_stream_name, buffer, functor);
		return this->_ws_client->running_streams[full_stream_name];
	}
}


//  ------------------------------ End | FuturesClient Global + CRTP methods - WS Streams 


//  ------------------------------ Start | FuturesClient General methods - Markets Stats

template <typename CT>
Json::Value FuturesClient<CT>::open_interest_stats(Params* params_obj)
{
	std::string query = params_obj ? this->_generate_query(*params_obj) : "";
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += "/futures/data/openInterestHist" + query;
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

template <typename CT>
Json::Value FuturesClient<CT>::top_long_short_ratio(Params* params_obj, bool accounts)
{
	std::string query = params_obj ? this->_generate_query(*params_obj) : "";
	std::string endpoint = accounts ? "/futures/data/topLongShortAccountRatio" : "/futures/data/topLongShortPositionRatio";
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += (endpoint + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

template <typename CT>
Json::Value FuturesClient<CT>::global_long_short_ratio(Params* params_obj)
{
	std::string query = params_obj ? this->_generate_query(*params_obj) : "";
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/futures/data/globalLongShortAccountRatio" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

template <typename CT>
Json::Value FuturesClient<CT>::taker_long_short_ratio(Params* params_obj)
{
	std::string query = params_obj ? this->_generate_query(*params_obj) : "";
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/futures/data/takerlongshortRatio" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

template <typename CT>
Json::Value FuturesClient<CT>::basis_data(Params* params_obj)
{
	std::string query = params_obj ? this->_generate_query(*params_obj) : "";
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/futures/data/basis" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

//  ------------------------------ End | FuturesClient General methods - Markets Stats


// =======================================================================================================


//  ------------------------------ Start | FuturesClientUSDT General methods - Infrastructure

FuturesClientUSDT::FuturesClientUSDT()
	: FuturesClient()
{};

FuturesClientUSDT::FuturesClientUSDT(std::string key, std::string secret)
	: FuturesClient(key, secret)
{}

FuturesClientUSDT::~FuturesClientUSDT()
{}

bool FuturesClientUSDT::v__init_ws_session()
{
	try
	{
		if (this->_ws_client) delete this->_ws_client;
		this->_ws_client = new WebsocketClient{ this->_WS_BASE_FUTURES_USDT, this->_WS_PORT_FUTURES };
		return 1;
	}
	catch (...)
	{
		throw("bad_init_ws");
	}
}

//  ------------------------------ Start | FuturesClientUSDT CRTP methods - Market Data Implementations

inline bool FuturesClientUSDT::v__ping_client()
{
	try
	{
		std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
		full_path += "/fapi/v1/ping";
		Json::Value ping_response = (this->_rest_client)->_getreq(full_path)["response"];
		return (ping_response != Json::nullValue);
	}
	catch (...)
	{
		throw("bad_ping");
	}
}

inline unsigned long long FuturesClientUSDT::v__exchange_time() 
{
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += "/fapi/v1/time";
	std::string ex_time = (this->_rest_client)->_getreq(full_path)["response"]["serverTime"].asString();

	return std::atoll(ex_time.c_str());
}

Json::Value FuturesClientUSDT::v__exchange_info()
{
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += "/fapi/v1/exchangeInfo";
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientUSDT::v__order_book(Params* params_obj)
{	
	std::string query = params_obj ? this->_generate_query(*params_obj) : "";
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/depth" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientUSDT::v__public_trades_recent(Params* params_obj)
{
	std::string query = params_obj ? this->_generate_query(*params_obj) : "";
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/trades" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientUSDT::v__public_trades_historical(Params* params_obj)
{
	std::string query = params_obj ? this->_generate_query(*params_obj) : "";
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/historicalTrades" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientUSDT::v__public_trades_agg(Params* params_obj)
{
	std::string query = params_obj ? this->_generate_query(*params_obj) : "";
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/aggTrades" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientUSDT::v__klines(Params* params_obj)
{
	std::string query = params_obj ? this->_generate_query(*params_obj) : "";
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/klines" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientUSDT::v__daily_ticker_stats(Params* params_obj)
{
	std::string query = params_obj ? this->_generate_query(*params_obj) : "";
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/ticker/24hr" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientUSDT::v__get_ticker(Params* params_obj)
{
	std::string query = params_obj ? this->_generate_query(*params_obj) : "";
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/ticker/price" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientUSDT::v__get_order_book_ticker(Params* params_obj)
{
	std::string query = params_obj ? this->_generate_query(*params_obj) : "";
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/ticker/bookTicker" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

//  ------------------------------ End | FuturesClientUSDT CRTP methods - Market Data Implementations


//  ------------------------------ Start | FuturesClientUSDT CRTP methods - Unique Endpoints

Json::Value FuturesClientUSDT::v_mark_price(Params* params_obj)
{
	std::string query = params_obj ? this->_generate_query(*params_obj) : ""; // todo: copy this format for everything?
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/premiumIndex" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientUSDT::v_public_liquidation_orders(Params* params_obj)
{
	std::string query = params_obj ? this->_generate_query(*params_obj) : "";
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/allForceOrders" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}
Json::Value FuturesClientUSDT::v_open_interest(Params* params_obj)
{
	std::string query = params_obj ? this->_generate_query(*params_obj) : "";
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/openInterest" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

// ~~~ Do not exist for this client

Json::Value FuturesClientUSDT::v_continues_klines(Params* params_obj)
{
	throw("non existing endpoint");
}
Json::Value FuturesClientUSDT::v_index_klines(Params* params_obj)
{
	throw("non existing endpoint");
}
Json::Value FuturesClientUSDT::v_mark_klines(Params* params_obj)
{
	throw("non existing endpoint");
}


// ~~~ Unique for this Client 

Json::Value FuturesClientUSDT::v_funding_rate_history(Params* params_obj)
{
	std::string query = params_obj ? this->_generate_query(*params_obj) : "";
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/fundingRate" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

//  ------------------------------ End | FuturesClientUSDT CRTP methods - Unique Endpoints

//  ------------------------------ Start | FuturesClientUSDT CRTP methods - Trade Implementations 


// -- Up to 'Client' Level


Json::Value FuturesClientUSDT::v__new_order(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/order" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response;
}

Json::Value FuturesClientUSDT::v__cancel_order(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += "/fapi/v1/order";
	Json::Value response = (this->_rest_client)->_deletereq(full_path);

	return response;
}

Json::Value FuturesClientUSDT::v__cancel_all_orders(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += "/fapi/v1/allOpenOrders";
	Json::Value response = (this->_rest_client)->_deletereq(full_path);

	return response;
}


Json::Value FuturesClientUSDT::v__query_order(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/order" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

Json::Value FuturesClientUSDT::v__open_orders(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/openOrders" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

Json::Value FuturesClientUSDT::v__all_orders(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/allOrders" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

Json::Value FuturesClientUSDT::v__account_info(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v2/account" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

Json::Value FuturesClientUSDT::v__account_trades_list(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/userTrades" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

// -- Up to 'FuturesClient' (this) Level


Json::Value FuturesClientUSDT::v_change_position_mode(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/positionSide/dual" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path); // should be spot?

	return response;
}

Json::Value FuturesClientUSDT::v_get_position_mode(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/positionSide/dual" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path); // should be spot?

	return response;
}

Json::Value FuturesClientUSDT::v_batch_orders(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/batchOrders" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path); // should be spot?

	return response;
}

Json::Value FuturesClientUSDT::v_cancel_batch_orders(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/batchOrders" + query);
	Json::Value response = (this->_rest_client)->_deletereq(full_path); // should be spot?

	return response;
}

Json::Value FuturesClientUSDT::v_cancel_all_orders_timer(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/countdownCancelAll" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path); // should be spot?

	return response;
}

Json::Value FuturesClientUSDT::v_query_open_order(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/openOrder" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path); // should be spot?

	return response;
}

Json::Value FuturesClientUSDT::v_account_balances(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v2/balance" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path); // should be spot?

	return response;
}

Json::Value FuturesClientUSDT::v_change_leverage(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/leverage" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path); // should be spot?

	return response;
}

Json::Value FuturesClientUSDT::v_change_margin_type(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/marginType" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path); // should be spot?

	return response;
}

Json::Value FuturesClientUSDT::v_change_position_margin(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/positionMargin" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path); // should be spot?

	return response;
}

Json::Value FuturesClientUSDT::v_change_position_margin_history(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/positionMargin/history" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path); // should be spot?

	return response;
}

Json::Value FuturesClientUSDT::v_position_info(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v2/positionRisk" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

Json::Value FuturesClientUSDT::v_get_income_history(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/income" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

Json::Value FuturesClientUSDT::v_get_leverage_bracket(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/leverageBracket" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

Json::Value FuturesClientUSDT::v_pos_adl_quantile_est(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/fapi/v1/adlQuantile" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path); 

	return response;
}

//  ------------------------------ End | FuturesClientUSDT CRTP methods - Trade Implementations 

//  ------------------------------ Start | FuturesClientUSDT CRTP methods - WS Streams


template <class FT>
unsigned int FuturesClientUSDT::v__stream_markprice_all(std::string symbol, std::string& buffer, FT& functor)
{
	std::string full_stream_name = "/ws/" + symbol + '@' + "miniTicker";
	if (this->_ws_client->is_open(full_stream_name))
	{
		std::cout << "already exists";
		return 0;
	}
	else
	{
		this->_ws_client->_stream_manager<FT>(full_stream_name, buffer, functor);
		return this->_ws_client->running_streams[full_stream_name];
	}
}


template <class FT>
unsigned int FuturesClientUSDT::v__stream_indexprice(std::string pair, std::string& buffer, FT& functor, unsigned int interval)
{
	throw("non-existing for usdt");
}

template <class FT>
unsigned int FuturesClientUSDT::v__stream_markprice_by_pair(std::string& pair, std::string& buffer, FT& functor, unsigned int interval)
{
	throw("non-existing for usdt");
}

template <class FT>
unsigned int FuturesClientUSDT::v__stream_kline_contract(std::string pair_and_type, std::string& buffer, FT& functor, std::string interval)
{
	throw("non-existing for usdt");
}

template <class FT>
unsigned int FuturesClientUSDT::v__stream_kline_index(std::string pair, std::string& buffer, FT& functor, std::string interval)
{
	throw("non-existing for usdt");
}

template <class FT>
unsigned int FuturesClientUSDT::v__stream_kline_markprice(std::string symbol, std::string& buffer, FT& functor, std::string interval)
{
	throw("non-existing for usdt");
}

//  ------------------------------ End | FuturesClientUSDT CRTP methods - WS Streams


// =======================================================================================================


//  ------------------------------ Start | FuturesClientCoin General methods - Infrastructure

FuturesClientCoin::FuturesClientCoin()
	: FuturesClient()
{};

FuturesClientCoin::FuturesClientCoin(std::string key, std::string secret)
	: FuturesClient(key, secret)
{}

FuturesClientCoin::~FuturesClientCoin()
{}

bool FuturesClientCoin::v__init_ws_session()
{
	try
	{
		if (this->_ws_client) delete this->_ws_client;
		this->_ws_client = new WebsocketClient{ this->_WS_BASE_FUTURES_COIN, this->_WS_PORT_FUTURES };
		return 1;
	}
	catch (...)
	{
		throw("bad_init_ws");
	}
}

//  ------------------------------ End | FuturesClientCoin General methods - Infrastructure

//  ------------------------------ Start | FuturesClientCoin CRTP methods - Market Data Implementations

inline bool FuturesClientCoin::v__ping_client()
{
	try
	{
		std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
		full_path += "/dapi/v1/ping";
		Json::Value ping_response = (this->_rest_client)->_getreq(full_path)["response"];
		return (ping_response != Json::nullValue);
	}
	catch (...)
	{
		throw("bad_ping");
	}
}

inline unsigned long long FuturesClientCoin::v__exchange_time()
{
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += "/dapi/v1/time";
	std::string ex_time = (this->_rest_client)->_getreq(full_path)["response"]["serverTime"].asString();

	return std::atoll(ex_time.c_str());
}

Json::Value FuturesClientCoin::v__exchange_info()
{
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += "/dapi/v1/exchangeInfo";
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientCoin::v__order_book(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/depth" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientCoin::v__public_trades_recent(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/trades" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientCoin::v__public_trades_historical(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/historicalTrades" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientCoin::v__public_trades_agg(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/aggTrades" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientCoin::v__klines(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/klines" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientCoin::v__daily_ticker_stats(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/ticker/24hr" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientCoin::v__get_ticker(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/ticker/price" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientCoin::v__get_order_book_ticker(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/ticker/bookTicker" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

//  ------------------------------ End | FuturesClientCoin CRTP methods - Market Data Implementations

//  ------------------------------ Start | FuturesClientUSDT CRTP methods - Trade Implementations 


// -- Up to 'Client' Level


Json::Value FuturesClientCoin::v__new_order(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/order" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response;
}

Json::Value FuturesClientCoin::v__cancel_order(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/order" + query);
	Json::Value response = (this->_rest_client)->_deletereq(full_path);

	return response;
}

Json::Value FuturesClientCoin::v__cancel_all_orders(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/allOpenOrders" + query);
	Json::Value response = (this->_rest_client)->_deletereq(full_path);

	return response;
}

Json::Value FuturesClientCoin::v__query_order(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/order" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

Json::Value FuturesClientCoin::v__open_orders(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/openOrders" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

Json::Value FuturesClientCoin::v__all_orders(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/allOrders" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

Json::Value FuturesClientCoin::v__account_info(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/account" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

Json::Value FuturesClientCoin::v__account_trades_list(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/userTrades" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

// -- Up to 'FuturesClient' (this) Level


Json::Value FuturesClientCoin::v_change_position_mode(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/positionSide/dual" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response;
}


Json::Value FuturesClientCoin::v_get_position_mode(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/positionSide/dual" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path); 

	return response;
}

Json::Value FuturesClientCoin::v_batch_orders(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/batchOrders" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response;
}

Json::Value FuturesClientCoin::v_cancel_batch_orders(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/batchOrders" + query);
	Json::Value response = (this->_rest_client)->_deletereq(full_path); 

	return response;
}

Json::Value FuturesClientCoin::v_cancel_all_orders_timer(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/countdownCancelAll" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path); 

	return response;
}

Json::Value FuturesClientCoin::v_query_open_order(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/openOrder" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path); 

	return response;
}

Json::Value FuturesClientCoin::v_account_balances(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/balance" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path); 

	return response;
}

Json::Value FuturesClientCoin::v_change_leverage(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/leverage" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path); 

	return response;
}

Json::Value FuturesClientCoin::v_change_margin_type(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/marginType" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path);

	return response;
}

Json::Value FuturesClientCoin::v_change_position_margin(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/positionMargin" + query);
	Json::Value response = (this->_rest_client)->_postreq(full_path); 

	return response;
}

Json::Value FuturesClientCoin::v_change_position_margin_history(Params* params_obj)
{
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/positionMargin/history" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path); 

	return response;
}

Json::Value FuturesClientCoin::v_position_info(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/positionRisk" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

Json::Value FuturesClientCoin::v_get_income_history(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/income" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

Json::Value FuturesClientCoin::v_get_leverage_bracket(Params* params_obj)
{
	std::unique_ptr<Params>unique_param_ptr;
	if (!params_obj)
	{
		unique_param_ptr = std::unique_ptr<Params>(new Params{});
		params_obj = unique_param_ptr.get();
	}
	std::string query = this->_generate_query(*params_obj, 1);
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/leverageBracket" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);

	return response;
}

Json::Value FuturesClientCoin::v_pos_adl_quantile_est(Params* params_obj)
{
	throw("no such endpoint :)"); // todo: no implementation
}

//  ------------------------------ End | FuturesClientUSDT CRTP methods - Trade Implementations 

//  ------------------------------ Start | FuturesClientUSDT CRTP methods - WS Streams


template <class FT>
unsigned int FuturesClientCoin::v__stream_markprice_all(std::string symbol, std::string& buffer, FT& functor) // here
{
	throw("non-existing for coin");
}


template <class FT>
unsigned int FuturesClientCoin::v__stream_indexprice(std::string pair, std::string& buffer, FT& functor, unsigned int interval)
{
	std::string full_stream_name = "/ws/" + pair + "@" + "indexPrice" + "@" + std::to_string(interval) + "ms";
	if (this->_ws_client->is_open(full_stream_name))
	{
		std::cout << "already exists";
		return 0;
	}
	else
	{
		this->_ws_client->_stream_manager<FT>(full_stream_name, buffer, functor);
		return this->_ws_client->running_streams[full_stream_name];
	}
}

template <class FT>
unsigned int FuturesClientCoin::v__stream_markprice_by_pair(std::string& pair, std::string& buffer, FT& functor, unsigned int interval)
{
	std::string full_stream_name = "/ws/" + pair + "@" + "markPrice" + "@" + std::to_string(interval) + "ms";
	if (this->_ws_client->is_open(full_stream_name))
	{
		std::cout << "already exists";
		return 0;
	}
	else
	{
		this->_ws_client->_stream_manager<FT>(full_stream_name, buffer, functor);
		return this->_ws_client->running_streams[full_stream_name];
	}
}

template <class FT>
unsigned int FuturesClientCoin::v__stream_kline_contract(std::string pair_and_type, std::string& buffer, FT& functor, std::string interval)
{
	std::string full_stream_name = "/ws/" + pair_and_type + "@" + "continuousKline_" + (interval);
	if (this->_ws_client->is_open(full_stream_name))
	{
		std::cout << "already exists";
		return 0;
	}
	else
	{
		this->_ws_client->_stream_manager<FT>(full_stream_name, buffer, functor);
		return this->_ws_client->running_streams[full_stream_name];
	}
}

template <class FT>
unsigned int FuturesClientCoin::v__stream_kline_index(std::string pair, std::string& buffer, FT& functor, std::string interval)
{
	std::string full_stream_name = "/ws/" + pair + "@" + "indexPriceKline_" + (interval);
	if (this->_ws_client->is_open(full_stream_name))
	{
		std::cout << "already exists";
		return 0;
	}
	else
	{
		this->_ws_client->_stream_manager<FT>(full_stream_name, buffer, functor);
		return this->_ws_client->running_streams[full_stream_name];
	}
}

template <class FT>
unsigned int FuturesClientCoin::v__stream_kline_markprice(std::string symbol, std::string& buffer, FT& functor, std::string interval)
{
	std::string full_stream_name = "/ws/" + symbol + "@" + "markPriceKline_" + (interval);
	if (this->_ws_client->is_open(full_stream_name))
	{
		std::cout << "already exists";
		return 0;
	}
	else
	{
		this->_ws_client->_stream_manager<FT>(full_stream_name, buffer, functor);
		return this->_ws_client->running_streams[full_stream_name];
	}
}

//  ------------------------------ End | FuturesClientUSDT CRTP methods - WS Streams


//  ------------------------------ Start | FuturesClientCoin CRTP methods - Unique Endpoints

Json::Value FuturesClientCoin::v_mark_price(Params* params_obj)
{
	std::string query = params_obj ? this->_generate_query(*params_obj) : ""; 
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/premiumIndex" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}

Json::Value FuturesClientCoin::v_public_liquidation_orders(Params* params_obj)
{
	std::string query = params_obj ? this->_generate_query(*params_obj) : "";
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/allForceOrders" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}
Json::Value FuturesClientCoin::v_open_interest(Params* params_obj)
{
	std::string query = params_obj ? this->_generate_query(*params_obj) : "";
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/openInterest" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}


// ~~~ Unique for this Client 

Json::Value FuturesClientCoin::v_continues_klines(Params* params_obj)
{
	std::string query = params_obj ? this->_generate_query(*params_obj) : "";
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/continuousKlines" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}
Json::Value FuturesClientCoin::v_index_klines(Params* params_obj)
{
	std::string query = params_obj ? this->_generate_query(*params_obj) : "";
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/indexPriceKlines" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}
Json::Value FuturesClientCoin::v_mark_klines(Params* params_obj)
{
	std::string query = params_obj ? this->_generate_query(*params_obj) : "";
	std::string full_path = !this->_testnet_mode ? this->_BASE_REST_FUTURES : this->_BASE_REST_FUTURES_TESTNET;
	full_path += ("/dapi/v1/markPriceKlines" + query);
	Json::Value response = (this->_rest_client)->_getreq(full_path);
	return response;
}


// ~~~ Don't exist for this client

Json::Value FuturesClientCoin::v_funding_rate_history(Params* params_obj)
{
	throw("non-existing endpoint");
}

//  ------------------------------ End | FuturesClientCoin CRTP methods - Unique Endpoints

//  ------------------------------ Start | FuturesClientCoin CRTP methods - WS Streams

// -- Global (up to Client level)
// todo: if testnet


//  ------------------------------ End | FuturesClientCoin CRTP methods - WS Streams


// =======================================================================================================


//  ------------------------------ Start | Params methods

Params::Params()
	: default_recv{ 0 }, default_recv_amt{ 0 }, flush_params{ 0 }
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
void Params::set_param(std::string key, const PT& value)
{
	param_map[key] = std::to_string(value);
}
template <> // do not call to_string on a string
void Params::set_param<std::string>(std::string key, const std::string& value)
{
	param_map[key] = value;
}

template <typename PT>
void Params::set_param(std::string key, PT&& value)
{
	param_map[key] = std::to_string(std::move(value));
}
template <> 
void Params::set_param<std::string>(std::string key, std::string&& value)
{
	param_map[key] = std::move(value);
}


bool Params::delete_param(std::string key)
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

void Params::set_recv(bool set_always, unsigned int recv_val)
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

bool Params::clear_params()
{
	this->param_map.clear();
	if (this->default_recv) this->set_param<unsigned int>("recvWindow", this->default_recv_amt);
	return this->empty();
}

bool Params::empty()
{
	return this->param_map.empty();
}

//  ------------------------------ End | Params methods
