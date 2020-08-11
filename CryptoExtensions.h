// todo: idea - pass stream of name to functor?
// todo: better handle error codes api
// todo: marginaccount construct from futures impossible?
// todo: pass client to _ws and use the ping_listen_key for that


// DOCs todos:
// 1. order book fetch from scratch example
// 2. ws symbols must be lower case
// 3. v_ is for crtp
// 4. custom requests, pass params into query
// 5. I let passing empty or none params so the user can receive the error and see whats missing! better than runtime error
// 6. all structs require auth (even margin requires header)
// 7. no default arguments for ws streams when using threads. Must specify...

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
inline char binary_to_hex_digit(unsigned a);
std::string binary_to_hex(unsigned char const* binary, unsigned binary_len);
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
	std::string _host; // not const because of testnet
	std::string _port;
	bool _reconnect_on_error;

	template <class FT>
	void _connect_to_endpoint(std::string stream_map_name, std::string& buf, FT& functor, std::pair<RestSession*,
		std::string> user_stream_pair = std::make_pair<RestSession*, std::string>(nullptr, ""));

public:
	WebsocketClient(std::string host, unsigned int port);

	std::unordered_map<std::string, bool> running_streams; // will be a map, containing pairs of: <bool(status), ws_stream> 

	void close_stream(const std::string stream_name);
	std::vector<std::string> open_streams();
	bool is_open(const std::string& stream_name) const;
	unsigned int refresh_listenkey_interval;

	template <class FT>
	void _stream_manager(std::string stream_map_name, std::string& buf, FT& functor, std::pair<RestSession*,
		std::string> user_stream_pair = std::make_pair<RestSession*, std::string>(nullptr, ""));

	void _set_reconnect(const bool& reconnect);

	void set_host_port(const std::string& new_host, const unsigned int& new_port);

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
	Params& operator=(Params&& params_obj);

	std::unordered_map<std::string, std::string> param_map;
	bool default_recv;
	unsigned int default_recv_amt;

	template <typename PT>
	void set_param(const std::string& key, const PT& value);
	template <typename PT>
	void set_param(const std::string& key, PT&& value);

	bool delete_param(const std::string& key);

	void set_recv(const bool& set_always, const unsigned int& recv_val = 0);

	bool empty() const;


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

	std::string _generate_query(const Params* params_ptr, const bool& sign_query = 0);





	// ----------------------CRTP methods

	// Market Data endpoints

	bool ping_client();
	unsigned long long exchange_time();
	Json::Value exchange_info();
	Json::Value order_book(const Params* params_ptr);
	Json::Value public_trades_recent(const Params* params_ptr);
	Json::Value public_trades_historical(const Params* params_ptr);
	Json::Value public_trades_agg(const Params* params_ptr);
	Json::Value klines(const Params* params_ptr);
	Json::Value daily_ticker_stats(const Params* params_ptr = nullptr);
	Json::Value get_ticker(const Params* params_ptr = nullptr);
	Json::Value get_order_book_ticker(const Params* params_ptr = nullptr);


	// Trading endpoints

	Json::Value test_new_order(const Params* params_ptr);
	Json::Value new_order(const Params* params_ptr);
	Json::Value cancel_order(const Params* params_ptr);
	Json::Value cancel_all_orders(const Params* params_ptr);
	Json::Value query_order(const Params* params_ptr);
	Json::Value open_orders(const Params* params_ptr = nullptr);
	Json::Value all_orders(const Params* params_ptr);
	Json::Value account_info(const Params* params_ptr = nullptr);
	Json::Value account_trades_list(const Params* params_ptr);

	// WS Streams

	template <class FT>
	unsigned int stream_aggTrade(const std::string& symbol, std::string& buffer, FT& functor);

	template <class FT>
	unsigned int stream_Trade(const std::string& symbol, std::string& buffer, FT& functor); // todo: only for spot

	template <class FT>
	unsigned int stream_kline(const std::string& symbol, std::string& buffer, FT& functor, std::string interval = "1h");

	template <class FT>
	unsigned int stream_ticker_ind_mini(const std::string& symbol, std::string& buffer, FT& functor);

	template <class FT>
	unsigned int stream_ticker_all_mini(std::string& buffer, FT& functor);

	template <class FT>
	unsigned int stream_ticker_ind(const std::string& symbol, std::string& buffer, FT& functor);

	template <class FT>
	unsigned int stream_ticker_all(std::string& buffer, FT& functor);

	template <class FT>
	unsigned int stream_ticker_ind_book(const std::string& symbol, std::string& buffer, FT& functor);

	template <class FT>
	unsigned int stream_ticker_all_book(std::string& buffer, FT& functor);

	template <class FT>
	unsigned int stream_depth_partial(const std::string& symbol, std::string& buffer, FT& functor, const unsigned int levels = 5, const unsigned int interval = 100); // todo: different intervals for different fronts

	template <class FT>
	unsigned int stream_depth_diff(const std::string& symbol, std::string& buffer, FT& functor, const unsigned int interval = 100);

	template <class FT>
	unsigned int stream_userStream(std::string& buffer, FT& functor); // todo: for margin, spot, etc...

	std::string get_listen_key();
	std::string ping_listen_key(const std::string& listen_key = "");
	std::string revoke_listen_key(const std::string& listen_key = "");


	// Library methods

	bool init_ws_session();
	void close_stream(const std::string& symbol, const std::string& stream_name);
	bool is_stream_open(const std::string& symbol, const std::string& stream_name);
	std::vector<std::string> get_open_streams();
	void ws_auto_reconnect(const bool& reconnect);
	inline void set_refresh_key_interval(const bool val);


	// ----------------------end CRTP methods

	bool init_rest_session();
	bool set_headers(RestSession* rest_client);
	void rest_set_verbose(const bool& state);

	// Global requests (wallet, account etc)

	bool exchange_status();

	struct Wallet
	{
		Client<T>* user_client;
		explicit Wallet(Client<T>& client);
		explicit Wallet(const Client<T>& client);
		~Wallet();

		Json::Value get_all_coins(const Params* params_ptr = nullptr);
		Json::Value daily_snapshot(const Params* params_ptr);
		Json::Value fast_withdraw_switch(const bool& state);
		Json::Value withdraw_balances(const Params* params_ptr, const bool& SAPI = 0);
		Json::Value deposit_history(const Params* params_ptr = nullptr, const bool& network = 0);
		Json::Value withdraw_history(const Params* params_ptr = nullptr, const bool& network = 0);
		Json::Value deposit_address(const Params* params_ptr, const bool& network = 0);
		Json::Value account_status(const Params* params_ptr = nullptr);
		Json::Value account_status_api(const Params* params_ptr = nullptr);
		Json::Value dust_log(const Params* params_ptr = nullptr);
		Json::Value dust_transfer(const Params* params_ptr);
		Json::Value asset_dividend_records(const Params* params_ptr = nullptr);
		Json::Value asset_details(const Params* params_ptr = nullptr);
		Json::Value trading_fees(const Params* params_ptr = nullptr);
	};

	struct FuturesWallet
	{
		Client<T>* user_client;
		explicit FuturesWallet(Client<T>& client);
		explicit FuturesWallet(const Client<T>& client);
		~FuturesWallet();

		Json::Value futures_transfer(const Params* params_ptr);
		Json::Value futures_transfer_history(const Params* params_ptr);
		Json::Value collateral_borrow(const Params* params_ptr);
		Json::Value collateral_borrow_history(const Params* params_ptr = nullptr);
		Json::Value collateral_repay(const Params* params_ptr);
		Json::Value collateral_repay_history(const Params* params_ptr = nullptr);
		Json::Value collateral_wallet(const Params* params_ptr = nullptr);
		Json::Value collateral_info(const Params* params_ptr = nullptr);
		Json::Value collateral_adjust_calc_rate(const Params* params_ptr);
		Json::Value collateral_adjust_get_max(const Params* params_ptr);
		Json::Value collateral_adjust(const Params* params_ptr);
		Json::Value collateral_adjust_history(const Params* params_ptr = nullptr);
		Json::Value collateral_liquidation_history(const Params* params_ptr = nullptr);

	};

	struct SubAccount // for corporate accounts
	{
		Client<T>* user_client;
		explicit SubAccount(Client<T>& client);
		explicit SubAccount(const Client<T>& client);
		~SubAccount();

		Json::Value get_all_subaccounts(const Params* params_ptr = nullptr);

		Json::Value transfer_master_history(const Params* params_ptr);
		Json::Value transfer_master_to_subaccount(const Params* params_ptr);

		Json::Value get_subaccount_balances(const Params* params_ptr);
		Json::Value get_subaccount_deposit_address(const Params* params_ptr);
		Json::Value get_subaccount_deposit_history(const Params* params_ptr);
		Json::Value get_subaccount_future_margin_status(const Params* params_ptr = nullptr);

		Json::Value enable_subaccount_margin(const Params* params_ptr);
		Json::Value get_subaccount_margin_status(const Params* params_ptr);
		Json::Value get_subaccount_margin_summary(const Params* params_ptr = nullptr);

		Json::Value enable_subaccount_futures(const Params* params_ptr);
		Json::Value get_subaccount_futures_status(const Params* params_ptr);
		Json::Value get_subaccount_futures_summary(const Params* params_ptr = nullptr);
		Json::Value get_subaccount_futures_positionrisk(const Params* params_ptr);

		Json::Value transfer_to_subaccount_futures(const Params* params_ptr);
		Json::Value transfer_to_subaccount_margin(const Params* params_ptr);
		Json::Value transfer_subaccount_to_subaccount(const Params* params_ptr);
		Json::Value transfer_subaccount_to_master(const Params* params_ptr);
		Json::Value transfer_subaccount_history(const Params* params_ptr);

	};

	struct MarginAccount
	{
		Client<T>* user_client;
		explicit MarginAccount(Client<T>& client);
		explicit MarginAccount(const Client<T>& client);
		~MarginAccount();

		Json::Value margin_transfer(const Params* params_ptr);
		Json::Value margin_borrow(const Params* params_ptr);
		Json::Value margin_repay(const Params* params_ptr);
		Json::Value margin_asset_query(const Params* params_ptr);
		Json::Value margin_pair_query(const Params* params_ptr);
		Json::Value margin_all_assets_query();
		Json::Value margin_all_pairs_query();
		Json::Value margin_price_index(const Params* params_ptr);
		Json::Value margin_new_order(const Params* params_ptr);
		Json::Value margin_cancel_order(const Params* params_ptr);
		Json::Value margin_transfer_history(const Params* params_ptr = nullptr);
		Json::Value margin_loan_record(const Params* params_ptr);
		Json::Value margin_repay_record(const Params* params_ptr);
		Json::Value margin_interest_history(const Params* params_ptr = nullptr);
		Json::Value margin_liquidations_record(const Params* params_ptr = nullptr);
		Json::Value margin_account_info(const Params* params_ptr = nullptr);
		Json::Value margin_account_order(const Params* params_ptr);
		Json::Value margin_account_open_orders(const Params* params_ptr = nullptr);
		Json::Value margin_account_all_orders(const Params* params_ptr);
		Json::Value margin_account_trades_list(const Params* params_ptr);
		Json::Value margin_max_borrow(const Params* params_ptr);
		Json::Value margin_max_transfer(const Params* params_ptr);
		Json::Value margin_isolated_margin_create(const Params* params_ptr);
		Json::Value margin_isolated_margin_transfer(const Params* params_ptr);
		Json::Value margin_isolated_margin_transfer_history(const Params* params_ptr);
		Json::Value margin_isolated_margin_account_info(const Params* params_ptr = nullptr);
		Json::Value margin_isolated_margin_symbol(const Params* params_ptr);
		Json::Value margin_isolated_margin_symbol_all(const Params* params_ptr = nullptr);

		template <class FT>
		unsigned int margin_stream_userStream(std::string& buffer, FT& functor, const bool& isolated_margin_type = 0);
		std::string margin_get_listen_key(const bool& isolated_margin_type = 0);
		std::string margin_ping_listen_key(const std::string& listen_key, const bool& isolated_margin_type = 0);
		std::string margin_revoke_listen_key(const std::string& listen_key, const bool& isolated_margin_type = 0);

	};

	struct Savings
	{
		Client<T>* user_client;
		explicit Savings(Client<T>& client);
		explicit Savings(const Client<T>& client);
		~Savings();

		Json::Value get_product_list_flexible(const Params* params_ptr = nullptr);
		Json::Value get_product_daily_quota_purchase_flexible(const Params* params_ptr);
		Json::Value purchase_product_flexible(const Params* params_ptr);
		Json::Value get_product_daily_quota_redemption_flexible(const Params* params_ptr);
		Json::Value redeem_product_flexible(const Params* params_ptr);
		Json::Value get_product_position_flexible(const Params* params_ptr);
		Json::Value get_product_list_fixed(const Params* params_ptr);
		Json::Value purchase_product_fixed(const Params* params_ptr);
		Json::Value get_product_position_fixed(const Params* params_ptr);
		Json::Value lending_account(const Params* params_ptr = nullptr);
		Json::Value get_purchase_record(const Params* params_ptr);
		Json::Value get_redemption_record(const Params* params_ptr);
		Json::Value get_interest_history(const Params* params_ptr);

	};

	struct Mining
	{
		Client<T>* user_client;
		explicit Mining(Client<T>& client);
		explicit Mining(const Client<T>& client);
		~Mining();

		Json::Value algo_list();
		Json::Value coin_list();
		Json::Value get_miner_list_detail(const Params* params_ptr);
		Json::Value get_miner_list(const Params* params_ptr);
		Json::Value revenue_list(const Params* params_ptr);
		Json::Value statistic_list(const Params* params_ptr);
		Json::Value account_list(const Params* params_ptr);
	};

	Json::Value custom_get_req(const std::string& base, const std::string& endpoint, const Params* params_ptr, const bool& signature = 0);
	Json::Value custom_post_req(const std::string& base, const std::string& endpoint, const Params* params_ptr, const bool& signature = 0);
	Json::Value custom_put_req(const std::string& base, const std::string& endpoint, const Params* params_ptr, const bool& signature = 0);
	Json::Value custom_delete_req(const std::string& base, const std::string& endpoint, const Params* params_ptr, const bool& signature = 0);

	template <typename FT>
	unsigned int custom_stream(std::string stream_query, std::string buffer, FT functor);

	RestSession* _rest_client = nullptr; // todo: move init
	WebsocketClient* _ws_client = nullptr; // todo: move init, leave decl

	~Client();

};


