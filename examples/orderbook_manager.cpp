#include "../include/Binance_Client.h"
#include <iostream>
#include <thread>
#include <map>
#include <iomanip>


class OrderbookManager
{
    FuturesClientUSDT* user_client;
    Json::CharReaderBuilder charbuilder;
    Json::CharReader* charreader;
    std::string parse_errors;

    Json::Value get_initial_snap();
    static void append_initial_into_book(const Json::Value& record, std::map<float, float>& side);


public:
    const std::string symbol;
    std::string msg_buffer;
    Json::Value stream_msg;

    std::map<float, float> bids;
    std::map<float, float> asks;

    explicit OrderbookManager(const std::string ticker_symbol, FuturesClientUSDT& client_init);
    OrderbookManager operator()(const std::string& response);


    void setup_initial_snap();
    void get_best_bid();
    void get_best_ask();
};


int main()
{
    FuturesClientUSDT public_client{};
    OrderbookManager btcusdt_orderbook{ "btcusdt", public_client };

    std::thread t4(&FuturesClientUSDT::stream_depth_diff<OrderbookManager>, std::ref(public_client), btcusdt_orderbook.symbol, std::ref(btcusdt_orderbook.msg_buffer), std::ref(btcusdt_orderbook), 100);

    btcusdt_orderbook.setup_initial_snap();

    while(1)
    {
        std::cout << "\nbest bid    ";
        btcusdt_orderbook.get_best_bid();
        std::cout << "\nbest ask    ";
        btcusdt_orderbook.get_best_ask();
        std::cout << "\n========";
        Sleep(3000);
    }

    t4.join();
    return 0;
}


 OrderbookManager::OrderbookManager(const std::string ticker_symbol, FuturesClientUSDT& client_init)
    : symbol{ ticker_symbol }, user_client{ &client_init }, msg_buffer{ "" }, parse_errors{ }, charreader{ charbuilder.newCharReader() }
{}

Json::Value OrderbookManager::get_initial_snap()
{
    Params req_params{};
    req_params.set_param<int>("limit", 500);
    req_params.set_param<std::string>("symbol", symbol);
    return user_client->order_book(&req_params);
}


void OrderbookManager::append_initial_into_book(const Json::Value& record, std::map<float, float>& side)
{
    for (Json::ValueConstIterator itr = record.begin(); itr != record.end(); itr++)
    {
        std::string val = itr->toStyledString();
        if (val.find("null") != std::string::npos) continue;

        std::vector<unsigned int> positions_of_quotes{};
        bool finished{ 0 };

        while (!finished)
        {
            unsigned int pos = val.find('"');
            positions_of_quotes.push_back(pos);
            val.replace(pos, 1, " ");

            if (positions_of_quotes.size() == 4) break;
        }

        float price_str = std::stof(val.substr(positions_of_quotes[0], positions_of_quotes[1] - positions_of_quotes[0]));
        float quantity_str = std::stof(val.substr(positions_of_quotes[2], positions_of_quotes[3] - positions_of_quotes[2]));

        side[price_str] = quantity_str;
    }
}

OrderbookManager OrderbookManager::operator()(const std::string& response)
{
    this->charreader->parse(response.c_str(),
        response.c_str() + response.size(),
        &this->stream_msg,
        &this->parse_errors);

    Json::Value asks_resp = this->stream_msg["a"];
    Json::Value bids_resp = this->stream_msg["b"];

    OrderbookManager::append_initial_into_book(asks_resp, this->asks);
    OrderbookManager::append_initial_into_book(bids_resp, this->bids);

    return *this;
}

void OrderbookManager::setup_initial_snap()
{
    Json::Value ex_response = this->get_initial_snap()["response"];
    Json::Value asks_resp = ex_response["asks"];
    Json::Value bids_resp = ex_response["bids"];

    OrderbookManager::append_initial_into_book(asks_resp, this->asks);
    OrderbookManager::append_initial_into_book(bids_resp, this->bids);

}


void OrderbookManager::get_best_bid()
{
    std::map<float, float>::const_reverse_iterator itr;

    for (itr = bids.rbegin(); itr != bids.rend(); itr++)
    {
        if (itr->second != 0)
        {
            std::cout << std::setprecision(8) << itr->first << ": " << itr->second;
            break;
        }
    }
}

void OrderbookManager::get_best_ask()
{
    std::map<float, float>::const_iterator itr;

    for (itr = asks.begin(); itr != asks.end(); itr++)
    {
        if (itr->second != 0)
        {
            std::cout << std::setprecision(8) << itr->first << ": " << itr->second;
            break;
        }
    }
}