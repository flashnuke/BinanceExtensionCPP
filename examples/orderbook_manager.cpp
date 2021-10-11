#include "../include/Binance_Client.h"
#include <iostream>
#include <thread>
#include <map>
#include <iomanip>
#include <mutex>

std::mutex ob_mutex;

#ifdef __unix__
# include <unistd.h>
#elif defined _WIN32
# include <windows.h>
#define sleep(x) Sleep(1000 * (x))
#endif

class OrderbookManager
{
    FuturesClientUSDT* user_client;
    Json::CharReaderBuilder charbuilder;
    Json::CharReader* charreader;
    std::string parse_errors;

    Json::Value get_initial_snap();
    static void append_initial_into_book(const Json::Value& record, std::vector<std::pair<double, double>>& side);
    static unsigned int insert_layer(std::vector<std::pair<double, double>>& side, double price, double quantity);
    static void remove_layer(std::vector<std::pair<double, double>>& side, double price);


public:
    const std::string symbol;

    std::vector<std::pair<double, double>> bids;
    std::vector<std::pair<double, double>> asks;

    explicit OrderbookManager(const std::string ticker_symbol, FuturesClientUSDT& client_init);
    OrderbookManager operator()(const std::string& response);

    void reset_order_book(std::vector<std::pair<double, double>>& side);
    void setup_initial_snap();
    void print_top_layers();
    void print_best_bid();
    void print_best_ask();
};


int main()
{
    FuturesClientUSDT public_client{};
    OrderbookManager btcusdt_orderbook{ "btcusdt", public_client };

    std::thread t4(&FuturesClientUSDT::stream_depth_partial<OrderbookManager>, std::ref(public_client), btcusdt_orderbook.symbol, std::ref(btcusdt_orderbook), 5, 100);
    btcusdt_orderbook.setup_initial_snap();

    while (1)
    {
        btcusdt_orderbook.print_top_layers();
        sleep(3);
    }

    t4.join();
    return 0;
}


OrderbookManager::OrderbookManager(const std::string ticker_symbol, FuturesClientUSDT& client_init)
    : symbol{ ticker_symbol }, user_client{ &client_init }, parse_errors{ }, charreader{ charbuilder.newCharReader() }
{}

void OrderbookManager::reset_order_book(std::vector<std::pair<double, double>>& side)
{
    side.clear();
}

Json::Value OrderbookManager::get_initial_snap()
{
    Params req_params{};
    req_params.set_param<int>("limit", 10);
    req_params.set_param<std::string>("symbol", symbol);
    return user_client->order_book(&req_params);
}

unsigned int OrderbookManager::insert_layer(std::vector<std::pair<double, double>>& side, double price, double quantity)
{
    for (std::vector<std::pair<double, double>>::iterator itr = side.begin(); itr != side.end(); itr++)
    {
        double i_price = itr->first;
        if (i_price < price)
        {
            side.insert(itr, std::pair<double, double>(price, quantity));
            return 1;
        }
        else if (i_price == price)
        {
            itr->second = quantity;
            return 1;
        }
    }
    side.push_back(std::pair<double, double>(price, quantity));
    return 1;
}

void OrderbookManager::remove_layer(std::vector<std::pair<double, double>>& side, double price)
{
    for (std::vector<std::pair<double, double>>::iterator itr = side.begin(); itr != side.end(); itr++)
    {
        double i_price = itr->first;
        if (price == itr->first) 
        {
            side.erase(itr);
            break;
        }
    }
}

void OrderbookManager::append_initial_into_book(const Json::Value& record, std::vector<std::pair<double, double>>& side)
{

    for (Json::ValueConstIterator itr = record.begin(); itr != record.end(); itr++)
    {
        double price = std::stod((*itr)[0].asCString());
        double quantity = std::stod((*itr)[1].asCString());

        if (quantity != 0)
        {
            OrderbookManager::insert_layer(side, price, quantity);
        }
        else
        {
            OrderbookManager::remove_layer(side, price);
        }
    }
}

OrderbookManager OrderbookManager::operator()(const std::string& response)
{
    std::lock_guard<std::mutex> guard(ob_mutex);
    Json::Value stream_msg;

    this->charreader->parse(response.c_str(),
        response.c_str() + response.size(),
        &stream_msg,
        &this->parse_errors);

    Json::Value asks_resp = stream_msg["a"];
    Json::Value bids_resp = stream_msg["b"];

    this->reset_order_book(this->asks);
    OrderbookManager::append_initial_into_book(asks_resp, this->asks);
    this->reset_order_book(this->bids);
    OrderbookManager::append_initial_into_book(bids_resp, this->bids);

    return *this;
}

void OrderbookManager::setup_initial_snap()
{
    std::lock_guard<std::mutex> guard(ob_mutex);

    Json::Value ex_response = this->get_initial_snap()["response"];
    Json::Value asks_resp = ex_response["asks"];
    Json::Value bids_resp = ex_response["bids"];

    this->reset_order_book(this->asks);
    OrderbookManager::append_initial_into_book(asks_resp, this->asks);
    this->reset_order_book(this->bids);
    OrderbookManager::append_initial_into_book(bids_resp, this->bids);
}


void OrderbookManager::print_best_bid()
{

    std::vector<std::pair<double, double>>::const_iterator itr;
    std::vector<std::pair<double, double>> cpy_b{ bids };

    std::pair<double, double> layer = bids[0];
    std::cout << layer.first << " : " << layer.second;
}

void OrderbookManager::print_best_ask()
{
    std::vector<std::pair<double, double>>::const_iterator itr;
    std::vector<std::pair<double, double>> cpy_b{ asks };

    std::pair<double, double> layer = asks[asks.size() - 1];
    std::cout << layer.first << " : " << layer.second;
}

void OrderbookManager::print_top_layers()
{
    std::lock_guard<std::mutex> guard(ob_mutex);

    std::cout << std::setprecision(8) << "\nbest bid    ";
    this->print_best_bid();
    std::cout << "\nbest ask    ";
    this->print_best_ask();
    std::cout << "\n========";
}