template <typename CT> // CT = coin type
class FuturesClient : public Client<FuturesClient<CT>>
{
private:
	inline bool v_init_ws_session();
	inline void v_close_stream(const std::string& symbol, const std::string& stream_name);
	inline bool v_is_stream_open(const std::string& symbol, const std::string& stream_name);
	inline std::vector<std::string> v_get_open_streams();
	inline void v_ws_auto_reconnect(const bool& reconnect);
	inline void v_set_refresh_key_interval(const bool val);


public:
	friend Client<FuturesClient<CT>>;
	bool _testnet_mode;

	FuturesClient();
	FuturesClient(std::string key, std::string secret);

	inline void set_testnet_mode(bool status);
	inline bool get_testnet_mode();


	// ------------------- crtp for all (spot + coin/usdt)

	// market data

	inline bool v_ping_client();  // todo: define lower levels
	inline unsigned long long v_exchange_time();
	Json::Value v_exchange_info();
	Json::Value v_order_book(const Params* params_ptr);
	Json::Value v_public_trades_recent(const Params* params_ptr);
	Json::Value v_public_trades_historical(const Params* params_ptr);
	Json::Value v_public_trades_agg(const Params* params_ptr);
	Json::Value v_klines(const Params* params_ptr);
	Json::Value v_daily_ticker_stats(const Params* params_ptr);
	Json::Value v_get_ticker(const Params* params_ptr);
	Json::Value v_get_order_book_ticker(const Params* params_ptr);

