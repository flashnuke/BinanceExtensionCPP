#include "Binance_Client.h"

template <typename T>
WebsocketClient<T>::WebsocketClient(T& exchange_client, const std::string host, const unsigned int port)
    : _host{ host }, _port{ std::to_string(port) }, _reconnect_on_error{ 0 }, exchange_client { exchange_client }, _max_reconnect_count{ 20 }
{}

template <typename T>
void WebsocketClient<T>::close_stream(const std::string& full_stream_name)
{
	try
	{
		this->running_streams[full_stream_name] = 0;
	}
	catch(...)
	{
		BadStreamCloseWS e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

template <typename T>
std::vector<std::string> WebsocketClient<T>::open_streams()
{
	std::unordered_map<std::string, bool>::iterator itr;
	std::vector<std::string> results;

	for (itr = this->running_streams.begin(); itr != this->running_streams.end(); itr++)
	{
		if (itr->second) results.push_back(itr->first);
	}

	return results;
};

template <typename T>
bool WebsocketClient<T>::is_open(const std::string& full_stream_name) const
{
	std::unordered_map<std::string, bool>::const_iterator itr;

	for (itr = this->running_streams.begin(); itr != this->running_streams.end(); itr++)
	{
		if (itr->first == full_stream_name) return itr->second;
	}

	return 0; 
};

template <typename T>
template <class FT>
void WebsocketClient<T>::_stream_manager(std::string stream_map_name, std::string& buf, FT& functor, const bool ping_listen_key)
{
	try
	{
		unsigned int reconnect_attempts = 0;
		this->running_streams[stream_map_name] = 0; // init
		do
		{
			try
			{
				this->_connect_to_endpoint<FT>(stream_map_name, buf, functor, ping_listen_key); // will not proceed unless connection is broken
			}
			catch (ClientException e)
			{
				if(this->running_streams[stream_map_name] && this->_reconnect_on_error) // if reconnect
				{
					reconnect_attempts++;
				}
				else // if not reconnect, throw exception
				{
					e.append_to_traceback(std::string(__FUNCTION__));
					throw(e);
				}
			}
		} while (this->running_streams[stream_map_name] && this->_reconnect_on_error && (reconnect_attempts < this->_max_reconnect_count)); // will repeat only of stream is up (no user shutdown) and reconnect is true, and reconnections not above max
	}
	
}

template <typename T>
template <class FT>
void WebsocketClient<T>::_connect_to_endpoint(const std::string stream_map_name, std::string& buf, FT& functor, const bool ping_listen_key)
{
	long long unsigned int last_keepalive{ 0 };

	net::io_context ioc;
	ssl::context ctx{ ssl::context::tlsv12_client };
	tcp::resolver resolver{ ioc };
	websocket::stream<beast::ssl_stream<tcp::socket>> ws{ ioc, ctx }; // todo: init here in map pair for subscribe/unsubscribe
	const boost::asio::ip::basic_resolver_results<boost::asio::ip::tcp> ex_client = resolver.resolve(this->_host, this->_port);
	auto ep = net::connect(get_lowest_layer(ws), ex_client);
	std::string full_host = this->_host + ':' + std::to_string(ep.port());
	ws.next_layer().handshake(ssl::stream_base::client);
	std::string handshake_endp = stream_map_name;
	ws.handshake(full_host, handshake_endp);

	beast::error_code ec; // error code

	if (ws.is_open()) // change back to is open
	{
		this->running_streams[stream_map_name] = 1;
	}
	else
	{
		BadStreamOpenWS e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}


	while (this->running_streams[stream_map_name])
	{
		try
		{
			if (ping_listen_key) // since const, no volatile?
			{
				long long unsigned int current_timestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
				if (current_timestamp - last_keepalive > this->exchange_client.refresh_listenkey_interval)
				{
					this->exchange_client.ping_listen_key();
					last_keepalive = current_timestamp;
				}
			}
			auto beast_buffer = boost::asio::dynamic_buffer(buf); // impossible to declare just once...
			ws.read(beast_buffer, ec);
			if (ec)
			{
				std::cerr << ec;
				if (!this->_reconnect_on_error)this->running_streams[stream_map_name] = 0; // to exit loop if not retry
				break;
			}

			functor(buf);
			buf.clear();
		}
		catch (...)
		{
			if (!this->_reconnect_on_error)
			{
				this->running_streams[stream_map_name] = 0; // to exit outer loop if not retry
			}

			BadStreamCallbackWS e{};
			e.append_to_traceback(std::string(__FUNCTION__));
			throw(e);
		}
	}
}

template <typename T>
void WebsocketClient<T>::_set_reconnect(const bool& reconnect)
{
	this->_reconnect_on_error = reconnect;
}

template <typename T>
void WebsocketClient<T>::set_host_port(const std::string new_host, const unsigned int new_port)
{
	this->_host = new_host;
	this->_port = std::to_string(new_port);
	if (!this->open_streams().empty())
	{
		BadSetupPathWS e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

template <typename T>
WebsocketClient<T>::~WebsocketClient()
{
    std::unordered_map<std::string, bool>::iterator stream_itr; // while status != 0: set 0 and delete thread pointer!
	
    for (stream_itr = this->running_streams.begin(); stream_itr != this->running_streams.end(); stream_itr++)
    {
        (stream_itr->second) = 0; // set status to false to ensure closing streams

    }
}