#include "../include/Binance_Client.h"
#include <iostream>

int main()
{
    try
    {
        std::string apiKey = "";
        std::string apiSecret = "";

        SpotClient my_client{ apiKey, apiSecret };
        SpotClient::Wallet my_client_wallet{ my_client }; // initializing a Wallet object using the Client

        std::cout << my_client_wallet.account_status();
    }
    catch (ClientException& e)
    {
        std::cout << e.what();
    }

    return 0;
}