	// trading endpoints

	// -- mutual with spot

	Json::Value v_test_new_order(const Params* params_ptr);
	Json::Value v_new_order(const Params* params_ptr);
	Json::Value v_cancel_order(const Params* params_ptr);
	Json::Value v_cancel_all_orders(const Params* params_ptr);
	Json::Value v_query_order(const Params* params_ptr);
	Json::Value v_open_orders(const Params* params_ptr);
	Json::Value v_all_orders(const Params* params_ptr);
	Json::Value v_account_info(const Params* params_ptr);
	Json::Value v_account_trades_list(const Params* params_ptr);

	// -- unique to future endpoints

	Json::Value change_position_mode(const Params* params_ptr);
	Json::Value get_position_mode(const Params* params_ptr = nullptr);
	Json::Value batch_orders(const Params* params_ptr);
	Json::Value cancel_batch_orders(const Params* params_ptr);
	Json::Value cancel_all_orders_timer(const Params* params_ptr);
	Json::Value query_open_order(const Params* params_ptr);
	Json::Value account_balances(const Params* params_ptr = nullptr);
	Json::Value change_leverage(const Params* params_ptr);
	Json::Value change_margin_type(const Params* params_ptr);
	Json::Value change_position_margin(const Params* params_ptr);
	Json::Value change_position_margin_history(const Params* params_ptr);
	Json::Value position_info(const Params* params_ptr = nullptr);
	Json::Value get_income_history(const Params* params_ptr);
	Json::Value get_leverage_bracket(const Params* params_ptr = nullptr);


