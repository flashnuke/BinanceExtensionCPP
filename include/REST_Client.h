#ifndef REST_CLIENT_H
#define REST_CLIENT_H

class RestSession
{
private:

	struct RequestHandler // handles response
	{
		RequestHandler();
		std::string req_raw;
		Json::Value req_json;
		CURLcode req_status;
		std::unique_lock<std::mutex>* locker;
	};


public:
	RestSession();

	bool status; // bool for whether session is active or not

	CURL* _get_handle{};
	CURL* _post_handle{};
	CURL* _put_handle{};
	CURL* _delete_handle{};

	Json::Value _getreq(std::string full_path);
	void get_timeout(unsigned long interval);
	std::mutex _get_lock;

	Json::Value _postreq(std::string full_path);
	void post_timeout(unsigned long interval);
	std::mutex _post_lock;

	Json::Value _putreq(std::string full_path);
	void put_timeout(unsigned long interval);
	std::mutex _put_lock;

	Json::Value _deletereq(std::string full_path);
	void delete_timeout(unsigned long interval);
	std::mutex _delete_lock;

	bool close();
	void set_verbose(const long int state);

	friend unsigned int _REQ_CALLBACK(void* contents, unsigned int size, unsigned int nmemb, RestSession::RequestHandler* req);
	friend unsigned int _PARSE_AND_VALIDATE(RestSession::RequestHandler* req);
	~RestSession();
};



#endif
