#include <fstream>
#include "../CryptoExtensions.h"
#include "../Binance_Client.cpp"
#include <thread>

bool auth_mode = 1;
std::string __KEYS_PATH = "keys_conf.json";

struct SomeFunctor
{
    unsigned int current_price{ 0 };
    Json::CharReaderBuilder charbuilder;
    Json::CharReader* charreader = charbuilder.newCharReader();
    Json::Value response_json;

    SomeFunctor operator()(const std::string& response)
    {
        this->response_json.clear();
        std::string parse_errors{};

        bool parse_status = this->charreader->parse(response.c_str(),
            response.c_str() + response.size(),
            &this->response_json,
            &parse_errors);

        std::cout << this->response_json["p"];

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
    Json::Value keys;

    if (auth_mode) keys = load_keys(__KEYS_PATH);

    SpotClient my_client{ keys["key"].asString(), keys["secret"].asString() };

    int dynamic_num{ 0 };
    Params temp_params{};
    temp_params.set_param<std::string>("side", "buy");

    Params order_params{ Params{} };
    std::string str_buf{};
    std::string str_buf2{};

    SomeFunctor btcusdt_agg{};
    SomeFunctor ethusdt_agg{};


    std::thread t1(&SpotClient::aggTrade<SomeFunctor>, std::ref(my_client), "ethusdt", std::ref(str_buf), std::ref(btcusdt_agg));
    std::thread t2(&SpotClient::aggTrade<SomeFunctor>, std::ref(my_client), "btcusdt", std::ref(str_buf2), std::ref(ethusdt_agg));


    while (1)
    {
        try
        {

            //std::cout << "Current server time is " << my_client.exchange_time() << std::endl;
            //std::cout << "ping status " << my_client.ping_client() << std::endl;
            //std::cout << "renewing session..." << std::endl;
            //my_client.renew_session();

            //order_params.set_param<int>("price", dynamic_num);

            //std::cout << my_client.send_order(order_params);

            dynamic_num++;
            std::cout << "main loop";
            if (dynamic_num > 10)
            {
                std::cout << "\nis stream open?" << my_client.is_stream_open("btcusdt", "aggTrade");
            }
            //my_client.aggTrade<SomeFunctor>("btcusdt", str_buf, btcusdt_agg);
        }
        catch (...)
        {
            std::cout << "error in main loop";
        }
        Sleep(200);

    }

    return 0;
}