#include "../Binance_Client.h"

// Client stream templates

template <typename T>
template <typename FT>
unsigned int Client<T>::MarginAccount::margin_stream_userStream(std::string& buffer, FT& functor, const bool ping_listen_key, const bool& isolated_margin_type)
{
	try
	{
		std::string stream_query = "/ws/" + this->margin_get_listen_key(isolated_margin_type);
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->_stream_manager<FT>(stream_query, buffer, functor, ping_listen_key);
		return this->_ws_client->running_streams[stream_query];
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}

}

template <typename FT>
unsigned int SpotClient::v_stream_userStream(std::string& buffer, FT& functor, const bool ping_listen_key)
{
	std::string stream_query = "/ws/" + this->get_listen_key();
	if (this->_ws_client->is_open(stream_query))
	{
		this->_ws_client->close_stream(stream_query);
	}
	this->_ws_client->_stream_manager<FT>(stream_query, buffer, functor, ping_listen_key);
	return this->_ws_client->running_streams[stream_query];

}

template <typename FT>
unsigned int SpotClient::v_stream_Trade(std::string symbol, std::string& buffer, FT& functor)
{
	std::string stream_query = "/ws/" + symbol + '@' + "trade";
	if (this->_ws_client->is_open(stream_query))
	{
		this->_ws_client->close_stream(stream_query);
	}
	this->_ws_client->_stream_manager<FT>(stream_query, buffer, functor, ping_listen_key);
	return this->_ws_client->running_streams[stream_query];
}

template<typename CT>
template <typename FT>
unsigned int FuturesClient<CT>::stream_markprice(const std::string& symbol, std::string& buffer, FT& functor, unsigned int interval)
{
	try
	{
		std::string stream_query = "/ws/" + symbol + '@' + "markPrice" + std::to_string(interval) + "ms";
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->_stream_manager<FT>(stream_query, buffer, functor);
		return this->_ws_client->running_streams[stream_query];
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}



template <typename CT>
template <typename FT>
unsigned int FuturesClient<CT>::v_stream_Trade(std::string symbol, std::string& buffer, FT& functor)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}


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
}  // only USDT

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




template<typename CT>
template <typename FT>
unsigned int FuturesClient<CT>::stream_liquidation_orders(const std::string& symbol, std::string& buffer, FT& functor)
{
	try
	{
		std::string stream_query = "/ws/" + symbol + "@" + "forceOrder";
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->_stream_manager<FT>(stream_query, buffer, functor);
		return this->_ws_client->running_streams[stream_query];
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

template<typename CT>
template <typename FT>
unsigned int FuturesClient<CT>::stream_liquidation_orders_all(std::string& buffer, FT& functor)
{
	try
	{
		std::string stream_query = "/ws/!forceOrder@arr";
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->_stream_manager<FT>(stream_query, buffer, functor);
		return this->_ws_client->running_streams[stream_query];
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

template<typename CT>
template <typename FT>
unsigned int FuturesClient<CT>::v_stream_userStream(std::string& buffer, FT& functor, const bool ping_listen_key) { return static_cast<CT*>(this)->v__stream_userStream(buffer, functor, ping_listen_key); }


template <typename FT>
unsigned int FuturesClientUSDT::v_stream_markprice_all(const std::string& symbol, std::string& buffer, FT& functor)
{
	std::string stream_query = "/ws/" + symbol + '@' + "miniTicker";
	if (this->_ws_client->is_open(stream_query))
	{
		this->_ws_client->close_stream(stream_query);
	}
	this->_ws_client->_stream_manager<FT>(stream_query, buffer, functor);
	return this->_ws_client->running_streams[stream_query];
}


template <typename FT>
unsigned int FuturesClientUSDT::v_stream_indexprice(const std::string& pair, std::string& buffer, FT& functor, unsigned int interval)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}

template <typename FT>
unsigned int FuturesClientUSDT::v_stream_markprice_by_pair(const std::string& pair, std::string& buffer, FT& functor, unsigned int interval)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}

template <typename FT>
unsigned int FuturesClientUSDT::v_stream_kline_contract(const std::string& pair_and_type, std::string& buffer, FT& functor, std::string interval)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}

template <typename FT>
unsigned int FuturesClientUSDT::v_stream_kline_index(const std::string& pair, std::string& buffer, FT& functor, std::string interval)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}

