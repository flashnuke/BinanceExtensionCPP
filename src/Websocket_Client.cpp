#include "../include/Binance_Client.h"

/**
	Default constructor
	@param exchange_client - passed internally
	@param host - the host of the path
	@param port - the port of the path
*/
template <typename T>
WebsocketClient<T>::WebsocketClient(T* exchange_client, const std::string host, const unsigned int port)
    : _host{ host }, _port{ std::to_string(port) }, _reconnect_on_error{ 0 }, exchange_client { exchange_client }, _max_reconnect_count{ 20 }
{}

/**
	Close a stream
	@param full_stream_name - Close a stream by its name
*/
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

/**
	Get open streams
	@return a vector containing std::string names of open streams
*/
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

/**
	Checks whether a stream is open
	@param full_stream_name - the full name of the stream
	@return a bool for whether is open or not
*/
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

/**
	Enable / Disable 'reconnect on error'
	@param reconnect - 1 to enable reconnect, 0 to disable
*/
template <typename T>
void WebsocketClient<T>::_set_reconnect(const bool& reconnect)
{
	this->_reconnect_on_error = reconnect;
}

/**
	Sets a different host / port 
	@param new_host - std::string representing the new host
	@param new_port - the new port
*/
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

/**
	Destructor
	setting 'exchange_client' to nullptr to avoid deleting an external exchange client
	iterates over open streams and closes them one by one
*/
template <typename T>
WebsocketClient<T>::~WebsocketClient()
{
	this->exchange_client = nullptr;
    std::unordered_map<std::string, bool>::iterator stream_itr; // while status != 0: set 0 and delete thread pointer!

    for (stream_itr = this->running_streams.begin(); stream_itr != this->running_streams.end(); stream_itr++)
    {
        this->close_stream(stream_itr->first); // set status to false to ensure closing streams
    }
}

template class WebsocketClient<SpotClient>;
template class WebsocketClient<FuturesClient<FuturesClientCoin>>;
template class WebsocketClient<FuturesClient<FuturesClientUSDT>>;
template class WebsocketClient<OpsClient>;
