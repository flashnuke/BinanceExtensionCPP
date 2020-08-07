// todo: futures and client source files in different files?
// todo: return empty json with "status = 1" if no cb passed.
// todo: idea - pass stream of name to functor?
// todo: param object for all methods?
// todo: one by one check for default args
// todo: default param for all (nullptr). where sign is needed, use unique_ptr
// todo important: testnet bool for every futures method! or just define a member attribute as bool


// DOCs todos:
// 1. order book fetch from scratch example
// 2. ws symbols must be lower case
// 3. v_ is for crtp
// 4. custom requests, pass params into query
// 5. I let passing empty or none params so the user can receive the error and see whats missing! better than runtime error

// First make everything for spot and then for futures

#ifndef CRYPTO_EXTENSIONS_H
#define CRYPTO_EXTENSIONS_H

#define _WIN32_WINNT 0x0601 // for boost

// external libraries
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>

#include <json/json.h>
#include <curl/curl.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>

// STL
#include <iostream>
#include <chrono>
#include <string>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <vector>




namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

unsigned long long local_timestamp();
inline auto binary_to_hex_digit(unsigned a) -> char;
auto binary_to_hex(unsigned char const* binary, unsigned binary_len)->std::string;
std::string HMACsha256(std::string const& message, std::string const& key);


class RestSession
{
private:

	struct RequestHandler // handles response
	{
		RequestHandler();
		std::string req_raw;
		Json::Value req_json;
		CURLcode req_status;
		std::unique_lock<std::mutex>* locker;
	};


public:
	RestSession();

	bool status; // bool for whether session is active or not

	CURL* _get_handle{};
	CURL* _post_handle{};
	CURL* _put_handle{};
	CURL* _delete_handle{};

	Json::Value _getreq(std::string full_path);
	inline void get_timeout(unsigned long interval);
	std::mutex _get_lock;

	Json::Value _postreq(std::string full_path);
	inline void post_timeout(unsigned long interval);
	std::mutex _post_lock;

	Json::Value _putreq(std::string full_path);
	inline void put_timeout(unsigned long interval);
	std::mutex _put_lock;

	Json::Value _deletereq(std::string full_path);
	inline void delete_timeout(unsigned long interval);
	std::mutex _delete_lock;

	bool close();
	void set_verbose(const long int state);

	friend unsigned int _REQ_CALLBACK(void* contents, unsigned int size, unsigned int nmemb, RestSession::RequestHandler* req);

	~RestSession();
};

class WebsocketClient
{
private:
	const std::string _host;
	const std::string _port;
	bool _reconnect_on_error;

	template <class FT>
	void _connect_to_endpoint(std::string stream_map_name, std::string& buf, FT& functor, std::pair<RestSession*,
		std::string> user_stream_pair = std::make_pair<RestSession*, std::string>(nullptr, ""));

public:
	WebsocketClient(std::string host, std::string port);

	std::unordered_map<std::string, bool> running_streams; // will be a map, containing pairs of: <bool(status), ws_stream> 

	void close_stream(const std::string stream_name);
	std::vector<std::string> open_streams();
	bool is_open(const std::string& stream_name);
	unsigned int refresh_listenkey_interval;

	template <class FT>
	void _stream_manager(std::string stream_map_name, std::string& buf, FT& functor, std::pair<RestSession*,
		std::string> user_stream_pair = std::make_pair<RestSession*, std::string>(nullptr, ""));
	
	void _set_reconnect(const bool& reconnect);


	~WebsocketClient();

};



struct Params
	// Params will be stored in a map of <str, str> and parsed by the query generator.
{

	Params();
	explicit Params(Params& param_obj);
	explicit Params(const Params& param_obj);

	Params& operator=(Params& params_obj);
	Params& operator=(const Params& params_obj);

	std::unordered_map<std::string, std::string> param_map;
	bool default_recv;
	unsigned int default_recv_amt;

	template <typename PT>
	void set_param(std::string key, PT value);

	bool delete_param(std::string key);

	void set_recv(bool set_always, unsigned int recv_val = 0);

	bool clear_params();
	bool empty();

	bool flush_params; // if true, param objects will be flushed at the end of methods

};


