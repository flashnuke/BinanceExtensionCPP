#include "../include/Binance_Client.h"
#include <iostream>

int main()
{
    std::string apiKey{ "" };
    std::string apiSecret{ "" };

    SpotClient my_client{ apiKey, apiSecret };

    Params order_params{};
    order_params.set_param("symbol", std::string("BTCUSDT"));
    order_params.set_param("price", 9000); // better: send value 9000 in std::string format
    order_params.set_param("side", std::string("BUY"));
    order_params.set_param("timeInForce", std::string("GTC"));
    order_params.set_param("type", std::string("LIMIT"));
    order_params.set_param("quantity", 0.01); // better: send value 0.01 in std::string format

    try
    {
        std::cout << my_client.test_new_order(&order_params);     
    }
    catch (BadRequestREST)
    {
        std::cerr << "Bad REST request";
    }

    return 0;
}