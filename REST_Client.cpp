// todo: function that checks all handles arguments

static long _IDLE_TIME_TCP = 120L;
static long _INTVL_TIME_TCP = 60L;
Json::CharReaderBuilder _J_BUILDER;
Json::CharReader* _J_READER = _J_BUILDER.newCharReader();


class RestSession
{
private:

	CURLcode _post_status; // move from here
	std::string _req_raw; // todo -> make this get_response and flush everytime	
	Json::Value _req_json;

public:
	RestSession();

	bool status; // bool for whether session is active or not
	CURL* _get_handle;
	CURL* _post_handle;

	Json::Value _getreq(std::string path);
	inline void get_timeout(unsigned long interval);

	Json::Value _postreq(std::string path);
	inline void post_timeout(unsigned long interval);

	void close();

	friend unsigned int _REQ_CALLBACK(void* contents, unsigned int size, unsigned int nmemb, RestSession* self);

	~RestSession();
};


unsigned int _REQ_CALLBACK(void* contents, unsigned int size, unsigned int nmemb, RestSession* self)
{
	self->_req_raw.clear(); // flush old data
	self->_req_json.clear();
	(&self->_req_raw)->append((char*)contents, size * nmemb);

	std::string parse_errors;
	_J_READER->parse(self->_req_raw.c_str(),
		 			self->_req_raw.c_str() + self->_req_raw.size(),
			 		&self->_req_json,
			   		&parse_errors);

	// todo: handle parse_errors

	return size * nmemb;
};

RestSession::RestSession()
{
	_get_handle = curl_easy_init();
	curl_easy_setopt(_get_handle, CURLOPT_HTTPGET, 1L);
	curl_easy_setopt(_get_handle, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(this->_get_handle, CURLOPT_WRITEFUNCTION, _REQ_CALLBACK);
	curl_easy_setopt(this->_get_handle, CURLOPT_WRITEDATA, this);

	if (!(_get_handle)) throw("exc");

	_post_handle = curl_easy_init();
	curl_easy_setopt(_post_handle, CURLOPT_POST, 1L);
	curl_easy_setopt(_post_handle, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(this->_post_handle, CURLOPT_WRITEFUNCTION, _REQ_CALLBACK);
	curl_easy_setopt(this->_post_handle, CURLOPT_WRITEDATA, this);

	if (!(_post_handle)) throw("exc");

	status = 1;
}

Json::Value RestSession::_getreq(std::string path)
{
	curl_easy_setopt(this->_get_handle, CURLOPT_URL, path.c_str());

	CURLcode _get_status;
	_get_status = curl_easy_perform(this->_get_handle);

	if (_get_status != CURLE_OK)
	{
		std::cout << curl_easy_strerror(_get_status);
	}

	return _req_json;
};

Json::Value RestSession::_postreq(std::string path)
{
	curl_easy_setopt(this->_post_handle, CURLOPT_URL, path.c_str());

	CURLcode _post_status;
	_post_status = curl_easy_perform(this->_post_handle);

	if (_post_status != CURLE_OK)
	{
		std::cout << curl_easy_strerror(_post_status);
	}

	return _req_json;
};

inline void RestSession::get_timeout(unsigned long interval) { curl_easy_setopt(this->_get_handle, CURLOPT_TIMEOUT, interval); };
inline void RestSession::post_timeout(unsigned long interval) { curl_easy_setopt(this->_post_handle, CURLOPT_TIMEOUT, interval); };

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
	std::cout << "des called\n";
}