template<typename T>
class Client
{
private:


protected:
	std::string _api_key;
	std::string _api_secret;


public:
	explicit Client();
	Client(std::string key, std::string secret);

	bool const _public_client;

	std::string _generate_query(Params& params_obj, bool sign_query = 0);

	const std::string _BASE_REST_FUTURES{ "https://fapi.binance.com" };
	const std::string _BASE_REST_SPOT{ "https://api.binance.com" };
	const std::string _WS_BASE_FUTURES{"fstream.binance.com"};
	const std::string _WS_BASE_SPOT{ "stream.binance.com" };
	const std::string _WS_PORT{ "9443" };


	// ----------------------CRTP methods
	
	// Market Data endpoints

	bool ping_client();
	unsigned long long exchange_time();
	Json::Value exchange_info(); 
	Json::Value order_book(Params* params_obj); 
	Json::Value public_trades_recent(Params* params_obj);
	Json::Value public_trades_historical(Params* params_obj); 
	Json::Value public_trades_agg(Params* params_obj); 
	Json::Value klines(Params* params_obj); 
	Json::Value daily_ticker_stats(Params* params_obj = nullptr); 
	Json::Value get_ticker(Params* params_obj = nullptr); 
	Json::Value get_order_book_ticker(Params* params_obj = nullptr);


	// Trading endpoints

	Json::Value test_new_order(Params* params_obj);
	Json::Value new_order(Params* params_obj);
	Json::Value cancel_order(Params* params_obj);
	Json::Value cancel_all_orders(Params* params_obj);
	Json::Value query_order(Params* params_obj);
	Json::Value open_orders(Params* params_obj = nullptr);
	Json::Value all_orders(Params* params_obj);
	Json::Value account_info(Params* params_obj = nullptr);
	Json::Value account_trades_list(Params* params_obj);



	// Library methods

	bool init_ws_session();
	std::string _get_listen_key();
	void close_stream(const std::string& symbol, const std::string& stream_name);
	bool is_stream_open(const std::string& symbol, const std::string& stream_name);
	std::vector<std::string> get_open_streams();
	void ws_auto_reconnect(const bool& reconnect);
	inline void set_refresh_key_interval(const bool val);


	// ----------------------end CRTP methods

	bool init_rest_session();
	bool set_headers(RestSession* rest_client);
	void rest_set_verbose(bool state);

	// Global requests (wallet, account etc)

	bool exchange_status(); // todo: (define) (Returns bool 1 up 0 down) (use spot base)
	Json::Value place_order_test(Params* params_obj);

	struct Wallet 
	{
		Client<T>* user_client;
		explicit Wallet(Client<T>& client); // todo: if public, exception
		explicit Wallet(const Client<T>& client); // todo: if public, exception
		~Wallet();

		Json::Value get_all_coins(Params* params_obj = nullptr); 
		Json::Value daily_snapshot(Params* params_obj); 
		Json::Value fast_withdraw_switch(bool state);
		Json::Value withdraw_balances(Params* params_obj, bool SAPI = 0); 
		Json::Value deposit_history(Params* params_obj = nullptr, bool network = 0);
		Json::Value withdraw_history(Params* params_obj = nullptr, bool network = 0); 
		Json::Value deposit_address(Params* params_obj, bool network = 0); 
		Json::Value account_status(Params* params_obj = nullptr); 
		Json::Value account_status_api(Params* params_obj = nullptr); 
		Json::Value dust_log(Params* params_obj = nullptr);  
		Json::Value dust_transfer(Params* params_obj);  
		Json::Value asset_dividend_records(Params* params_obj = nullptr);
		Json::Value asset_details(Params* params_obj = nullptr); 
		Json::Value trading_fees(Params* params_obj = nullptr); 
	}; 

