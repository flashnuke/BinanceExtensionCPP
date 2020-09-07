#include "../include/Binance_Client.h"
#include <iostream>

int main()
{
    std::string apiKey{ "" };
    std::string apiSecret{ "" };

    SpotClient my_client{ apiKey, apiSecret }; // sending to 'dapi' from a SpotClient instance

    Params req_params{};
    req_params.set_param("symbol", std::string("btcusd_perp"));

    try
    {
        std::string api_base{ "https://dapi.binance.com" };
        std::string api_endpoint{ "/dapi/v1/depth" };

        std::cout << my_client.custom_get_req(api_base, api_endpoint, &req_params, 0); // base, endpoint, params, signature (0 for unsigned)
    }
    catch (BadRequestREST)
    {
        std::cerr << "Bad REST request";
    }

    return 0;
}
