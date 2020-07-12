// todo: function that checks all handles arguments
// todo: check response status code, pass "json: status" or something
// todo: what can you pass to postfields? what is the format?
// regarding above: always leave an empty json of "status: true" to reduce runtime cost
// note: if needed, add application/x-www-form-urlencoded to post in request

static long _IDLE_TIME_TCP = 120L;
static long _INTVL_TIME_TCP = 60L;
Json::CharReaderBuilder _J_BUILDER;
Json::CharReader* _J_READER = _J_BUILDER.newCharReader();


class RestSession
{
private:

	std::string _req_raw_get; // todo -> make this get_response and flush everytime	
	Json::Value _req_json_get;
	CURLcode _get_status;

	std::string _req_raw_post; // todo -> make this get_response and flush everytime	
	Json::Value _req_json_post;
	CURLcode _post_status;

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

	friend unsigned int _GET_CALLBACK(void* contents, unsigned int size, unsigned int nmemb, RestSession* self); // different because of members
	friend unsigned int _POST_CALLBACK(void* contents, unsigned int size, unsigned int nmemb, RestSession* self);

	~RestSession();
};


unsigned int _GET_CALLBACK(void* contents, unsigned int size, unsigned int nmemb, RestSession* self)
{
	self->_req_raw_get.clear(); // flush old data
	self->_req_json_get.clear();
	(&self->_req_raw_get)->append((char*)contents, size * nmemb);

	std::string parse_errors;
	bool parse_status = _J_READER->parse(self->_req_raw_get.c_str(),
		self->_req_raw_get.c_str() + self->_req_raw_get.size(),
		&self->_req_json_get,
		&parse_errors);

	self->_req_json_get["request_status"] = 0;


	if (self->_get_status != CURLE_OK || self->_get_status == CURLE_HTTP_RETURNED_ERROR)
	{
		self->_req_json_get["response"] = self->_req_raw_get;

		return 0;
	}

	else if (!parse_status)
	{
		self->_req_json_get["parse_status"] = parse_errors;
		return size * nmemb;
	}

	else if (self->_req_json_get.isMember("code"))
	{
		return size * nmemb;
	}

	self->_req_json_get["request_status"] = 1;

	// todo: handle parse_errors

	return size * nmemb;
};

unsigned int _POST_CALLBACK(void* contents, unsigned int size, unsigned int nmemb, RestSession* self)
{
	self->_req_raw_post.clear(); // flush old data
	self->_req_json_post.clear();
	(&self->_req_raw_post)->append((char*)contents, size * nmemb);

	std::string parse_errors;
	bool parse_status = _J_READER->parse(self->_req_raw_post.c_str(),
					  	self->_req_raw_post.c_str() + self->_req_raw_post.size(),
						&self->_req_json_post,
						&parse_errors);

	self->_req_json_post["request_status"] = 0;

	if (self->_post_status != CURLE_OK || self->_post_status == CURLE_HTTP_RETURNED_ERROR)
	{
		self->_req_json_post["response"] = self->_req_raw_post;

		return 0;
	}

	else if (!parse_status)
	{
		self->_req_json_post["parse_status"] = parse_errors;
		return size * nmemb;
	}

	else if (self->_req_json_post.isMember("code"))
	{
		return size * nmemb;
	}

	self->_req_json_get["request_status"] = 1;

	return size * nmemb;
};

RestSession::RestSession()
{
	_get_handle = curl_easy_init();
	curl_easy_setopt(this->_get_handle, CURLOPT_HTTPGET, 1L);
	curl_easy_setopt(this->_get_handle, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(this->_get_handle, CURLOPT_WRITEFUNCTION, _GET_CALLBACK);
	curl_easy_setopt(this->_get_handle, CURLOPT_WRITEDATA, this);
	curl_easy_setopt(this->_get_handle, CURLOPT_FAILONERROR, 0); // excplicitly set to 0


	_post_handle = curl_easy_init();
	curl_easy_setopt(this->_post_handle, CURLOPT_POST, 1L);
	curl_easy_setopt(this->_post_handle, CURLOPT_POSTFIELDSIZE, 0);
	curl_easy_setopt(this->_post_handle, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(this->_post_handle, CURLOPT_WRITEFUNCTION, _POST_CALLBACK);
	curl_easy_setopt(this->_post_handle, CURLOPT_WRITEDATA, this);
	curl_easy_setopt(this->_post_handle, CURLOPT_FAILONERROR, 0); // excplicitly set to 0


	if (!(this->_get_handle)) throw("exc"); // handle exc
	if (!(this->_post_handle)) throw("exc"); // handle exc

	status = 1;
}

Json::Value RestSession::_getreq(std::string path)
{
	curl_easy_setopt(this->_get_handle, CURLOPT_URL, path.c_str());

	this->_get_status = curl_easy_perform(this->_get_handle);

	return this->_req_json_get;
};

Json::Value RestSession::_postreq(std::string path)
{
	curl_easy_setopt(this->_post_handle, CURLOPT_URL, path.c_str());

	this->_post_status = curl_easy_perform(this->_post_handle);
		
	return this->_req_json_post;
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