template <typename FT>
unsigned int FuturesClientUSDT::v_stream_kline_markprice(const std::string& symbol, std::string& buffer, FT& functor, std::string interval)
{
	MissingEndpoint e{};
	e.append_to_traceback(std::string(__FUNCTION__));
	throw(e);
}

template <typename FT>
unsigned int FuturesClientUSDT::v__stream_userStream(std::string& buffer, FT& functor, const bool ping_listen_key)
{
	std::string stream_query = "/ws/" + this->get_listen_key();
	if (this->_ws_client->is_open(stream_query))
	{
		this->_ws_client->close_stream(stream_query);
	}
	this->_ws_client->_stream_manager<FT>(stream_query, buffer, functor);
	return this->_ws_client->running_streams[stream_query];
}

template <typename FT>
unsigned int FuturesClientCoin::v_stream_indexprice(const std::string& pair, std::string& buffer, FT& functor, unsigned int interval)
{
	std::string stream_query = "/ws/" + pair + "@" + "indexPrice" + "@" + std::to_string(interval) + "ms";
	if (this->_ws_client->is_open(stream_query))
	{
		this->_ws_client->close_stream(stream_query);
	}
	this->_ws_client->_stream_manager<FT>(stream_query, buffer, functor);
	return this->_ws_client->running_streams[stream_query];
}

template <typename FT>
unsigned int FuturesClientCoin::v_stream_markprice_by_pair(const std::string& pair, std::string& buffer, FT& functor, unsigned int interval)
{
	std::string stream_query = "/ws/" + pair + "@" + "markPrice" + "@" + std::to_string(interval) + "ms";
	if (this->_ws_client->is_open(stream_query))
	{
		this->_ws_client->close_stream(stream_query);
	}
	this->_ws_client->_stream_manager<FT>(stream_query, buffer, functor);
	return this->_ws_client->running_streams[stream_query];
}

template <typename FT>
unsigned int FuturesClientCoin::v_stream_kline_contract(const std::string& pair_and_type, std::string& buffer, FT& functor, std::string interval)
{
	std::string stream_query = "/ws/" + pair_and_type + "@" + "continuousKline_" + (interval);
	if (this->_ws_client->is_open(stream_query))
	{
		this->_ws_client->close_stream(stream_query);
	}
	this->_ws_client->_stream_manager<FT>(stream_query, buffer, functor);
	return this->_ws_client->running_streams[stream_query];
}

template <typename FT>
unsigned int FuturesClientCoin::v_stream_kline_index(const std::string& pair, std::string& buffer, FT& functor, std::string interval)
{
	std::string stream_query = "/ws/" + pair + "@" + "indexPriceKline_" + (interval);
	if (this->_ws_client->is_open(stream_query))
	{
		this->_ws_client->close_stream(stream_query);
	}
	this->_ws_client->_stream_manager<FT>(stream_query, buffer, functor);
	return this->_ws_client->running_streams[stream_query];
}

template <typename FT>
unsigned int FuturesClientCoin::v_stream_kline_markprice(const std::string& symbol, std::string& buffer, FT& functor, std::string interval)
{
	std::string stream_query = "/ws/" + symbol + "@" + "markPriceKline_" + (interval);
	if (this->_ws_client->is_open(stream_query))
	{
		this->_ws_client->close_stream(stream_query);
	}
	this->_ws_client->_stream_manager<FT>(stream_query, buffer, functor);
	return this->_ws_client->running_streams[stream_query];
}

template <typename FT>
unsigned int FuturesClientCoin::v__stream_userStream(std::string& buffer, FT& functor, const bool ping_listen_key)
{
	std::string stream_query = "/ws/" + this->get_listen_key();
	if (this->_ws_client->is_open(stream_query))
	{
		this->_ws_client->close_stream(stream_query);
	}
	this->_ws_client->_stream_manager<FT>(stream_query, buffer, functor, ping_listen_key);
	return this->_ws_client->running_streams[stream_query];
}


