#include "../Binance_Client.h"

// Client inline methods

template<typename T>
inline void Client<T>::close_stream(const std::string& stream_name)
{
		this->_ws_client->close_stream(stream_name);
}

template<typename T>
inline bool Client<T>::is_stream_open(const std::string& stream_name)
{
		return this->_ws_client->is_open(stream_name);
}

template<typename T>
inline std::vector<std::string> Client<T>::get_open_streams()
{
		return this->_ws_client->open_streams();
}

template<typename T>
inline void Client<T>::ws_auto_reconnect(const bool reconnect)
{
	this->_ws_client->_set_reconnect(reconnect);
}

template <typename T>
inline void Client<T>::set_refresh_key_interval(const unsigned int val)
{
	this->refresh_listenkey_interval = val;
}

template <typename T>
inline void Client<T>::set_max_reconnect_count(const unsigned int val)
{
	this->_ws_client->_max_reconnect_count = val;
}

// FuturesClient inline methods

template <typename CT>
inline void FuturesClient<CT>::set_testnet_mode(const bool status)
{
	return static_cast<CT*>(this)->v_set_testnet_mode(status);
}

template <typename CT>
inline bool FuturesClient<CT>::get_testnet_mode()
{
	return this->_testnet_mode;
}

inline void ClientException::append_to_traceback(const std::string& loc)
{
	this->traceback.push_back(loc);
}

inline void ClientException::append_to_traceback(std::string&& loc)
{
	this->traceback.push_back(std::move(loc));
}

// Params inline methods

inline bool Params::empty() const
{
	return this->param_map.empty();
}

// Auth utils inline methods

inline char binary_to_hex_digit(unsigned a)
{
	return a + (a < 10 ? '0' : 'a' - 10);
}