	struct SubAccount // for corporate accounts
	{
		Client<T>* user_client;
		explicit SubAccount(Client<T>& client); // todo: if public, exception
		explicit SubAccount(const Client<T>& client); // todo: if public, exception
		~SubAccount();

		Json::Value get_all_subaccounts(Params* params_obj = nullptr); 

		Json::Value transfer_master_history(Params* params_obj); 
		Json::Value transfer_master_to_subaccount(Params* params_obj); 

		Json::Value get_subaccount_balances(Params* params_obj); 
		Json::Value get_subaccount_deposit_address(Params* params_obj); 
		Json::Value get_subaccount_deposit_history(Params* params_obj); 
		Json::Value get_subaccount_future_margin_status(Params* params_obj = nullptr); 

		Json::Value enable_subaccount_margin(Params* params_obj); 
		Json::Value get_subaccount_margin_status(Params* params_obj); 
		Json::Value get_subaccount_margin_summary(Params* params_obj = nullptr);

		Json::Value enable_subaccount_futures(Params* params_obj);
		Json::Value get_subaccount_futures_status(Params* params_obj); 
		Json::Value get_subaccount_futures_summary(Params* params_obj = nullptr); 
		Json::Value get_subaccount_futures_positionrisk(Params* params_obj); 

		Json::Value transfer_to_subaccount_futures(Params* params_obj);
		Json::Value transfer_to_subaccount_margin(Params* params_obj);
		Json::Value transfer_subaccount_to_subaccount(Params* params_obj); 
		Json::Value transfer_subaccount_to_master(Params* params_obj); 
		Json::Value transfer_subaccount_history(Params* params_obj); 

	};


	Json::Value custom_get_req(const std::string& base, const std::string& endpoint, Params* params_obj, bool signature = 0);
	Json::Value custom_post_req(const std::string& base, const std::string& endpoint, Params* params_obj, bool signature = 0);
	Json::Value custom_put_req(const std::string& base, const std::string& endpoint, Params* params_obj, bool signature = 0);
	Json::Value custom_delete_req(const std::string& base, const std::string& endpoint, Params* params_obj, bool signature = 0);

	template <typename FT>
	unsigned int custom_stream(std::string stream_query, std::string buffer, FT functor);

	RestSession* _rest_client = nullptr; // move init
	WebsocketClient* _ws_client = nullptr; // move init, leave decl

	~Client();

};


template <typename CT> // CT = coin type
class FuturesClient : public Client<FuturesClient<CT>>
{
private:
	inline bool v_init_ws_session();
	inline std::string v__get_listen_key();
	inline void v_close_stream(const std::string& symbol, const std::string& stream_name);
	inline bool v_is_stream_open(const std::string& symbol, const std::string& stream_name);
	inline std::vector<std::string> v_get_open_streams();
	inline void v_ws_auto_reconnect(const bool& reconnect);
	inline void v_set_refresh_key_interval(const bool val);


public:
	friend Client<FuturesClient<CT>>;

	FuturesClient();
	FuturesClient(std::string key, std::string secret);

	// consider: are these crtp?? for USDT and Margined
	// starting with v__ means it has Spot version as well

	// ------------------- crtp for all (spot + coin/usdt)

	// market data

	inline bool v_ping_client();  // todo: define lower levels
	inline unsigned long long v_exchange_time();
	Json::Value v_exchange_info();
	Json::Value v_order_book(Params* params_obj);
	Json::Value v_public_trades_recent(Params* params_obj);
	Json::Value v_public_trades_historical(Params* params_obj);
	Json::Value v_public_trades_agg(Params* params_obj);
	Json::Value v_klines(Params* params_obj);
	Json::Value v_daily_ticker_stats(Params* params_obj);
	Json::Value v_get_ticker(Params* params_obj);
	Json::Value v_get_order_book_ticker(Params* params_obj);

	// trading endpoints

	// -- mutual with spot

