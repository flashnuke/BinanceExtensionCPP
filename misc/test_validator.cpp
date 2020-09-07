#include <fstream>
#include "../include/Binance_Client.h"

#include <thread>
#include <chrono>
#include <iostream>


bool auth_mode = 1;
std::string __KEYS_PATH = "keys_conf.json";

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

Json::Value load_keys(std::string conf_filename) // return return type
{
    Json::Value keys_json;
    std::ifstream file_handle(__KEYS_PATH);
    if (!file_handle.is_open())
    {
        std::cout << "failed to load keys json" << std::endl;
        exit(1);
    };

    file_handle >> keys_json;

    return keys_json;
}

int main()
{
    try
    {
        // ========================================================== Client Tests

        std::cout << "\n\n\nStarting Client tests...\n\n";

        Json::Value keys;

        if (auth_mode) keys = load_keys(__KEYS_PATH);

        SpotClient my_client{ keys["key"].asString(), keys["secret"].asString() }; // private
        SpotClient::MarginAccount margin_acc{ my_client };

        FuturesClientUSDT my_f_client{}; // public

        try
        {
            std::cout << "\nattemping private struct on public client\n";
            FuturesClientUSDT::FuturesWallet my_f_wallet{ my_f_client };
        }
        catch (ClientException& e)
        {
            std::cout << "\nresult of attempt: " << e.what();
        }


        // ========================================================== WS Tests

        std::cout << "\n\n\nStarting WS tests...\n\n";

        std::string str_buf{};
        SomeFunctor ws_stream_read{};

        std::string cust_stream = "btcusdt@aggTrade/ethusdt@aggTrade";
        my_client.set_refresh_key_interval(1);
        std::cout << "\nstarting custom stream + ping\n";
        std::thread t1(&SpotClient::custom_stream<SomeFunctor>, std::ref(my_client), cust_stream, std::ref(str_buf), std::ref(ws_stream_read), 1);
        Sleep(2000);
        std::cout << "\nis stream open? " << my_client.is_stream_open(cust_stream);
        std::cout << "\ngoing to sleep for 3 seconds";
        Sleep(3000);
        std::cout << "\nterminating custom stream";
        my_client.close_stream(cust_stream);
        std::cout << "\ncustom stream was terminated";

        std::cout << "\nstarting aggTrade stream\n";
        std::thread t2(&SpotClient::stream_aggTrade<SomeFunctor>, std::ref(my_client), "btcusdt", std::ref(str_buf), std::ref(ws_stream_read));
        std::cout << "\ngoing to sleep for 2 seconds";
        Sleep(2000);
        std::cout << "\nterminating aggTrade stream";
        my_client.close_stream("btcusdt@aggTrade");
        std::cout << "\naggTrade stream was terminated";

        t1.join();
        t2.join();
        Sleep(2000);

        // ========================================================== REST Tests
       
        std::cout << "\n\n\nStarting REST tests...\n\n";

        Params temp_params{};
        temp_params.set_param<std::string>("symbol", "BTCUSDT");
        Params order_params{ Params{} };
        order_params = Params{};

        std::cout << "\nfetching all orders spotclient:\n";
        std::cout << my_client.all_orders(&temp_params) << "\n";

        std::cout << "\nfetching all orders futuresclient:\n";
        std::cout << my_f_client.all_orders(&temp_params) << "\n";

        std::cout << "\nMarginAccount default not isolated listen_key: " << margin_acc.margin_get_listen_key() << "\n";
        std::cout << "\nMarginAccount isolated listen_key: " << margin_acc.margin_isolated_get_listen_key("btcusdt") << "\n";


        std::cout << "\testing custom request with params: \n" << my_client.custom_get_req("https://api.binance.com", "/api/v3/time", &order_params, 0);


        // ========================================================== Exceptions

        std::cout << "\n\n\nStarting Exceptions tests...\n\n";

        try
        {
            BadRequestREST e{};
            throw(e);
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << "\n";
            std::cout << "\nBadRequestREST was caught\n";
        }

        try
        {
            BadSetupHeadersREST e{};
            throw(e);
        }
        catch (ClientException& e)
        {
            std::cout << e.what() << "\n";
            std::cout << "\nBadSetupHeadersREST was caught\n";
        }

        try
        {
            CustomException e("custom exception");
            throw(e);
        }
        catch (ClientException& e)
        {
            std::cout << e.what() << "\n";
            std::cout << "\nCustomException was caught\n";
        }

    }
    catch (ClientException& e)
    {
        std::cout << "\nan exception has occurred: " << e.what();
    }
    std::cout << "finished successfully";
    return 0;
}