	// -- unique to USDT endpoint

	Json::Value pos_adl_quantile_est(const Params* params_ptr = nullptr); // todo: define, default param

	// global for 'futures' methods. note: base path is spot



	// -------------------  inter-future crtp ONLY

	// todo: exception for bad_endpoint or nonexisting

	 // market Data

	Json::Value mark_price(const Params* params_ptr = nullptr); // todo: define, crtp? default param
	Json::Value public_liquidation_orders(const Params* params_ptr); // todo: define, crtp?
	Json::Value open_interest(const Params* params_ptr); // todo: define, crtp?


	// note that the following four might be only for coin margined market data
	Json::Value continues_klines(const Params* params_ptr);
	Json::Value index_klines(const Params* params_ptr);
	Json::Value mark_klines(const Params* params_ptr);

	// note that the following four might be only for coin margined market data

	Json::Value funding_rate_history(const Params* params_ptr);

	// WS Streams

// -- Global that are going deeper to USDT and COIN

	template <class FT>
	unsigned int v_stream_Trade(std::string symbol, std::string& buffer, FT& functor);


	// -- going deeper...

	// todo: define global for both

	template <class FT>
	unsigned int stream_markprice(std::string symbol, std::string& buffer, FT& functor, unsigned int interval = 1000);

	template <class FT>
	unsigned int stream_liquidation_orders(std::string symbol, std::string& buffer, FT& functor);

