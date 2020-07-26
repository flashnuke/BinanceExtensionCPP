
#include "CryptoExtensions.h"

Json::CharReaderBuilder _J_BUILDER;
Json::CharReader* _J_READER = _J_BUILDER.newCharReader();
static long _IDLE_TIME_TCP = 120L;
static long _INTVL_TIME_TCP = 60L;


unsigned int _REQ_CALLBACK(void* contents, unsigned int size, unsigned int nmemb, RestSession::RequestHandler* req)
{
	(&req->req_raw)->append((char*)contents, size * nmemb);

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
		return size * nmemb;
	}

	else if (req->req_json.isMember("code"))
	{
		return size * nmemb;
	}
	req->req_json["request_status"] = 1;

	return size * nmemb;
};

RestSession::RestSession()
{
	_get_handle = curl_easy_init();
	curl_easy_setopt(this->_get_handle, CURLOPT_HTTPGET, 1L);
	curl_easy_setopt(this->_get_handle, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(this->_get_handle, CURLOPT_WRITEFUNCTION, _REQ_CALLBACK);
	curl_easy_setopt(this->_get_handle, CURLOPT_FAILONERROR, 0); 


	_post_handle = curl_easy_init();
	curl_easy_setopt(this->_post_handle, CURLOPT_POST, 1L);
	curl_easy_setopt(this->_post_handle, CURLOPT_POSTFIELDSIZE, 0);
	curl_easy_setopt(this->_post_handle, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(this->_post_handle, CURLOPT_WRITEFUNCTION, _REQ_CALLBACK);
	curl_easy_setopt(this->_post_handle, CURLOPT_FAILONERROR, 0);

	_put_handle = curl_easy_init();
	curl_easy_setopt(this->_put_handle, CURLOPT_UPLOAD, 1L);
	curl_easy_setopt(this->_put_handle, CURLOPT_INFILESIZE, 0);
	curl_easy_setopt(this->_put_handle, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(this->_put_handle, CURLOPT_WRITEFUNCTION, _REQ_CALLBACK);
	curl_easy_setopt(this->_put_handle, CURLOPT_FAILONERROR, 0);


	if (!(this->_get_handle)) throw("exc"); // handle exc
	if (!(this->_post_handle)) throw("exc"); // handle exc
	if (!(this->_put_handle)) throw("exc"); // handle exc


	status = 1;
}

Json::Value RestSession::_getreq(std::string full_path)
{
	curl_easy_setopt(this->_get_handle, CURLOPT_URL, full_path.c_str());

	RequestHandler request{};
	curl_easy_setopt(this->_get_handle, CURLOPT_WRITEDATA, &request);

	request.req_status = curl_easy_perform(this->_get_handle);

	return request.req_json;
};

Json::Value RestSession::_postreq(std::string full_path)
{
	curl_easy_setopt(this->_post_handle, CURLOPT_URL, full_path.c_str());

	RequestHandler request{};

	curl_easy_setopt(this->_post_handle, CURLOPT_WRITEDATA, &request);

	request.req_status = curl_easy_perform(this->_post_handle);

	return request.req_json;
};

Json::Value RestSession::_putreq(std::string full_path)
{
	curl_easy_setopt(this->_put_handle, CURLOPT_URL, full_path.c_str());

	RequestHandler request{};
	curl_easy_setopt(this->_put_handle, CURLOPT_WRITEDATA, &request);

	request.req_status = curl_easy_perform(this->_put_handle);
	return request.req_json;
};

inline void RestSession::get_timeout(unsigned long interval) { curl_easy_setopt(this->_get_handle, CURLOPT_TIMEOUT, interval); };
inline void RestSession::post_timeout(unsigned long interval) { curl_easy_setopt(this->_post_handle, CURLOPT_TIMEOUT, interval); };
inline void RestSession::put_timeout(unsigned long interval) { curl_easy_setopt(this->_put_handle, CURLOPT_TIMEOUT, interval); };

bool RestSession::close()
{
	try
	{
		if (this->status)
		{
			curl_easy_cleanup(this->_post_handle);
			curl_easy_cleanup(this->_get_handle);
		}

		this->status = 0;
		return 1;
	}
	catch (...)
	{
		throw("bad_close_rest");
	}
};

RestSession::RequestHandler::RequestHandler()
	: req_raw{ "" }, req_json{ Json::Value{} }, req_status{ CURLcode{} }
{
	req_json["request_status"] = 0;
	req_json["response"] = Json::arrayValue;

};

RestSession::~RestSession()
{
	this->close();
	std::cout << "des called\n";
}
