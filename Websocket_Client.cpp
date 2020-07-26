
#include "CryptoExtensions.h"

//todo: send ping method. might need to use it every 24 hours...


WebsocketClient::WebsocketClient(std::string host, std::string port)
    : _host{ host }, _port{ port }, reconnect_on_error{ 0 }, refresh_listenkey_interval{ 3300 }
{}


void WebsocketClient::close_stream(const std::string full_stream_name) // todo: return sucess unsigned int
{
    this->running_streams[full_stream_name] = 0;
}

std::vector<std::string> WebsocketClient::open_streams()
{
	std::unordered_map<std::string, bool>::iterator itr;
	std::vector<std::string> results;

	for (itr = this->running_streams.begin(); itr != this->running_streams.end(); itr++)
	{
		if (itr->second) results.push_back(itr->first);
	}

	return results;
};

bool WebsocketClient::is_open(const std::string& full_stream_name)
{
	std::unordered_map<std::string, bool>::iterator itr;

	for (itr = this->running_streams.begin(); itr != this->running_streams.end(); itr++)
	{
		if (itr->first == full_stream_name) return itr->second;
	}

	return 0; // does not exit - is not open
};

template <class FT>
void WebsocketClient::_stream_manager(std::string stream_map_name, std::string& buf, FT& functor, std::pair<RestSession*,
	std::string> user_stream_pair)
{
	this->running_streams[stream_map_name] = 0; // init
	do
	{
		if (user_stream_pair.first) this->_connect_to_endpoint<FT>(stream_map_name, buf, functor, user_stream_pair);
		else this->_connect_to_endpoint<FT>(stream_map_name, buf, functor);
	} while (this->running_streams[stream_map_name] && this->reconnect_on_error);
}

template <class FT>
void WebsocketClient::_connect_to_endpoint(std::string stream_map_name, std::string& buf, FT& functor, std::pair<RestSession*, std::string> user_stream_pair)
{
	long long unsigned int last_keepalive{ 0 };

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
			if (user_stream_pair.first)
			{
				long long unsigned int current_timestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
				if (current_timestamp - last_keepalive > this->refresh_listenkey_interval) // delete - return param
				{
					(user_stream_pair.first)->_putreq(user_stream_pair.second);
					std::cout << "bazinga!"; // delete
					last_keepalive = current_timestamp;
				}
			}
			auto beast_buffer = boost::asio::dynamic_buffer(buf); // impossible to declare just once...
			ws.read(beast_buffer, ec);
			if (ec)
			{
				std::cerr << ec;
				if (!this->reconnect_on_error)this->running_streams[stream_map_name] = 0; // to exit loop if not retry
				break;
			}

			functor(buf);
			buf.clear();
		}
		catch (...)
		{
			if (!this->reconnect_on_error)this->running_streams[stream_map_name] = 0; // to exit loop if not retry
			break;
		}
	}
}

void WebsocketClient::_set_reconnect(const bool& reconnect)
{
	this->reconnect_on_error = reconnect;
}


WebsocketClient::~WebsocketClient()
{
    std::unordered_map<std::string, bool>::iterator stream_itr; // while status != 0: set 0 and delete thread pointer!

    for (stream_itr = this->running_streams.begin(); stream_itr != this->running_streams.end(); stream_itr++)
    {
        (stream_itr->second) = 0; // set status to false to ensure closing streams

    }
}