template<typename T>
template <typename FT>
unsigned int  Client<T>::stream_aggTrade(const std::string& symbol, std::string& buffer, FT& functor)
{
	try
	{
		std::string stream_query = "/ws/" + symbol + '@' + "aggTrade";
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->_stream_manager<FT>(stream_query, buffer, functor);
		return this->_ws_client->running_streams[stream_query];
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}


template<typename T>
template <typename FT>
unsigned int Client<T>::stream_kline(const std::string& symbol, std::string& buffer, FT& functor, std::string interval)
{
	try
	{
		std::string stream_query = "/ws/" + symbol + '@' + "kline_" + interval;
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->_stream_manager<FT>(stream_query, buffer, functor);
		return this->_ws_client->running_streams[stream_query];
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

template<typename T>
template <typename FT>
unsigned int Client<T>::stream_ticker_ind_mini(const std::string& symbol, std::string& buffer, FT& functor)
{
	try
	{
		std::string stream_query = "/ws/" + symbol + '@' + "miniTicker";
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->_stream_manager<FT>(stream_query, buffer, functor);
		return this->_ws_client->running_streams[stream_query];
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

template<typename T>
template <typename FT>
unsigned int Client<T>::stream_ticker_all_mini(std::string& buffer, FT& functor)
{
	try
	{
		std::string stream_query = "/ws/!miniTicker@arr";
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->_stream_manager<FT>(stream_query, buffer, functor);
		return this->_ws_client->running_streams[stream_query];
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

template<typename T>
template <typename FT>
unsigned int Client<T>::stream_ticker_ind(const std::string& symbol, std::string& buffer, FT& functor)
{
	try
	{
		std::string stream_query = "/ws/" + symbol + "@" + "ticker";
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->_stream_manager<FT>(stream_query, buffer, functor);
		return this->_ws_client->running_streams[stream_query];
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

template<typename T>
template <typename FT>
unsigned int Client<T>::stream_ticker_all(std::string& buffer, FT& functor)
{
	try
	{
		std::string stream_query = "/ws/!ticker@arr";
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->_stream_manager<FT>(stream_query, buffer, functor);
		return this->_ws_client->running_streams[stream_query];
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

template<typename T>
template <typename FT>
unsigned int Client<T>::stream_ticker_ind_book(const std::string& symbol, std::string& buffer, FT& functor)
{
	try
	{
		std::string stream_query = "/ws/" + symbol + "@" + "bookTicker";
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->_stream_manager<FT>(stream_query, buffer, functor);
		return this->_ws_client->running_streams[stream_query];
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

template<typename T>
template <typename FT>
unsigned int Client<T>::stream_ticker_all_book(std::string& buffer, FT& functor)
{
	try
	{
		std::string stream_query = "/ws/!bookTicker";
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->_stream_manager<FT>(stream_query, buffer, functor);
		return this->_ws_client->running_streams[stream_query];
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

template<typename T>
template <typename FT>
unsigned int Client<T>::stream_depth_partial(const std::string& symbol, std::string& buffer, FT& functor, unsigned int levels, unsigned int interval)
{
	try
	{
		std::string stream_query = "/ws/" + symbol + "@" + "depth" + std::to_string(levels) + "@" + std::to_string(interval) + "ms";
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->_stream_manager<FT>(stream_query, buffer, functor);
		return this->_ws_client->running_streams[stream_query];
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}

template<typename T>
template <typename FT>
unsigned int Client<T>::stream_depth_diff(const std::string& symbol, std::string& buffer, FT& functor, unsigned int interval)
{
	try
	{
		std::string stream_query = "/ws/" + symbol + '@' + "depth" + "@" + std::to_string(interval) + "ms";
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->_stream_manager<FT>(stream_query, buffer, functor);
		return this->_ws_client->running_streams[stream_query];
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}


template<typename T>
template <typename FT>
unsigned int Client<T>::stream_userStream(std::string& buffer, FT& functor, const bool ping_listen_key)
{
	try
	{
		return static_cast<T*>(this)->v_stream_userStream(buffer, functor, ping_listen_key);
	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}


template <typename T>
template <typename FT>
unsigned int Client<T>::custom_stream(std::string stream_query, std::string buffer, FT functor, const bool ping_listen_key)
{
	try
	{
		stream_query = "/stream?streams=" + stream_query;
		if (this->_ws_client->is_open(stream_query))
		{
			this->_ws_client->close_stream(stream_query);
		}
		this->_ws_client->_stream_manager<FT>(stream_query, buffer, functor, ping_listen_key);
		return this->_ws_client->running_streams[stream_query];

	}
	catch (ClientException e)
	{
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
}


// Websocket methods templates


template <typename T>
template <class FT>
void WebsocketClient<T>::_stream_manager(std::string stream_map_name, std::string& buf, FT& functor, const bool ping_listen_key)
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