	template <class FT>
	unsigned int stream_liquidation_orders_all(std::string& buffer, FT& functor);

	template <class FT>
	unsigned int stream_markprice_all(std::string pair, std::string& buffer, FT& functor); // only USDT

	template <class FT>
	unsigned int stream_indexprice(std::string pair, std::string& buffer, FT& functor, unsigned int interval = 1000); // only Coin

	template <class FT>
	unsigned int stream_markprice_by_pair(std::string& pair, std::string& buffer, FT& functor, unsigned int interval = 1000); // only coin

	template <class FT>
	unsigned int stream_kline_contract(std::string pair_and_type, std::string& buffer, FT& functor, std::string interval = "1h"); // only coin

	template <class FT>
	unsigned int stream_kline_index(std::string pair, std::string& buffer, FT& functor, std::string interval = "1h"); // only coin

	template <class FT>
	unsigned int stream_kline_markprice(std::string symbol, std::string& buffer, FT& functor, std::string interval = "1h"); // only coin

	template <class FT>
	unsigned int v_stream_userStream(std::string& buffer, FT& functor);

	std::string v_get_listen_key(); 
	std::string v_ping_listen_key(const std::string& listen_key); 
	std::string v_revoke_listen_key(const std::string& listen_key); 



	// end CRTP

