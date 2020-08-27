#include <fstream>
#include "../include/Binance_Client.cpp"

#include <thread>
#include <chrono>


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

        std::cout << this->response_json;

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

    //SpotClient my_client{ keys["key"].asString(), keys["secret"].asString() };
    //SpotClient::Wallet my_wallet{ my_client };

    FuturesClientUSDT f_usdt_client{ keys["key"].asString(), keys["secret"].asString() };
    f_usdt_client.set_refresh_key_interval(15);


    int dynamic_num{ 0 };
    Params temp_params{};
    temp_params.set_param<std::string>("symbol", "btcusdt");

    Params order_params{ Params{} };
    order_params = Params{};
    std::string str_buf{};
    std::string str_buf2{};
    std::string str_buf3{};
    std::string str_buf4{};


    SomeFunctor btcusdt_agg{};
    SomeFunctor ethusdt_agg{};
    SomeFunctor user_stream{};
    SomeFunctor ethbtc_agg{};

    std::string cust_stream = "btcusdt@aggTrade/ethusdt@aggTrade";
    try
    {
        f_usdt_client.stream_aggTrade<SomeFunctor>("btcusdt", str_buf, ethusdt_agg);
    }
    catch (ClientException e)
    {
        std::cout << e.what();
    }

    //std::thread t1(&FuturesClientUSDT::v_stream_markprice_all<SomeFunctor>, std::ref(f_usdt_client), "btcusdt", std::ref(str_buf), std::ref(ethusdt_agg));
    //std::thread t2(&FuturesClientUSDT::stream_userStream<SomeFunctor>, std::ref(f_usdt_client), std::ref(str_buf3), std::ref(user_stream), 1);
    //std::thread t3(&FuturesClientUSDT::stream_aggTrade<SomeFunctor>, std::ref(f_usdt_client), "btcusdt", std::ref(str_buf2), std::ref(btcusdt_agg));
    //std::thread t4(&SpotClient::aggTrade<SomeFunctor>, std::ref(my_client), "ethbtc", std::ref(str_buf3), std::ref(ethbtc_agg));
    //std::thread t5(&SpotClient::custom_stream<SomeFunctor>, std::ref(my_client), cust_stream, std::ref(str_buf3), std::ref(ethbtc_agg));


    //std::thread t3(&SpotClient::ping_client, std::ref(my_client));
    //std::thread t4(&SpotClient::ping_client, std::ref(my_client));

    //FuturesClient my_client_f{ keys["key"].asString(), keys["secret"].asString() };
    //my_client.rest_set_verbose(1);
    while (1)
    {
        //f_usdt_client.set_testnet_mode(1);
        //std::cout << my_wallet.get_all_coins(&order_params);
        //std::cout << my_wallet.get_all_coins();
        //std::cout << f_usdt_client.ping_client();

        //order_params.set_param<std::string>("holla", "man");
        //std::cout << f_usdt_client.account_info();
        //std::cout << "\nv__ping_client() ok\n";

        Sleep(99999);

    }

    //return 0;
}