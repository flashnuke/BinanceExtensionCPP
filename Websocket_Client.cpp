
// todo: in 'connect' method, check firstly if we are connected alrdy. if so, disconnect

#include "CryptoExtensions.h"


WebsocketClient::WebsocketClient(std::string host, std::string port)
    : _host{ host }, _port{ port }
{}


void WebsocketClient::_connect_to_endpoint(std::string endpoint)
{
    if (this->_ws)
    {
        if(this->_ws->is_open()) std::cout << "there is a client already? " << this->_ws->is_open() << "\n";
    }
    
    net::io_context ioc;
    ssl::context ctx{ ssl::context::tlsv12_client };
    tcp::resolver resolver{ ioc };
    this->_ws = new websocket::stream<beast::ssl_stream<tcp::socket>>{ ioc, ctx };

    auto const ex_client = resolver.resolve(this->_host, this->_port);
    auto ep = net::connect(get_lowest_layer(*this->_ws), ex_client);
    this->_host += ':' + std::to_string(ep.port());
    this->_ws->next_layer().handshake(ssl::stream_base::client);

    (this->_ws)->handshake(this->_host, "/ws/btcusdt@aggTrade"); // "/ws/btcusdt@aggTrade"
    beast::flat_buffer buffer;

    while (1)
    {
        this->_ws->read(buffer);
        std::cout << beast::make_printable(buffer.data()) << std::endl;
    }

}

void WebsocketClient::start_stream(std::string endpoint)
// todo: insert pointer as param, change void
// todo: insert function as param. pass pointer into function (callback)
{
    this->_connect_to_endpoint(endpoint);
}


WebsocketClient::~WebsocketClient()
{
    delete this->_ws;
}