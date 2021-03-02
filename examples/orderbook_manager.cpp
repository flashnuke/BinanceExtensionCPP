#include "../include/Binance_Client.h"
#include <iostream>
#include <thread>
#include <map>
#include <iomanip>
#include <mutex>

std::mutex ob_mutex;

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
    std::string msg_buffer;
    Json::Value stream_msg;

    std::vector<std::pair<double, double>> bids;
    std::vector<std::pair<double, double>> asks;

    explicit OrderbookManager(const std::string ticker_symbol, FuturesClientUSDT& client_init);
    OrderbookManager operator()(const std::string& response);


    void setup_initial_snap();
    void print_best_bid();
    void print_best_ask();
};


int main()
{
    FuturesClientUSDT public_client{};
    OrderbookManager btcusdt_orderbook{ "btcusdt", public_client };

    std::thread t4(&FuturesClientUSDT::stream_depth_diff<OrderbookManager>, std::ref(public_client), btcusdt_orderbook.symbol, std::ref(btcusdt_orderbook.msg_buffer), std::ref(btcusdt_orderbook), 100);

    btcusdt_orderbook.setup_initial_snap();

    while (1)
    {
        std::cout << "\nbest bid    ";
        btcusdt_orderbook.print_best_bid();
        std::cout << "\nbest ask    ";
        btcusdt_orderbook.print_best_ask();
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
    std::lock_guard<std::mutex> guard(ob_mutex);
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

        double price = std::stod(val.substr(positions_of_quotes[0], positions_of_quotes[1] - positions_of_quotes[0]));
        double quantity = std::stod(val.substr(positions_of_quotes[2], positions_of_quotes[3] - positions_of_quotes[2]));

        if (quantity != 0)
        {
            std::cout << val << "\n"; // todo remove
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


void OrderbookManager::print_best_bid()
{
    std::vector<std::pair<double, double>>::const_iterator itr;
    std::vector<std::pair<double, double>> cpy_b{ bids };
    for (itr = cpy_b.begin(); itr != cpy_b.begin() + 10; itr++)
    {
        //std::cout << itr->first << "   " << itr->second << "\n";
    }
    std::pair<double, double> layer = cpy_b.front();
    std::cout << layer.first << " <:> " << layer.second;

}

void OrderbookManager::print_best_ask()
{
    std::pair<double, double> layer = this->asks.back();
    std::cout << layer.first << " : " << layer.second;
}