	Json::Value v_test_new_order(Params* params_obj);
	Json::Value v_new_order(Params* params_obj);
	Json::Value v_cancel_order(Params* params_obj);
	Json::Value v_cancel_all_orders(Params* params_obj);
	Json::Value v_query_order(Params* params_obj);
	Json::Value v_open_orders(Params* params_obj = nullptr);
	Json::Value v_all_orders(Params* params_obj);
	Json::Value v_account_info(Params* params_obj = nullptr);
	Json::Value v_account_trades_list(Params* params_obj);

	// -- unique to future endpoints

	Json::Value futures_transfer(Params* params_obj);
	Json::Value futures_transfer_history(Params* params_obj);
	Json::Value change_position_mode(Params* params_obj);
	Json::Value get_position_mode(Params* params_obj = nullptr);
	Json::Value batch_orders(Params* params_obj);
	Json::Value cancel_batch_orders(Params* params_obj);
	Json::Value cancel_all_orders_timer(Params* params_obj);
	Json::Value query_open_order(Params* params_obj);
	Json::Value account_balances(Params* params_obj = nullptr);
	Json::Value change_leverage(Params* params_obj);
	Json::Value change_margin_type(Params* params_obj);
	Json::Value change_position_margin(Params* params_obj);
	Json::Value change_position_margin_history(Params* params_obj);
	Json::Value position_info(Params* params_obj = nullptr);
	Json::Value get_income_history(Params* params_obj);
	Json::Value get_leverage_bracket(Params* params_obj = nullptr);


	// -- unique to USDT endpoint

	Json::Value pos_adl_quantile_est(Params* params_obj = nullptr); // todo: define, default param



	// -------------------  inter-future crtp ONLY

	// todo: exception for bad_endpoint or nonexisting

	 // market Data

	Json::Value mark_price(Params* params_obj = nullptr); // todo: define, crtp? default param
	Json::Value public_liquidation_orders(Params* params_obj); // todo: define, crtp?
	Json::Value open_interest(Params* params_obj); // todo: define, crtp?


	// note that the following four might be only for coin margined market data
	Json::Value continues_klines(Params* params_obj); // todo: define, crtp?
	Json::Value index_klines(Params* params_obj); // todo: define, crtp?
	Json::Value mark_klines(Params* params_obj); // todo: define, crtp?

	// note that the following four might be only for coin margined market data

	Json::Value funding_rate_history(Params* params_obj); // todo: define, crtp?


	// end CRTP

	// endpoints are same for both wallet types below

	Json::Value open_interest_stats(Params* params_obj); 
	Json::Value top_long_short_ratio(Params* params_obj, bool accounts = 0);
	Json::Value global_long_short_ratio(Params* params_obj);
	Json::Value taker_long_short_ratio(Params* params_obj);
	Json::Value basis_data(Params* params_obj);


	unsigned int aggTrade(std::string symbol);
	template <class FT>
	unsigned int userStream(std::string& buffer, FT& functor);

	~FuturesClient();
};


class FuturesClientUSDT : public FuturesClient<FuturesClientUSDT>
{
public:
	friend FuturesClient;

	FuturesClientUSDT();
	FuturesClientUSDT(std::string key, std::string secret);

	// up to Client level

	inline bool v__ping_client();
	inline unsigned long long v__exchange_time(); 
	Json::Value v__exchange_info(); 
	Json::Value v__order_book(Params* params_obj);
	Json::Value v__public_trades_recent(Params* params_obj);
	Json::Value v__public_trades_historical(Params* params_obj);
	Json::Value v__public_trades_agg(Params* params_obj);
	Json::Value v__klines(Params* params_obj); 
	Json::Value v__daily_ticker_stats(Params* params_obj); 
	Json::Value v__get_ticker(Params* params_obj);
	Json::Value v__get_order_book_ticker(Params* params_obj);

	// market Data

	Json::Value v_mark_price(Params* params_obj = nullptr); 
	Json::Value v_public_liquidation_orders(Params* params_obj); 
	Json::Value v_open_interest(Params* params_obj); 


