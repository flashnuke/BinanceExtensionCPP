#ifndef WS_CLIENT_H
#define WS_CLIENT_H


template <typename T>
class WebsocketClient
{
private:
	std::string _host; // not const because of testnet
	std::string _port;
	T exchange_client; // user client obj


	template <typename FT>
	void _connect_to_endpoint(const std::string stream_map_name, std::string& buf, FT& functor, const bool ping_listen_key); // todo: make stream map name const ref?

public:
	unsigned int _max_reconnect_count;
	bool _reconnect_on_error;

	WebsocketClient(T& exchange_client, const std::string host, const unsigned int port);

	std::unordered_map<std::string, bool> running_streams; // will be a map, containing pairs of: <bool(status), ws_stream> 

	void close_stream(const std::string& full_stream_name);
	std::vector<std::string> open_streams();
	bool is_open(const std::string& stream_name) const;

	template <typename FT>
	void _stream_manager(std::string stream_map_name, std::string& buf, FT& functor, const bool ping_listen_key = 0);

	void _set_reconnect(const bool& reconnect);

	void set_host_port(const std::string new_host, const unsigned int new_port);

	~WebsocketClient();

};


#endif
