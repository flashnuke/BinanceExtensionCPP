#include "../include/Binance_Client.h"
#include <thread>
#include <iostream>

struct SomeFunctor
{
    Json::CharReaderBuilder charbuilder;
    Json::CharReader* charreader;
    std::string parse_errors;
    Json::Value stream_msg;

    SomeFunctor()
        : parse_errors{ }, charreader{ charbuilder.newCharReader() }
    {}


    SomeFunctor operator()(const std::string& response)
    {
        this->stream_msg.clear();
        this->parse_errors.clear();

        this->charreader->parse(response.c_str(),
            response.c_str() + response.size(),
            &this->stream_msg,
            &parse_errors);

        std::cout << this->stream_msg;

        return *this;
    }
};


int main()
{
    try
    {

        std::string api_key{ "key" };
        std::string api_secret{ "secret" };

        FuturesClientUSDT my_client{ api_key, api_secret };
        SomeFunctor ws_stream_read{};

        std::thread t1(&FuturesClientUSDT::stream_userStream<SomeFunctor>, std::ref(my_client), std::ref(ws_stream_read), 1);

        t1.join();
    }
    catch (ClientException& e)
    {
        std::cout << e.what();
    }

    return 0;
}