	// note that the following four might be only for coin margined market data
	Json::Value v_continues_klines(Params* params_obj); 
	Json::Value v_index_klines(Params* params_obj); 
	Json::Value v_mark_klines(Params* params_obj); 

	// note that the following four might be only for usdt margined market data

	Json::Value v_funding_rate_history(Params* params_obj); 


	// trading endpoints

// -- mutual with spot

	Json::Value v__test_new_order(Params* params_obj);
	Json::Value v__new_order(Params* params_obj);
	Json::Value v__cancel_order(Params* params_obj);
	Json::Value v__cancel_all_orders(Params* params_obj);
	Json::Value v__query_order(Params* params_obj);
	Json::Value v__open_orders(Params* params_obj = nullptr);
	Json::Value v__all_orders(Params* params_obj);
	Json::Value v__account_info(Params* params_obj = nullptr);
	Json::Value v__account_trades_list(Params* params_obj);

	// -- unique to future endpoints

	Json::Value v_futures_transfer(Params* params_obj);
	Json::Value v_futures_transfer_history(Params* params_obj);
	Json::Value v_change_position_mode(Params* params_obj);
	Json::Value v_get_position_mode(Params* params_obj = nullptr);
	Json::Value v_batch_orders(Params* params_obj);
	Json::Value v_cancel_batch_orders(Params* params_obj);
	Json::Value v_cancel_all_orders_timer(Params* params_obj);
	Json::Value v_query_open_order(Params* params_obj);
	Json::Value v_account_balances(Params* params_obj = nullptr);
	Json::Value v_change_leverage(Params* params_obj);
	Json::Value v_change_margin_type(Params* params_obj);
	Json::Value v_change_position_margin(Params* params_obj);
	Json::Value v_change_position_margin_history(Params* params_obj);
	Json::Value v_position_info(Params* params_obj = nullptr);
	Json::Value v_get_income_history(Params* params_obj);
	Json::Value v_get_leverage_bracket(Params* params_obj = nullptr);


	// -- unique to USDT endpoint

	Json::Value v_pos_adl_quantile_est(Params* params_obj = nullptr); // todo: define, default param

	~FuturesClientUSDT();
};


class FuturesClientCoin : public FuturesClient<FuturesClientCoin>
{
public:
	friend FuturesClient;

	FuturesClientCoin();
	FuturesClientCoin(std::string key, std::string secret);

	// up to Client level

	inline bool v__ping_client(); 
	inline unsigned long long v__exchange_time(); 
	Json::Value v__exchange_info(); 
	Json::Value v__order_book(Params* params_obj); 
	Json::Value v__public_trades_recent(Params* params_obj); 
	Json::Value v__public_trades_historical(Params* params_obj); 
	Json::Value v__public_trades_agg(Params* params_obj);
	Json::Value v__klines(Params* params_obj);
	Json::Value v__daily_ticker_stats(Params* params_obj); 
	Json::Value v__get_ticker(Params* params_obj);
	Json::Value v__get_order_book_ticker(Params* params_obj); 

	// market Data

	Json::Value v_mark_price(Params* params_obj); 
	Json::Value v_public_liquidation_orders(Params* params_obj);
	Json::Value v_open_interest(Params* params_obj); 


	// note that the following four might be only for coin margined market data
	Json::Value v_continues_klines(Params* params_obj); 
	Json::Value v_index_klines(Params* params_obj); 
	Json::Value v_mark_klines(Params* params_obj);

	// note that the following four might be only for coin margined market data

	Json::Value v_funding_rate_history(Params* params_obj); 

	// trading endpoints

// -- mutual with spot

	Json::Value v__test_new_order(Params* params_obj);
	Json::Value v__new_order(Params* params_obj);
	Json::Value v__cancel_order(Params* params_obj);
	Json::Value v__cancel_all_orders(Params* params_obj);
	Json::Value v__query_order(Params* params_obj);
	Json::Value v__open_orders(Params* params_obj = nullptr);
	Json::Value v__all_orders(Params* params_obj);
	Json::Value v__account_info(Params* params_obj = nullptr);
	Json::Value v__account_trades_list(Params* params_obj);