	// endpoints are same for both wallet types below

	Json::Value open_interest_stats(const Params* params_ptr);
	Json::Value top_long_short_ratio(const Params* params_ptr, bool accounts = 0);
	Json::Value global_long_short_ratio(const Params* params_ptr);
	Json::Value taker_long_short_ratio(const Params* params_ptr);
	Json::Value basis_data(const Params* params_ptr);


	~FuturesClient();
};


class FuturesClientUSDT : public FuturesClient<FuturesClientUSDT>
{
public:
	friend FuturesClient;

	FuturesClientUSDT();
	FuturesClientUSDT(std::string key, std::string secret);
	bool v__init_ws_session();
	void v_set_testnet_mode(bool status);


	// up to Client level

	inline bool v__ping_client();
	inline unsigned long long v__exchange_time();
	Json::Value v__exchange_info();
	Json::Value v__order_book(const Params* params_ptr);
	Json::Value v__public_trades_recent(const Params* params_ptr);
	Json::Value v__public_trades_historical(const Params* params_ptr);
	Json::Value v__public_trades_agg(const Params* params_ptr);
	Json::Value v__klines(const Params* params_ptr);
	Json::Value v__daily_ticker_stats(const Params* params_ptr);
	Json::Value v__get_ticker(const Params* params_ptr);
	Json::Value v__get_order_book_ticker(const Params* params_ptr);

	// market Data

	Json::Value v_mark_price(const Params* params_ptr);
	Json::Value v_public_liquidation_orders(const Params* params_ptr);
	Json::Value v_open_interest(const Params* params_ptr);


	// note that the following four might be only for coin margined market data
	Json::Value v_continues_klines(const Params* params_ptr);
	Json::Value v_index_klines(const Params* params_ptr);
	Json::Value v_mark_klines(const Params* params_ptr);

	// note that the following four might be only for usdt margined market data

	Json::Value v_funding_rate_history(const Params* params_ptr);


	// trading endpoints

	// -- mutual with spot

	Json::Value v__new_order(const Params* params_ptr);
	Json::Value v__cancel_order(const Params* params_ptr);
	Json::Value v__cancel_all_orders(const Params* params_ptr);
	Json::Value v__query_order(const Params* params_ptr);
	Json::Value v__open_orders(const Params* params_ptr);
	Json::Value v__all_orders(const Params* params_ptr);
	Json::Value v__account_info(const Params* params_ptr);
	Json::Value v__account_trades_list(const Params* params_ptr);

	// -- unique to future endpoints

	Json::Value v_change_position_mode(const Params* params_ptr);
	Json::Value v_get_position_mode(const Params* params_ptr);
	Json::Value v_batch_orders(const Params* params_ptr);
	Json::Value v_cancel_batch_orders(const Params* params_ptr);
	Json::Value v_cancel_all_orders_timer(const Params* params_ptr);
	Json::Value v_query_open_order(const Params* params_ptr);
	Json::Value v_account_balances(const Params* params_ptr);
	Json::Value v_change_leverage(const Params* params_ptr);
	Json::Value v_change_margin_type(const Params* params_ptr);
	Json::Value v_change_position_margin(const Params* params_ptr);
	Json::Value v_change_position_margin_history(const Params* params_ptr);
	Json::Value v_position_info(const Params* params_ptr);
	Json::Value v_get_income_history(const Params* params_ptr);
	Json::Value v_get_leverage_bracket(const Params* params_ptr);


	// -- unique to USDT endpoint

	Json::Value v_pos_adl_quantile_est(const Params* params_ptr); 


