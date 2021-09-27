#include "../Binance_Client.h"

// Client stream templates

/**
	Start userstream (Margin)
	@param buffer - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@param ping_listen_key - if true, will ping listen key periodically
	@param isolaten_margin_type - if true, userstream will be for isolated margin
	@return an unsigned int representing success
*/
template <typename T>
template <typename FT>
unsigned int Client<T>::MarginAccount::margin_stream_userstream(std::string& buffer, FT& functor, const bool ping_listen_key, const bool& isolated_margin_type)
{
	try
	{
		std::string stream_name = this->margin_get_listen_key(isolated_margin_type);
		std::string stream_query = "/ws/" + stream_name;
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->template _stream_manager<FT>(stream_name, stream_query, buffer, functor, ping_listen_key);
		return this->_ws_client->running_streams[stream_query];
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}

}

/**
	Start userstream (Spot)
	@param buffer - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@param ping_listen_key - if true, will ping listen key periodically
	@return an unsigned int representing success
*/
template <typename FT>
unsigned int SpotClient::v_stream_userstream(std::string& buffer, FT& functor, const bool ping_listen_key)
{
	std::string stream_name = this->get_listen_key();
	std::string stream_query = "/ws/" + stream_name;
	if (this->_ws_client->is_open(stream_query))
	{
		this->_ws_client->close_stream(stream_query);
	}
	this->_ws_client->_stream_manager<FT>(stream_name, stream_query, buffer, functor, ping_listen_key);
	return this->_ws_client->running_streams[stream_query];

}

/**
	Start trades stream (Spot)
	@param symbol - the symbol of the trades
	@param buffer - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@return an unsigned int representing success
*/
template <typename FT>
unsigned int SpotClient::v_stream_Trade(const std::string& symbol, std::string& buffer, FT& functor)
{
	std::string stream_name = symbol + '@' + "trade";
	std::string stream_query = "/ws/" + stream_name;
	if (this->_ws_client->is_open(stream_query))
	{
		this->_ws_client->close_stream(stream_query);
	}
	this->_ws_client->_stream_manager<FT>(stream_name, stream_query, buffer, functor);
	return this->_ws_client->running_streams[stream_query];
}

/**
	Start mark price stream (symbol)
	@param symbol - the symbol of the trades
	@param buffer - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@param interval - interval of the responses, part of the query
	@return an unsigned int representing success
*/
template<typename CT>
template <typename FT>
unsigned int FuturesClient<CT>::stream_markprice(const std::string& symbol, std::string& buffer, FT& functor, unsigned int interval)
{
	try
	{
		std::string stream_name = symbol + '@' + "markPrice" + std::to_string(interval) + "ms";
		std::string stream_query = "/ws/" + stream_name;
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->template _stream_manager<FT>(stream_name, stream_query, buffer, functor);
		return this->_ws_client->running_streams[stream_query];
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}


/**
	Start trades stream (Futures)
	@param symbol - the symbol of the trades
	@param buffer - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@return an unsigned int representing success
*/
template <typename CT>
template <typename FT>
unsigned int FuturesClient<CT>::v_stream_Trade(const std::string& symbol, std::string& buffer, FT& functor)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}


