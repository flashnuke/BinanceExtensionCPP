#include "../Binance_Client.h"

// Client inline methods

template<typename T>
void Client<T>::close_stream(const std::string& stream_name)
{
		this->_ws_client->close_stream(stream_name);
}

template<typename T>
bool Client<T>::is_stream_open(const std::string& stream_name)
{
		return this->_ws_client->is_open(stream_name);
}

template<typename T>
std::vector<std::string> Client<T>::get_open_streams()
{
		return this->_ws_client->open_streams();
}

template<typename T>
void Client<T>::ws_auto_reconnect(const bool reconnect)
{
	this->_ws_client->_set_reconnect(reconnect);
}

template <typename T>
void Client<T>::set_refresh_key_interval(const unsigned int val)
{
	this->refresh_listenkey_interval = val;
}

template <typename T>
void Client<T>::set_max_reconnect_count(const unsigned int val)
{
	this->_ws_client->_max_reconnect_count = val;
}

// FuturesClient inline methods

template <typename CT>
void FuturesClient<CT>::set_testnet_mode(const bool& status)
{
	return static_cast<CT*>(this)->v_set_testnet_mode(status);
}

template <typename CT>
bool FuturesClient<CT>::get_testnet_mode()
{
	return this->_testnet_mode;
}

// Auth utils inline methods

char binary_to_hex_digit(unsigned a)
{
	return a + (a < 10 ? '0' : 'a' - 10);
}