	// WS Streams

	// -- Global that are going deeper to USDT and COIN



	// -- going deeper...


	template <class FT>
	unsigned int v_stream_markprice_all(std::string pair, std::string& buffer, FT& functor); // only USDT

	template <class FT>
	unsigned int v_stream_indexprice(std::string pair, std::string& buffer, FT& functor, unsigned int interval); // only Coin

	template <class FT>
	unsigned int v_stream_markprice_by_pair(std::string& pair, std::string& buffer, FT& functor, unsigned int interval); // only coin

	template <class FT>
	unsigned int v_stream_kline_contract(std::string pair_and_type, std::string& buffer, FT& functor, std::string interval); // only coin

	template <class FT>
	unsigned int v_stream_kline_index(std::string pair, std::string& buffer, FT& functor, std::string interval); // only coin

	template <class FT>
	unsigned int v_stream_kline_markprice(std::string symbol, std::string& buffer, FT& functor, std::string interval); // only coin

	template <class FT>
	unsigned int v__stream_userStream(std::string& buffer, FT& functor);

	std::string v__get_listen_key();
	std::string v__ping_listen_key(); 
	std::string v__revoke_listen_key();


	~FuturesClientUSDT();
};


class FuturesClientCoin : public FuturesClient<FuturesClientCoin>
{
public:
	friend FuturesClient;

	FuturesClientCoin();
	FuturesClientCoin(std::string key, std::string secret);
	bool v__init_ws_session();
	void v_set_testnet_mode(bool status);


	// up to Client level

	inline bool v__ping_client();
	inline unsigned long long v__exchange_time();
	Json::Value v__exchange_info();
	Json::Value v__order_book(const Params* params_ptr);
	Json::Value v__public_trades_recent(const Params* params_ptr);
	Json::Value v__public_trades_historical(const Params* params_ptr);
	Json::Value v__public_trades_agg(const Params* params_ptr);
	Json::Value v__klines(const Params* params_ptr);
	Json::Value v__daily_ticker_stats(const Params* params_ptr);
	Json::Value v__get_ticker(const Params* params_ptr);
	Json::Value v__get_order_book_ticker(const Params* params_ptr);

	// market Data

	Json::Value v_mark_price(const Params* params_ptr);
	Json::Value v_public_liquidation_orders(const Params* params_ptr);
	Json::Value v_open_interest(const Params* params_ptr);


	// note that the following four might be only for coin margined market data
	Json::Value v_continues_klines(const Params* params_ptr);
	Json::Value v_index_klines(const Params* params_ptr);
	Json::Value v_mark_klines(const Params* params_ptr);

	// note that the following four might be only for coin margined market data

	Json::Value v_funding_rate_history(const Params* params_ptr);

	// trading endpoints

// -- mutual with spot

	Json::Value v__new_order(const Params* params_ptr);
	Json::Value v__cancel_order(const Params* params_ptr);
	Json::Value v__cancel_all_orders(const Params* params_ptr);
	Json::Value v__query_order(const Params* params_ptr);
	Json::Value v__open_orders(const Params* params_ptr);
	Json::Value v__all_orders(const Params* params_ptr);
	Json::Value v__account_info(const Params* params_ptr);
	Json::Value v__account_trades_list(const Params* params_ptr);

	// -- unique to future endpoints

	Json::Value v_change_position_mode(const Params* params_ptr);
	Json::Value v_get_position_mode(const Params* params_ptr);
	Json::Value v_batch_orders(const Params* params_ptr);
	Json::Value v_cancel_batch_orders(const Params* params_ptr);
	Json::Value v_cancel_all_orders_timer(const Params* params_ptr);
	Json::Value v_query_open_order(const Params* params_ptr);
	Json::Value v_account_balances(const Params* params_ptr);
	Json::Value v_change_leverage(const Params* params_ptr);
	Json::Value v_change_margin_type(const Params* params_ptr);
	Json::Value v_change_position_margin(const Params* params_ptr);
	Json::Value v_change_position_margin_history(const Params* params_ptr);
	Json::Value v_position_info(const Params* params_ptr);
	Json::Value v_get_income_history(const Params* params_ptr);
	Json::Value v_get_leverage_bracket(const Params* params_ptr);


