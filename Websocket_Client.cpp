
// todo: in 'connect' method, check firstly if we are connected alrdy. if so, disconnect
// todo: keep thread without joining?
// todo: if streamname in map, abort? in order to avoid dups, simply send the entire path or with symbol
// todo: form a 'stream' object that would contain: thread addr, functor, pointer to stream string, status, error_code
// todo: stream object contains: disconnect(), reconnect() - delete current thread and call _connect_endpoint() method
// todod dest: while status != 0: set 0 and delete thread pointer!

#include "CryptoExtensions.h"


WebsocketClient::WebsocketClient(std::string host, std::string port)
    : _host{ host }, _port{ port }
{}


void WebsocketClient::close_stream(const std::string full_stream_name)
{
    this->running_streams[full_stream_name] = 0;
}

void WebsocketClient::get_streams()
{};

void WebsocketClient::is_open(std::string symbol, std::string stream_name)
{};

template <class FT>
void WebsocketClient::_connect_to_endpoint(std::string symbol, std::string stream_name, std::string& buf, FT& functor)
{

	std::string stream_map_name = symbol + "@" + stream_name;

	if (this->running_streams.find(stream_map_name) != this->running_streams.end()) // if stream in map
	{
		std::cout << "error: stream exists";
	}
	else
	{
		net::io_context ioc;
		ssl::context ctx{ ssl::context::tlsv12_client };
		tcp::resolver resolver{ ioc };
		websocket::stream<beast::ssl_stream<tcp::socket>> ws{ ioc, ctx };

		const boost::asio::ip::basic_resolver_results<boost::asio::ip::tcp> ex_client = resolver.resolve(this->_host, this->_port);
		auto ep = net::connect(get_lowest_layer(ws), ex_client);
		this->_host += ':' + std::to_string(ep.port());
		ws.next_layer().handshake(ssl::stream_base::client);
		std::string handshake_endp = "/ws/" + stream_map_name;
		ws.handshake(this->_host, handshake_endp);

		beast::error_code ec; // error code

		if (ws.is_open())
		{
			this->running_streams[stream_map_name] = 1;
		}
		else
		{
			throw("stream_init_bad");
		}

		while (this->running_streams[stream_map_name])
		{
			try
			{
				auto beast_buffer = boost::asio::dynamic_buffer(buf); // impossible to declare just once...
				ws.read(beast_buffer, ec);
				if (ec)
				{
					std::cerr << ec;
					this->running_streams[stream_map_name] = 0;
					throw("stream_response_bad"); // todo: add to exceptions. throw error code
				}

				functor(buf);
				buf.clear();
			}
			catch (...)
			{
				this->running_streams[stream_map_name] = 0;
				throw("stream_response_exception"); // todo: add to exceptions
			}

		}
	}

}

WebsocketClient::~WebsocketClient()
{
    std::map<std::string, bool>::iterator stream_itr; // while status != 0: set 0 and delete thread pointer!

    for (stream_itr = this->running_streams.begin(); stream_itr != this->running_streams.end(); stream_itr++)
    {
        (stream_itr->second) = 0; // set status to false to ensure closing streams

    }
}