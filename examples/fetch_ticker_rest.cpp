#include "../include/Binance_Client.h"
#include <iostream>


int main()
{
    FuturesClientUSDT public_client{};

    Params request_params;
    request_params.set_param("symbol", std::string("BTCUSDT"));
    Json::Value response = public_client.get_ticker(&request_params);
    
    std::cout << "the current price of btcusdt is " << response["response"]["price"];
    
    return 0;
}