	// -- unique to USDT endpoint

	Json::Value v_pos_adl_quantile_est(const Params* params_ptr);

	// WS Streams


	// -- going deeper...

	template <class FT>
	unsigned int v_stream_markprice_all(std::string pair, std::string& buffer, FT& functor); // only USDT

	template <class FT>
	unsigned int v_stream_indexprice(std::string pair, std::string& buffer, FT& functor, unsigned int interval); // only Coin

	template <class FT>
	unsigned int v_stream_markprice_by_pair(std::string& pair, std::string& buffer, FT& functor, unsigned int interval); // only coin

	template <class FT>
	unsigned int v_stream_kline_contract(std::string pair_and_type, std::string& buffer, FT& functor, std::string interval); // only coin

	template <class FT>
	unsigned int v_stream_kline_index(std::string pair, std::string& buffer, FT& functor, std::string interval); // only coin

	template <class FT>
	unsigned int v_stream_kline_markprice(std::string symbol, std::string& buffer, FT& functor, std::string interval); // only coin

	template <class FT>
	unsigned int v__stream_userStream(std::string& buffer, FT& functor); 

	std::string v__get_listen_key();
	std::string v__ping_listen_key();
	std::string v__revoke_listen_key();

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
	Json::Value v_order_book(const Params* params_ptr);
	Json::Value v_public_trades_recent(const Params* params_ptr);
	Json::Value v_public_trades_historical(const Params* params_ptr);
	Json::Value v_public_trades_agg(const Params* params_ptr);
	Json::Value v_klines(const Params* params_ptr);
	Json::Value v_daily_ticker_stats(const Params* params_ptr);
	Json::Value v_get_ticker(const Params* params_ptr);
	Json::Value v_get_order_book_ticker(const Params* params_ptr);

	// ------------------- crtp global end

	// Trading endpoints

	// ---- CRTP implementations

	Json::Value v_test_new_order(const Params* params_ptr);
	Json::Value v_new_order(const Params* params_ptr);
	Json::Value v_cancel_order(const Params* params_ptr);
	Json::Value v_cancel_all_orders(const Params* params_ptr);
	Json::Value v_query_order(const Params* params_ptr);
	Json::Value v_open_orders(const Params* params_ptr);
	Json::Value v_all_orders(const Params* params_ptr);
	Json::Value v_account_info(const Params* params_ptr);
	Json::Value v_account_trades_list(const Params* params_ptr);

	// ---- general methods

	Json::Value oco_new_order(const Params* params_ptr);
	Json::Value oco_cancel_order(const Params* params_ptr);
	Json::Value oco_query_order(const Params* params_ptr = nullptr);
	Json::Value oco_all_orders(const Params* params_ptr = nullptr);
	Json::Value oco_open_orders(const Params* params_ptr = nullptr);

	// WS Streams

	template <class FT>
	unsigned int v_stream_Trade(std::string symbol, std::string& buffer, FT& functor); // todo: only spot


	// crtp infrastructure start

	bool v_init_ws_session();

	template <class FT>
	unsigned int v_stream_userStream(std::string& buffer, FT& functor);
	std::string v_get_listen_key();
	std::string v_ping_listen_key(const std::string& listen_key);
	std::string v_revoke_listen_key(const std::string& listen_key);

	void v_close_stream(const std::string& symbol, const std::string& stream_name);
	bool v_is_stream_open(const std::string& symbol, const std::string& stream_name);
	std::vector<std::string> v_get_open_streams();
	void v_ws_auto_reconnect(const bool& reconnect);
	inline void v_set_refresh_key_interval(const bool& val);

	// crtp infrastructure end , todo: make this more organized ofc



public:
	friend Client;

	SpotClient();
	SpotClient(std::string key, std::string secret);

	~SpotClient();
};

#endif