#ifndef curl
#include <curl/curl.h>
#endif

#include <iostream>
// todo: function that checks all handles arguments

static long _IDLE_TIME_TCP = 120L;
static long _INTVL_TIME_TCP = 60L;

unsigned int _REQ_CALLBACK(void* contents, unsigned int size, unsigned int nmemb, std::string* container)
{
	*container = ""; // flush old data
	container->append((char*)contents, size * nmemb);

	return size * nmemb;
};

class RestSession
{
private:
	CURL* _get_handle;

	CURL* _post_handle;
	CURLcode _post_status; // move from here
	std::string _req_response; // todo -> make this get_response and flush everytime	

public:
	RestSession();

	bool status; // bool for whether session is active or not

	std::string _getreq(std::string path);
	inline void get_timeout(unsigned long interval);

	std::string _postreq(std::string path);

	inline void post_timeout(unsigned long interval);

	void close();

	~RestSession();
};

RestSession::RestSession()
{
	std::cout << "init";
	_get_handle = curl_easy_init();
	curl_easy_setopt(_get_handle, CURLOPT_HTTPGET, 1L);
	curl_easy_setopt(_get_handle, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(this->_get_handle, CURLOPT_WRITEFUNCTION, _REQ_CALLBACK);
	curl_easy_setopt(this->_get_handle, CURLOPT_WRITEDATA, &this->_req_response);

	if (!(_get_handle)) throw("exc");

	_post_handle = curl_easy_init();
	curl_easy_setopt(_post_handle, CURLOPT_POST, 1L);
	curl_easy_setopt(_post_handle, CURLOPT_FOLLOWLOCATION, 1L);

	if (!(_post_handle)) throw("exc");


	status = 1;
}

std::string RestSession::_getreq(std::string path)
{


	curl_easy_setopt(this->_get_handle, CURLOPT_URL, path.c_str());

	CURLcode _get_status;
	_get_status = curl_easy_perform(this->_get_handle);

	if (_get_status != CURLE_OK)
	{
		std::cout << curl_easy_strerror(_get_status);
	}

	return this->_req_response;
};




void RestSession::get_timeout(unsigned long interval) { curl_easy_setopt(this->_get_handle, CURLOPT_TIMEOUT, interval); };
void RestSession::post_timeout(unsigned long interval) { curl_easy_setopt(this->_post_handle, CURLOPT_TIMEOUT, interval); };

void RestSession::close()
{
	if (this->status)
	{
		curl_easy_cleanup(this->_post_handle);
		curl_easy_cleanup(this->_get_handle);
	}

	this->status = 0;
};


RestSession::~RestSession()
{
	this->close();
	std::cout << "des called";
}