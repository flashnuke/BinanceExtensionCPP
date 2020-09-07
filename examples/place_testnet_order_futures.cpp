#include "../include/Binance_Client.h"
#include <iostream>

// when using FuturesClient, 'test_new_order()' method is unsuable. Set Client 'testnet_mode' to 'true' instead.

int main()
{
    std::string apiKey{ "" };
    std::string apiSecret{ "" };

    FuturesClientUSDT my_client{ apiKey, apiSecret };
    my_client.set_testnet_mode(1); // setting 'testnet_mode' to 'true'

    Params order_params{};
    order_params.set_param("symbol", std::string("btcusdt"));
    order_params.set_param("price", 9000); // better: send value 9000 in std::string format
    order_params.set_param("side", std::string("BUY"));
    order_params.set_param("timeInForce", std::string("GTC"));
    order_params.set_param("type", std::string("LIMIT"));
    order_params.set_param("quantity", 0.01); // better: send value 0.01 in std::string format

    try
    {
        std::cout << my_client.new_order(&order_params);
     }
    catch (BadRequestREST)
    {
        std::cerr << "Bad REST request";
    }
    return 0;

}