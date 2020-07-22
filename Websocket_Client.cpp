
#include "CryptoExtensions.h"

//todo: send ping method. might need to use it every 24 hours...


WebsocketClient::WebsocketClient(std::string host, std::string port)
    : _host{ host }, _port{ port }
{}


void WebsocketClient::close_stream(const std::string full_stream_name) // todo: return sucess unsigned int
{
    this->running_streams[full_stream_name] = 0;
}

std::vector<std::string> WebsocketClient::open_streams()
{
	std::map<std::string, bool>::iterator itr;
	std::vector<std::string> results;

	for (itr = this->running_streams.begin(); itr != this->running_streams.end(); itr++)
	{
		if (itr->second) results.push_back(itr->first);
	}

	return results;
};

bool WebsocketClient::is_open(const std::string& full_stream_name)
{
	std::map<std::string, bool>::iterator itr;

	for (itr = this->running_streams.begin(); itr != this->running_streams.end(); itr++)
	{
		if (itr->first == full_stream_name) return itr->second;
	}

	return 0; // does not exit - is not open
};

template <class FT>
void WebsocketClient::_connect_to_endpoint(std::string stream_map_name, std::string& buf, FT& functor)
{
	this->running_streams[stream_map_name] = 0; // init
	net::io_context ioc;
	ssl::context ctx{ ssl::context::tlsv12_client };
	tcp::resolver resolver{ ioc };
	websocket::stream<beast::ssl_stream<tcp::socket>> ws{ ioc, ctx };

	const boost::asio::ip::basic_resolver_results<boost::asio::ip::tcp> ex_client = resolver.resolve(this->_host, this->_port);
	auto ep = net::connect(get_lowest_layer(ws), ex_client);
	std::string full_host = this->_host + ':' + std::to_string(ep.port());
	ws.next_layer().handshake(ssl::stream_base::client);
	std::string handshake_endp = "/ws/" + stream_map_name;
	ws.handshake(full_host, handshake_endp);

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

WebsocketClient::~WebsocketClient()
{
    std::map<std::string, bool>::iterator stream_itr; // while status != 0: set 0 and delete thread pointer!

    for (stream_itr = this->running_streams.begin(); stream_itr != this->running_streams.end(); stream_itr++)
    {
        (stream_itr->second) = 0; // set status to false to ensure closing streams

    }
}