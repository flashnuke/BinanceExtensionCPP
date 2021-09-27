#include "../include/Binance_Client.h"

Json::CharReaderBuilder _J_BUILDER;
Json::CharReader* _J_READER = _J_BUILDER.newCharReader();
static long _IDLE_TIME_TCP = 120L;
static long _INTVL_TIME_TCP = 60L;

/**
	Request callback method (REST)
	@param contents - text response
	@param size - size of text
	@param nmemb - number of elements
	@param req - RestSession object of the request
	@return 0 if error, 1 if success
*/
unsigned int _REQ_CALLBACK(void* contents, unsigned int size, unsigned int nmemb, RestSession::RequestHandler* req) 
{
	(&req->req_raw)->append((char*)contents, size * nmemb);
	return size * nmemb;
};

/**
 	Parse request response string to JSON
	@param req - RestSesssion object of the request
	@return 0 is request was unsuccessful
 */
unsigned int _PARSE_AND_VALIDATE(RestSession::RequestHandler* req)
{
	std::string parse_errors{};
	bool parse_status;
	
	parse_status = _J_READER->parse(req->req_raw.c_str(),
					req->req_raw.c_str() + req->req_raw.size(),
					&req->req_json["response"],
					&parse_errors);
	
	if (req->req_status != CURLE_OK || req->req_status == CURLE_HTTP_RETURNED_ERROR)
	{
		req->req_json["response"] = req->req_raw;
		return 0;
	}
	else if (!parse_status)
	{
		req->req_json["parse_status"] = parse_errors;
		return 0;
	}
	else if (req->req_json.isMember("code") && req->req_json["code"] != 200)
	{
		return 0;
	}
	req->req_json["request_status"] = 1;
	return 1;
};