/**
	Start mark price stream (all)
	@param pair - the pair
	@param buffer - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@return an unsigned int representing success
*/
template<typename CT>
template <typename FT>
unsigned int FuturesClient<CT>::stream_markprice_all(const std::string& pair, std::string& buffer, FT& functor)
{
	try
	{
		return static_cast<CT*>(this)->v_stream_markprice_all(pair, buffer, functor);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
} // only USDT

/**
	Start index price stream
	@param pair - the pair
	@param buffer - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@param interval - interval of the responses, part of the query
	@return an unsigned int representing success
*/
template<typename CT>
template <typename FT>
unsigned int FuturesClient<CT>::stream_indexprice(const std::string& pair, std::string& buffer, FT& functor, unsigned int interval)
{
	try
	{
		return static_cast<CT*>(this)->v_stream_indexprice(pair, buffer, functor, interval);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
} // only Coin



/**
	Start mark price stream (by pair)
	@param pair - the pair
	@param buffer - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@param interval - interval of the responses, part of the query
	@return an unsigned int representing success
*/
template<typename CT>
template <typename FT>
unsigned int FuturesClient<CT>::stream_markprice_by_pair(const std::string& pair, std::string& buffer, FT& functor, unsigned int interval)
{
	try
	{
		return static_cast<CT*>(this)->v_stream_markprice_by_pair(pair, buffer, functor, interval);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
} // only coin


/**
	Start kline contract stream
	@param pair_and_type - the pair & type
	@param buffer - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@param interval - interval of the responses, part of the query
	@return an unsigned int representing success
*/
template<typename CT>
template <typename FT>
unsigned int FuturesClient<CT>::stream_kline_contract(const std::string& pair_and_type, std::string& buffer, FT& functor, std::string interval)
{
	try
	{
		return static_cast<CT*>(this)->v_stream_kline_contract(pair_and_type, buffer, functor, interval);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
} // only coin

/**
	Start kline index stream
	@param pair - the pair
	@param buffer - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@param interval - interval of the responses, part of the query
	@return an unsigned int representing success
*/
template<typename CT>
template <typename FT>
unsigned int FuturesClient<CT>::stream_kline_index(const std::string& pair, std::string& buffer, FT& functor, std::string interval)
{
	try
	{
		return static_cast<CT*>(this)->v_stream_kline_index(pair, buffer, functor, interval);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
} // only coin

/**
	Start kline mark price stream
	@param symbol - the symbol
	@param buffer - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@param interval - interval of the responses, part of the query
	@return an unsigned int representing success
*/
template<typename CT>
template <typename FT>
unsigned int FuturesClient<CT>::stream_kline_markprice(const std::string& symbol, std::string& buffer, FT& functor, std::string interval)
{
	try
	{
		return static_cast<CT*>(this)->v_stream_kline_markprice(symbol, buffer, functor, interval);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
} // only coin

/**
	Start liquidation orders stream
	@param symbol - the symbol
	@param buffer - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@return an unsigned int representing success
*/
template<typename CT>
template <typename FT>
unsigned int FuturesClient<CT>::stream_liquidation_orders(const std::string& symbol, std::string& buffer, FT& functor)
{
	try
	{
		std::string stream_name = symbol + "@" + "forceOrder";
		std::string stream_query = "/ws/" + stream_name;
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->template _stream_manager<FT>(stream_name, stream_query, buffer, functor);
		return this->_ws_client->running_streams[stream_query];
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Start liquidation orders stream (all)
	@param buffer - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@return an unsigned int representing success
*/
template<typename CT>
template <typename FT>
unsigned int FuturesClient<CT>::stream_liquidation_orders_all(std::string& buffer, FT& functor)
{
	try
	{
		std::string stream_name = "!forceOrder@arr";
		std::string stream_query = "/ws/" + stream_name;
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->template _stream_manager<FT>(stream_name, stream_query, buffer, functor);
		return this->_ws_client->running_streams[stream_query];
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	BLVT Info Streams
	@param buffer - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@param token_name - token name
	@return an unsigned int representing success
*/
template<typename CT>
template <typename FT>
unsigned int FuturesClient<CT>::stream_blvt_info(std::string& buffer, FT& functor, std::string token_name)
{
	try
	{
		return static_cast<CT*>(this)->v_stream_blvt_info(buffer, functor, token_name);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	BLVT NAV Kline/Candlestick Streams
	@param buffer - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@param token_name - token name
	@param interval - interval of klines
	@return an unsigned int representing success
*/
template<typename CT>
template <typename FT>
unsigned int FuturesClient<CT>::stream_blvt_klines(std::string& buffer, FT& functor, std::string token_name, std::string interval)
{
	try
	{
		return static_cast<CT*>(this)->v_stream_blvt_klines(buffer, functor, token_name, interval);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	BLVT NAV Kline/Candlestick Streams
	@param buffer - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@param token_name - token name
	@return an unsigned int representing success
*/
template<typename CT>
template <typename FT>
unsigned int FuturesClient<CT>::stream_composite_index_symbol(std::string& buffer, FT& functor, std::string token_name)
{
	try
	{
		return static_cast<CT*>(this)->v_stream_composite_index_symbol(buffer, functor, token_name);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	CRTP of stream_userstream
*/
template<typename CT>
template <typename FT>
unsigned int FuturesClient<CT>::v_stream_userstream(std::string& buffer, FT& functor, const bool ping_listen_key) { return static_cast<CT*>(this)->v__stream_userstream(buffer, functor, ping_listen_key); }

/**
	CRTP of stream_markprice_all
*/
template <typename FT>
unsigned int FuturesClientUSDT::v_stream_markprice_all(const std::string& symbol, std::string& buffer, FT& functor)
{
	std::string stream_name = symbol + '@' + "miniTicker";
	std::string stream_query = "/ws/" + stream_name;
	if (this->_ws_client->is_open(stream_query))
	{
		this->_ws_client->close_stream(stream_query);
	}
	this->_ws_client->_stream_manager<FT>(stream_name, stream_query, buffer, functor);
	return this->_ws_client->running_streams[stream_query];
}

/**
	CRTP of stream_indexprice
*/
template <typename FT>
unsigned int FuturesClientUSDT::v_stream_indexprice(const std::string& pair, std::string& buffer, FT& functor, unsigned int interval)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}

/**
	CRTP of stream_markprice_by_pair
*/
template <typename FT>
unsigned int FuturesClientUSDT::v_stream_markprice_by_pair(const std::string& pair, std::string& buffer, FT& functor, unsigned int interval)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}

/**
	CRTP of stream_kline_contract
*/
template <typename FT>
unsigned int FuturesClientUSDT::v_stream_kline_contract(const std::string& pair_and_type, std::string& buffer, FT& functor, std::string interval)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}

/**
	CRTP of stream_kline_index
*/
template <typename FT>
unsigned int FuturesClientUSDT::v_stream_kline_index(const std::string& pair, std::string& buffer, FT& functor, std::string interval)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}

/**
	CRTP of stream_kline_markprice
*/
template <typename FT>
unsigned int FuturesClientUSDT::v_stream_kline_markprice(const std::string& symbol, std::string& buffer, FT& functor, std::string interval)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}

/**
	CRTP of v_stream_userstream
*/
template <typename FT>
unsigned int FuturesClientUSDT::v__stream_userstream(std::string& buffer, FT& functor, const bool ping_listen_key)
{
	std::string stream_name = this->get_listen_key();
	std::string stream_query = "/ws/" + stream_name;
	if (this->_ws_client->is_open(stream_query))
	{
		this->_ws_client->close_stream(stream_query);
	}
	this->_ws_client->_stream_manager<FT>(stream_name, stream_query, buffer, functor);
	return this->_ws_client->running_streams[stream_query];
}

/**
	CRTP of stream_indexprice
*/
template <typename FT>
unsigned int FuturesClientCoin::v_stream_indexprice(const std::string& pair, std::string& buffer, FT& functor, unsigned int interval)
{
	std::string stream_name = pair + "@" + "indexPrice" + "@" + std::to_string(interval) + "ms";
	std::string stream_query = "/ws/" + stream_name;
	if (this->_ws_client->is_open(stream_query))
	{
		this->_ws_client->close_stream(stream_query);
	}
	this->_ws_client->_stream_manager<FT>(stream_name, stream_query, buffer, functor);
	return this->_ws_client->running_streams[stream_query];
}

/**
	CRTP of stream_markprice_by_pair
*/
template <typename FT>
unsigned int FuturesClientCoin::v_stream_markprice_by_pair(const std::string& pair, std::string& buffer, FT& functor, unsigned int interval)
{
	std::string stream_name = pair + "@" + "markPrice" + "@" + std::to_string(interval) + "ms";
	std::string stream_query = "/ws/" + stream_name;
	if (this->_ws_client->is_open(stream_query))
	{
		this->_ws_client->close_stream(stream_query);
	}
	this->_ws_client->_stream_manager<FT>(stream_name, stream_query, buffer, functor);
	return this->_ws_client->running_streams[stream_query];
}

/**
	CRTP of stream_blvt_info
*/
template <typename FT>
unsigned int FuturesClientCoin::v_stream_blvt_info(std::string& buffer, FT& functor, std::string token_name)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}

/**
	CRTP of stream_blvt_klines
*/
template <typename FT>
unsigned int FuturesClientCoin::v_stream_blvt_klines(std::string& buffer, FT& functor, std::string token_name, std::string interval)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}

/**
	CRTP of stream_composite_index_symbol
*/
template <typename FT>
unsigned int FuturesClientCoin::v_stream_composite_index_symbol(std::string& buffer, FT& functor, std::string token_name)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}

/**
	CRTP of stream_kline_contract
*/
template <typename FT>
unsigned int FuturesClientCoin::v_stream_kline_contract(const std::string& pair_and_type, std::string& buffer, FT& functor, std::string interval)
{
	std::string stream_name = pair_and_type + "@" + "continuousKline_" + (interval);
	std::string stream_query = "/ws/" + stream_name;
	if (this->_ws_client->is_open(stream_query))
	{
		this->_ws_client->close_stream(stream_query);
	}
	this->_ws_client->_stream_manager<FT>(stream_name, stream_query, buffer, functor);
	return this->_ws_client->running_streams[stream_query];
}

/**
	CRTP of stream_kline_index
*/
template <typename FT>
unsigned int FuturesClientCoin::v_stream_kline_index(const std::string& pair, std::string& buffer, FT& functor, std::string interval)
{
	std::string stream_name = pair + "@" + "indexPriceKline_" + (interval);
	std::string stream_query = "/ws/" + stream_name;
	if (this->_ws_client->is_open(stream_query))
	{
		this->_ws_client->close_stream(stream_query);
	}
	this->_ws_client->_stream_manager<FT>(stream_name, stream_query, buffer, functor);
	return this->_ws_client->running_streams[stream_query];
}

/**
	CRTP of stream_kline_markprice
*/
template <typename FT>
unsigned int FuturesClientCoin::v_stream_kline_markprice(const std::string& symbol, std::string& buffer, FT& functor, std::string interval)
{
	std::string stream_name = symbol + "@" + "markPriceKline_" + (interval);
	std::string stream_query = "/ws/" + stream_name;
	if (this->_ws_client->is_open(stream_query))
	{
		this->_ws_client->close_stream(stream_query);
	}
	this->_ws_client->_stream_manager<FT>(stream_name, stream_query, buffer, functor);
	return this->_ws_client->running_streams[stream_query];
}


/**
	CRTP of stream_blvt_info
*/
template <typename FT>
unsigned int FuturesClientUSDT::v_stream_blvt_info(std::string& buffer, FT& functor, std::string token_name)
{
	try
	{
		std::string stream_name = token_name + "@nav_Kline_";
		std::string stream_query = "/ws/" + stream_name;
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->_stream_manager<FT>(stream_name, stream_query, buffer, functor);
		return this->_ws_client->running_streams[stream_query];
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	CRTP of stream_blvt_klines
*/
template <typename FT>
unsigned int FuturesClientUSDT::v_stream_blvt_klines(std::string& buffer, FT& functor, std::string token_name, std::string interval)
{
	try
	{
		std::string stream_name = token_name + "@nav_Kline_" + interval;
		std::string stream_query = "/ws/" + stream_name;
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->_stream_manager<FT>(stream_name, stream_query, buffer, functor);
		return this->_ws_client->running_streams[stream_query];
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	CRTP of stream_composite_index_symbol
*/
template <typename FT>
unsigned int FuturesClientUSDT::v_stream_composite_index_symbol(std::string& buffer, FT& functor, std::string token_name)
{
	try
	{
		std::string stream_name = token_name + "@compositeIndex";
		std::string stream_query = "/ws/" + stream_name;
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->_stream_manager<FT>(stream_name, stream_query, buffer, functor);
		return this->_ws_client->running_streams[stream_query];
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	CRTP of v_stream_userstream
*/
template <typename FT>
unsigned int FuturesClientCoin::v__stream_userstream(std::string& buffer, FT& functor, const bool ping_listen_key)
{
	std::string stream_name = this->get_listen_key();
	std::string stream_query = "/ws/" + stream_name;
	if (this->_ws_client->is_open(stream_query))
	{
		this->_ws_client->close_stream(stream_query);
	}
	this->_ws_client->_stream_manager<FT>(stream_name, stream_query, buffer, functor, ping_listen_key);
	return this->_ws_client->running_streams[stream_query];
}

/**
	Start aggregated trades stream
	@param symbol - the symbol
	@param buffer - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@return an unsigned int representing success
*/
template<typename T>
template <typename FT>
unsigned int  Client<T>::stream_aggTrade(const std::string& symbol, std::string& buffer, FT& functor)
{
	try
	{
		std::string stream_name = symbol + '@' + "aggTrade";
		std::string stream_query = "/ws/" + stream_name;
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->template _stream_manager<FT>(stream_name, stream_query, buffer, functor);
		return this->_ws_client->running_streams[stream_query];
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Start kline (candlesticks) stream
	@param symbol - the symbol
	@param buffer - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@param interval - interval of the responses, part of the query
	@return an unsigned int representing success
*/
template<typename T>
template <typename FT>
unsigned int Client<T>::stream_kline(const std::string& symbol, std::string& buffer, FT& functor, std::string interval)
{
	try
	{
		std::string stream_name = symbol + '@' + "kline_" + interval;
		std::string stream_query = "/ws/" + stream_name;
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->template _stream_manager<FT>(stream_name, stream_query, buffer, functor);
		return this->_ws_client->running_streams[stream_query];
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Start mini ticker stream (individual)
	@param symbol - the symbol
	@param buffer - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@return an unsigned int representing success
*/
template<typename T>
template <typename FT>
unsigned int Client<T>::stream_ticker_ind_mini(const std::string& symbol, std::string& buffer, FT& functor)
{
	try
	{
		std::string stream_name = symbol + '@' + "miniTicker";
		std::string stream_query = "/ws/" + stream_name;
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->template _stream_manager<FT>(stream_name, stream_query, buffer, functor);
		return this->_ws_client->running_streams[stream_query];
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Start mini ticker stream (all)
	@param buffer - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@return an unsigned int representing success
*/
template<typename T>
template <typename FT>
unsigned int Client<T>::stream_ticker_all_mini(std::string& buffer, FT& functor)
{
	try
	{
		std::string stream_name = "!miniTicker@arr";
		std::string stream_query = "/ws/" + stream_name;
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->template _stream_manager<FT>(stream_name, stream_query, buffer, functor);
		return this->_ws_client->running_streams[stream_query];
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Start ticker stream (individual)
	@param symbol - the symbol
	@param buffer - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@return an unsigned int representing success
*/
template<typename T>
template <typename FT>
unsigned int Client<T>::stream_ticker_ind(const std::string& symbol, std::string& buffer, FT& functor)
{
	try
	{
		std::string stream_name = symbol + "@" + "ticker";
		std::string stream_query = "/ws/" + stream_name;
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->template _stream_manager<FT>(stream_name, stream_query, buffer, functor);
		return this->_ws_client->running_streams[stream_query];
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Start ticker stream (all)
	@param buffer - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@return an unsigned int representing success
*/
template<typename T>
template <typename FT>
unsigned int Client<T>::stream_ticker_all(std::string& buffer, FT& functor)
{
	try
	{
		std::string stream_name = "!ticker@arr";
		std::string stream_query = "/ws/" + stream_name;
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->template _stream_manager<FT>(stream_name, stream_query, buffer, functor);
		return this->_ws_client->running_streams[stream_query];
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Start book ticker stream (individual)
	@param symbol - the symbol
	@param buffer - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@return an unsigned int representing success
*/
template<typename T>
template <typename FT>
unsigned int Client<T>::stream_ticker_ind_book(const std::string& symbol, std::string& buffer, FT& functor)
{
	try
	{
		std::string stream_name = symbol + "@" + "bookTicker";
		std::string stream_query = "/ws/" + stream_name;
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->template _stream_manager<FT>(stream_name, stream_query, buffer, functor);
		return this->_ws_client->running_streams[stream_query];
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Start book ticker stream (all)
	@param buffer - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@return an unsigned int representing success
*/
template<typename T>
template <typename FT>
unsigned int Client<T>::stream_ticker_all_book(std::string& buffer, FT& functor)
{
	try
	{
		std::string stream_name = "!bookTicker";
		std::string stream_query = "/ws/" + stream_name;
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->template _stream_manager<FT>(stream_name, stream_query, buffer, functor);
		return this->_ws_client->running_streams[stream_query];
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Start partial depth stream
	@param symbol - the symbol
	@param buffer - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@param levels - how many depth level, part of the query
	@param interval - interval of the responses, part of the query
	@return an unsigned int representing success
*/
template<typename T>
template <typename FT>
unsigned int Client<T>::stream_depth_partial(const std::string& symbol, std::string& buffer, FT& functor, unsigned int levels, unsigned int interval)
{
	try
	{
		std::string stream_name = symbol + "@" + "depth" + std::to_string(levels);
		if (interval) stream_name += "@" + std::to_string(interval) + "ms";
		std::string stream_query = "/ws/" + stream_name;
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->template _stream_manager<FT>(stream_name, stream_query, buffer, functor);
		return this->_ws_client->running_streams[stream_query];
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Start diff depth stream
	@param symbol - the symbol
	@param buffer - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@param interval - interval of the responses, part of the query
	@return an unsigned int representing success
*/
template<typename T>
template <typename FT>
unsigned int Client<T>::stream_depth_diff(const std::string& symbol, std::string& buffer, FT& functor, unsigned int interval)
{
	try
	{
		std::string stream_name = symbol + '@' + "depth" + "@" + std::to_string(interval) + "ms";
		std::string stream_query = "/ws/" + stream_name;
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->template _stream_manager<FT>(stream_name, stream_query, buffer, functor);
		return this->_ws_client->running_streams[stream_query];
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Start User Stream
	@param buffer - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@param ping_listen_key - if true, will ping listen key
	@return an unsigned int representing success
*/
template<typename T>
template <typename FT>
unsigned int Client<T>::stream_userstream(std::string& buffer, FT& functor, const bool ping_listen_key)
{
	try
	{
		return static_cast<T*>(this)->v_stream_userstream(buffer, functor, ping_listen_key);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Start Custom Stream
	@param stream_name - The name of the stream
	@param buffer - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@param ping_listen_key - if true, will ping listen key
	@return an unsigned int representing success
*/
template <typename T>
template <typename FT>
unsigned int Client<T>::custom_stream(const std::string stream_name, std::string& buffer, FT& functor, const bool ping_listen_key)
{
	try
	{
		std::string stream_query = "/stream?streams=" + stream_name;
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->template _stream_manager<FT>(stream_name, stream_query, buffer, functor, ping_listen_key);
		return this->_ws_client->running_streams[stream_query];

	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Open trade stream
	@param symbol - a string reference of the symbol
	@param buffer - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@return an unsigned int representing success
*/
template<typename T>
template <typename FT>
unsigned int Client<T>::stream_Trade(const std::string& symbol, std::string& buffer, FT& functor)
{
	try
	{
		return static_cast<T*>(this)->v_stream_Trade(symbol, buffer, functor);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

/**
	Start userstream (Ops)
	@param buffer - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@param ping_listen_key - if true, will ping listen key periodically
	@return an unsigned int representing success
*/
template <typename FT>
unsigned int OpsClient::v_stream_userstream(std::string& buffer, FT& functor, const bool ping_listen_key)
{
	std::string stream_name = this->get_listen_key();
	std::string stream_query = "/ws/" + stream_name;
	if (this->_ws_client->is_open(stream_query))
	{
		this->_ws_client->close_stream(stream_query);
	}
	this->_ws_client->_stream_manager<FT>(stream_name, stream_query, buffer, functor, ping_listen_key);
	return this->_ws_client->running_streams[stream_query];
}

/**
	Start trades stream (Ops)
	@param symbol - the symbol of the trades
	@param buffer - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@return an unsigned int representing success
*/
template <typename FT>
unsigned int OpsClient::v_stream_Trade(const std::string& symbol, std::string& buffer, FT& functor)
{
	std::string stream_name = symbol + '@' + "trade";
	std::string stream_query = "/ws/" + stream_name;
	if (this->_ws_client->is_open(stream_query))
	{
		this->_ws_client->close_stream(stream_query);
	}
	this->_ws_client->_stream_manager<FT>(stream_name, stream_query, buffer, functor);
	return this->_ws_client->running_streams[stream_query];
}

/**
	Start kline (candlesticks) stream
	@param symbol - the symbol
	@param buffer - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@param interval - interval of the responses, part of the query
	@return an unsigned int representing success
*/
template <typename FT>
unsigned int OpsClient::v_stream_kline(const std::string& symbol, std::string& buffer, FT& functor, std::string interval)
{
	std::string stream_name = symbol + "@" + "kline_" + (interval);
	std::string stream_query = "/ws/" + stream_name;
	if (this->_ws_client->is_open(stream_query))
	{
		this->_ws_client->close_stream(stream_query);
	}
	this->_ws_client->_stream_manager<FT>(stream_name, stream_query, buffer, functor);
	return this->_ws_client->running_streams[stream_query];
}


// Websocket methods templates


/**
	Manage an active stream
	This manager is responsible for reconnecting as well

	@param stream_map_name - The name of the stream
	@param stream_path - The path of the string (query)
	@param buf - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@param ping_listen_key - if true, will ping listen key
	@return an unsigned int representing success
*/
template <typename T>
template <class FT>
void WebsocketClient<T>::_stream_manager(std::string stream_map_name, const std::string stream_path, std::string& buf, FT& functor, const bool ping_listen_key)
{
	unsigned int reconnect_attempts = 0;
	this->running_streams[stream_map_name] = 0; // init
	do
	{
		try
		{
			this->_connect_to_endpoint<FT>(stream_map_name, stream_path, buf, functor, ping_listen_key); // will not proceed unless connection is broken
		}

		catch (ClientException e)
		{
			if (this->running_streams[stream_map_name] && this->_reconnect_on_error) // if reconnect
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

/**
	Starts a new stream
	This method starts the stream and maintains a 'while' loop that loads the websocket messages into 'buf' and
	invoke 'functor' as the callback.
	The member 'running_streams' contains names of active streams. In order to close
	a stream, the name should be removed from there.

	@param stream_map_name - The name of the stream
	@param stream_path - The path of the string (query)
	@param buf - a reference of the string buffer to load responses to
	@param functor - a reference to the functor object to be called as callback
	@param ping_listen_key - if true, will ping listen key
	@return an unsigned int representing success
*/
template <typename T>
template <class FT>
void WebsocketClient<T>::_connect_to_endpoint(const std::string stream_map_name, const std::string stream_path, std::string& buf, FT& functor, const bool ping_listen_key)
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
	ws.handshake(full_host, stream_path);

	beast::error_code ec; // error code

	if (ws.is_open()) // change back to is open
	{
		this->running_streams[stream_map_name] = 1;
		if (this->_gzip_conversion)
		{
			bool conversion_successful{ 0 };
			ws.write(net::buffer(std::string("{\"method\":\"BINARY\", \"params\":[\"false\"], \"id\":1}")));
			while (!conversion_successful)
			{
				auto beast_buffer = boost::asio::dynamic_buffer(buf);
				ws.read(beast_buffer, ec);
				conversion_successful = (buf == "{\"id\":1}");
				buf.clear();
			}
		}
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
				if (current_timestamp - last_keepalive > this->exchange_client->refresh_listenkey_interval)
				{
					this->exchange_client->ping_listen_key();
					last_keepalive = current_timestamp;
				}
			}
			auto beast_buffer = boost::asio::dynamic_buffer(buf); // impossible to declare just once...
			ws.read(beast_buffer, ec);
			if (ec)
			{
				if (!this->_reconnect_on_error) this->running_streams[stream_map_name] = 0; // to exit loop if not retry
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
				try
				{
					ws.close(beast::websocket::close_code::bad_payload, ec);
				}
				catch (...)
				{
					BadStreamCloseWS e{};
					e.append_to_traceback(std::string(__FUNCTION__));
					throw(e);
				}
			}

			BadStreamCallbackWS e{};
			e.append_to_traceback(std::string(__FUNCTION__));
			throw(e);
		}
	}
	try
	{
		ws.close(beast::websocket::close_code::normal, ec);
	}
	catch (...)
	{
		BadStreamCloseWS e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}

}