	// -- unique to future endpoints

	Json::Value v_futures_transfer(Params* params_obj);
	Json::Value v_futures_transfer_history(Params* params_obj);
	Json::Value v_change_position_mode(Params* params_obj);
	Json::Value v_get_position_mode(Params* params_obj = nullptr);
	Json::Value v_batch_orders(Params* params_obj);
	Json::Value v_cancel_batch_orders(Params* params_obj);
	Json::Value v_cancel_all_orders_timer(Params* params_obj);
	Json::Value v_query_open_order(Params* params_obj);
	Json::Value v_account_balances(Params* params_obj = nullptr);
	Json::Value v_change_leverage(Params* params_obj);
	Json::Value v_change_margin_type(Params* params_obj);
	Json::Value v_change_position_margin(Params* params_obj);
	Json::Value v_change_position_margin_history(Params* params_obj);
	Json::Value v_position_info(Params* params_obj = nullptr);
	Json::Value v_get_income_history(Params* params_obj);
	Json::Value v_get_leverage_bracket(Params* params_obj = nullptr);


	// -- unique to USDT endpoint

	Json::Value v_pos_adl_quantile_est(Params* params_obj = nullptr);

	~FuturesClientCoin();
};

class SpotClient : public Client<SpotClient>
{
private:
	// CRTP methods
	// ------------------- crtp for all (spot + coin/usdt)

	// market data

	inline bool v_ping_client(); 
	inline unsigned long long v_exchange_time(); 
	Json::Value v_exchange_info(); 
	Json::Value v_order_book(Params* params_obj); 
	Json::Value v_public_trades_recent(Params* params_obj);
	Json::Value v_public_trades_historical(Params* params_obj);
	Json::Value v_public_trades_agg(Params* params_obj); 
	Json::Value v_klines(Params* params_obj); 
	Json::Value v_daily_ticker_stats(Params* params_obj); 
	Json::Value v_get_ticker(Params* params_obj);
	Json::Value v_get_order_book_ticker(Params* params_obj);

	// ------------------- crtp global end

	// Trading endpoints

	// ---- CRTP implementations

	Json::Value v_test_new_order(Params* params_obj);
	Json::Value v_new_order(Params* params_obj);
	Json::Value v_cancel_order(Params* params_obj);
	Json::Value v_cancel_all_orders(Params* params_obj);
	Json::Value v_query_order(Params* params_obj);
	Json::Value v_open_orders(Params* params_obj = nullptr);
	Json::Value v_all_orders(Params* params_obj);
	Json::Value v_account_info(Params* params_obj = nullptr);
	Json::Value v_account_trades_list(Params* params_obj);

	// ---- general methods

	Json::Value oco_new_order(Params* params_obj);
	Json::Value oco_cancel_order(Params* params_obj);
	Json::Value oco_query_order(Params* params_obj = nullptr);
	Json::Value oco_all_orders(Params* params_obj = nullptr);
	Json::Value oco_open_orders(Params* params_obj = nullptr);


	// crtp infrastructure start

	bool v_init_ws_session();
	std::string v__get_listen_key();
	void v_close_stream(const std::string& symbol, const std::string& stream_name);
	bool v_is_stream_open(const std::string& symbol, const std::string& stream_name);
	std::vector<std::string> v_get_open_streams();
	void v_ws_auto_reconnect(const bool& reconnect);
	inline void v_set_refresh_key_interval(const bool val);

	// crtp infrastructure end , todo: make this more organized ofc






public:
	friend Client;

	SpotClient();
	SpotClient(std::string key, std::string secret);

	template <class FT>
	unsigned int aggTrade(std::string symbol, std::string& buffer, FT& functor);
	template <class FT>
	unsigned int userStream(std::string& buffer, FT& functor);

	~SpotClient();
};

#endif