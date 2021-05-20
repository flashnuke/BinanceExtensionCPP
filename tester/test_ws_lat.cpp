#include "../include/Binance_Client.h"
#include <thread>
#include <iostream>
#include <fstream>
#include <vector>

struct SomeFunctor
{
    Json::CharReaderBuilder charbuilder;
    Json::CharReader* charreader;
    std::string parse_errors;
    std::string msg_buffer;
    Json::Value stream_msg;
    unsigned int counter = 0;
    std::vector<std::string> timestamp_results;

    SomeFunctor()
        : msg_buffer{ "" }, parse_errors{ }, charreader{ charbuilder.newCharReader() }
    {}


    SomeFunctor operator()(const std::string& response)
    {
        this->stream_msg.clear();
        this->counter++;

        this->charreader->parse(response.c_str(),
            response.c_str() + response.size(),
            &this->stream_msg,
            &parse_errors);

        this->timestamp_results.push_back(this->stream_msg["data"]["E"].asString() + ","
            + this->stream_msg["data"]["T"].asString() + "," + std::to_string(local_timestamp()) + "\n");

        if (this->counter > 100)
        {
            this->write_results();
        }

        return *this;
    }

    void write_results()
    {
        std::ofstream result_file;
        std::cout << "writing results";
        result_file.open("results.txt");
        for (std::string timestamps : this->timestamp_results)
        {
            result_file << timestamps;
        }
        result_file.close();
        exit(0);

    }
};


int main()
{
    try
    {
        FuturesClientUSDT my_client{ };
        SomeFunctor ws_stream_read{};
        std::string cust_stream = "btcusdt@depth@0ms";
        std::thread t1(&FuturesClientUSDT::custom_stream<SomeFunctor>, std::ref(my_client), cust_stream, std::ref(ws_stream_read.msg_buffer), std::ref(ws_stream_read), 0);

        t1.join();
    }
    catch (ClientException& e)
    {
        std::cout << e.what();
    }

    return 0;
}