/**
	Default Constructor
	Set all CURL handles (GET, POST, PUT, DELETE)
*/
RestSession::RestSession() // except handles in rest_init exchange client level
{
	_get_handle = curl_easy_init();
	curl_easy_setopt(this->_get_handle, CURLOPT_HTTPGET, 1L);
	curl_easy_setopt(this->_get_handle, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(this->_get_handle, CURLOPT_WRITEFUNCTION, _REQ_CALLBACK);
	curl_easy_setopt(this->_get_handle, CURLOPT_FAILONERROR, 0); 
	if (!(this->_get_handle)) throw("BAD_GET_SETUP");

	_post_handle = curl_easy_init();
	curl_easy_setopt(this->_post_handle, CURLOPT_POST, 1L);
	curl_easy_setopt(this->_post_handle, CURLOPT_POSTFIELDSIZE, 0);
	curl_easy_setopt(this->_post_handle, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(this->_post_handle, CURLOPT_WRITEFUNCTION, _REQ_CALLBACK);
	curl_easy_setopt(this->_post_handle, CURLOPT_FAILONERROR, 0);
	if (!(this->_post_handle)) throw("BAD_POST_SETUP"); // handle exc

	_put_handle = curl_easy_init();
	curl_easy_setopt(this->_put_handle, CURLOPT_UPLOAD, 1L);
	curl_easy_setopt(this->_put_handle, CURLOPT_INFILESIZE, 0);
	curl_easy_setopt(this->_put_handle, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(this->_put_handle, CURLOPT_WRITEFUNCTION, _REQ_CALLBACK);
	curl_easy_setopt(this->_put_handle, CURLOPT_FAILONERROR, 0);
	if (!(this->_put_handle)) throw("BAD_PUT_SETUP"); // handle exc

	_delete_handle = curl_easy_init();
	curl_easy_setopt(this->_delete_handle, CURLOPT_CUSTOMREQUEST, "DELETE");
	curl_easy_setopt(this->_delete_handle, CURLOPT_POSTFIELDSIZE, 0);
	curl_easy_setopt(this->_delete_handle, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(this->_delete_handle, CURLOPT_WRITEFUNCTION, _REQ_CALLBACK);
	curl_easy_setopt(this->_delete_handle, CURLOPT_FAILONERROR, 0);
	if (!(this->_delete_handle)) throw("BAD_DELETE_SETUP"); // handle exc

	this->status = 1;
}

/**
	Enable verbose state for the session
	@param state - 1 to enable, 0 to disable
*/
void RestSession::set_verbose(const long int state)
{
	curl_easy_setopt(this->_get_handle, CURLOPT_VERBOSE, state);
	curl_easy_setopt(this->_post_handle, CURLOPT_VERBOSE, state);
	curl_easy_setopt(this->_put_handle, CURLOPT_VERBOSE, state);
	curl_easy_setopt(this->_delete_handle, CURLOPT_VERBOSE, state);

}

/**
	Make a GET request
	@param full_path - the full path of the request
	@return a JSON value returned by the request response
*/
Json::Value RestSession::_getreq(std::string full_path)
{
	try
	{
		RequestHandler request{};

		std::unique_lock<std::mutex> req_lock(this->_get_lock); // will be unlocked in callback
		request.locker = &req_lock;

		curl_easy_setopt(this->_get_handle, CURLOPT_URL, full_path.c_str());
		curl_easy_setopt(this->_get_handle, CURLOPT_WRITEDATA, &request);

		request.req_status = curl_easy_perform(this->_get_handle);
		_PARSE_AND_VALIDATE(&request);
		request.locker->unlock();

		return request.req_json;
	}
	catch (...)
	{
		BadRequestREST e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Make a POST request
	@param full_path - the full path of the request
	@return a JSON value returned by the request response
*/
Json::Value RestSession::_postreq(std::string full_path)
{
	try
	{
		RequestHandler request{};

		std::unique_lock<std::mutex> req_lock(this->_post_lock); // will be unlocked in callback
		request.locker = &req_lock;

		curl_easy_setopt(this->_post_handle, CURLOPT_URL, full_path.c_str());
		curl_easy_setopt(this->_post_handle, CURLOPT_WRITEDATA, &request);

		request.req_status = curl_easy_perform(this->_post_handle);
		_PARSE_AND_VALIDATE(&request); 
		request.locker->unlock();

		return request.req_json;
	}
	catch (...)
	{
		BadRequestREST e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Make a PUT request
	@param full_path - the full path of the request
	@return a JSON value returned by the request response
*/
Json::Value RestSession::_putreq(std::string full_path)
{
	try
	{
		RequestHandler request{};

		std::unique_lock<std::mutex> req_lock(this->_put_lock); // will be unlocked in callback
		request.locker = &req_lock;

		curl_easy_setopt(this->_put_handle, CURLOPT_URL, full_path.c_str());
		curl_easy_setopt(this->_put_handle, CURLOPT_WRITEDATA, &request);

		request.req_status = curl_easy_perform(this->_put_handle);
		_PARSE_AND_VALIDATE(&request); 
		request.locker->unlock();

		return request.req_json;
	}
	catch (...)
	{
		BadRequestREST e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Make a DELETE request
	@param full_path - the full path of the request
	@return a JSON value returned by the request response
*/
Json::Value RestSession::_deletereq(std::string full_path)
{
	try
	{
		RequestHandler request{};

		std::unique_lock<std::mutex> req_lock(this->_delete_lock); // will be unlocked in callback
		request.locker = &req_lock;

		curl_easy_setopt(this->_delete_handle, CURLOPT_URL, full_path.c_str());
		curl_easy_setopt(this->_delete_handle, CURLOPT_WRITEDATA, &request);

		request.req_status = curl_easy_perform(this->_delete_handle);
		_PARSE_AND_VALIDATE(&request); 
		request.locker->unlock();

		return request.req_json;
	}
	catch (...)
	{
		BadRequestREST e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};


/**
	Close the current session
	@return a boolean value representing success for closing the session
*/
bool RestSession::close()
{
	try
	{
		if (this->status)
		{
			curl_easy_cleanup(this->_post_handle);
			curl_easy_cleanup(this->_get_handle);
			curl_easy_cleanup(this->_put_handle);
			curl_easy_cleanup(this->_delete_handle);
		}

		this->status = 0;
		return 1;
	}
	catch (...)
	{
		BadCleanupREST e{};
		e.append_to_traceback(std::string(__FUNCTION__));
		throw(e);
	}
};

/**
	Default constructor
	this class is used to handle request responses
*/
RestSession::RequestHandler::RequestHandler()
	: req_raw{ "" }, req_json{ Json::Value{} }, req_status{ CURLcode{} }, locker { nullptr }
{
	req_json["request_status"] = 0;
	req_json["response"] = Json::arrayValue;

};

/**
	Destructor
	close the session upon deletion
*/
RestSession::~RestSession()
{
	this->close();
}
