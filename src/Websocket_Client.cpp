#include "../include/Binance_Client.h"

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

template class WebsocketClient<SpotClient>;
template class WebsocketClient<FuturesClient<FuturesClientCoin>>;
template class WebsocketClient<FuturesClient<FuturesClientUSDT>>;