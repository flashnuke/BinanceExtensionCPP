#include "../include/Binance_Client.h"
#include <thread>
#include <iostream>

struct SomeFunctor
{
    Json::CharReaderBuilder charbuilder;
    Json::CharReader* charreader;
    std::string parse_errors;
    std::string msg_buffer;
    Json::Value stream_msg;

    SomeFunctor()
        : msg_buffer{ "" }, parse_errors{ }, charreader{ charbuilder.newCharReader() }
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
        OpsClient my_client{ };
        SomeFunctor ws_stream_read{};

        std::string symbol = "BTC-210430-56000-C";
        std::thread t1(&OpsClient::stream_Trade<SomeFunctor>, std::ref(my_client), symbol, std::ref(ws_stream_read.msg_buffer), std::ref(ws_stream_read));

        t1.join();
    }
    catch (ClientException& e)
    {
        std::cout << e.what();
    }

    return 0;
}