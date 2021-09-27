#include "../Binance_Client.h"

// Client inline methods

/**
	Closes a Websocket stream
	@param stream_name - the name of the stream
*/
template<typename T>
inline void Client<T>::close_stream(const std::string& stream_name)
{
		this->_ws_client->close_stream(stream_name);
}

/**
	Checks whether a Websocket stream is active
	@param stream_name - the name of the stream
	@return a boolean for whether is open or not
*/
template<typename T>
inline bool Client<T>::is_stream_open(const std::string& stream_name)
{
		return this->_ws_client->is_open(stream_name);
}

/**
	Returns currently active streams
	@return vector containing the stream names
*/
template<typename T>
inline std::vector<std::string> Client<T>::get_open_streams()
{
		return this->_ws_client->open_streams();
}

/**
	Enables / Disables 'reconnect on error' 
	@param reconnect - a boolean for enabling or disabling
*/
template<typename T>
inline void Client<T>::ws_auto_reconnect(const bool reconnect)
{
	this->_ws_client->_set_reconnect(reconnect);
}

/**
	Sets the refresh ListenKey interval duration
	@param val - the interval duration
*/
template <typename T>
inline void Client<T>::set_refresh_key_interval(const unsigned int val)
{
	this->refresh_listenkey_interval = val;
}

/**
	Sets the max number of attempts to reconnect on error
	@param val - the max count value
*/
template <typename T>
inline void Client<T>::set_max_reconnect_count(const unsigned int val)
{
	this->_ws_client->_max_reconnect_count = val;
}

// FuturesClient inline methods

/**
	Enables / disables testnet mode
	@param status - a boolean for enabling / disabling
*/
template <typename CT>
inline void FuturesClient<CT>::set_testnet_mode(const bool status)
{
	return static_cast<CT*>(this)->v_set_testnet_mode(status);
}

/**
	Get current testnet mode status
	@return 1 if enabled, 0 if disabled
*/
template <typename CT>
inline bool FuturesClient<CT>::get_testnet_mode()
{
	return this->_testnet_mode;
}

/**
	Appends name of function to current exception object
	@param loc - name of function (constant)
*/
inline void ClientException::append_to_traceback(const std::string& loc)
{
	this->traceback.push_back(loc);
}

/**
	Appends name of function to current exception object
	@param loc - name of function
*/
inline void ClientException::append_to_traceback(std::string&& loc)
{
	this->traceback.push_back(std::move(loc));
}

// Params inline methods

/**
	Checks whether the Params object is empty
	@return a boolean - 1 if empty, 0 if not
*/
inline bool Params::empty() const
{
	return this->param_map.empty();
}

// Auth utils inline methods

/**
	Converts binary to hex digits
	@return - the generated result
*/
inline char binary_to_hex_digit(unsigned a)
{
	return a + (a < 10 ? '0' : 'a' - 10);
}

