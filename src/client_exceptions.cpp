#include "../include/Binance_Client.h"


//  ------------------------------ Base Exception Class

ClientException::ClientException(std::string error_text)
	: error_desc{ error_text }
{};

void ClientException::append_to_traceback(const std::string& loc)
{
	this->traceback.push_back(loc);
}

void ClientException::append_to_traceback(std::string&& loc)
{
	this->traceback.push_back(std::move(loc));
}


const char* ClientException::what()
{
	final_error_body = "exception: " + this->error_desc;
	for (std::string func : traceback)
	{
		final_error_body += "\n" + func;
	}

	return final_error_body.c_str();
}


//  ------------------------------ Rest Client Exceptions


BadSetupSessionREST::BadSetupSessionREST()
	: ClientException("bad_rest_client_init")
{};

BadRequestREST::BadRequestREST()
	: ClientException("missing_credentials_for_this_method_or_class")
{};

BadCleanupREST::BadCleanupREST()
	: ClientException("missing_credentials_for_this_method_or_class")
{};

BadSetupHeadersREST::BadSetupHeadersREST()
	: ClientException("missing_credentials_for_this_method_or_class")
{};

//  ------------------------------ WS Client Exceptions

BadStreamOpenWS::BadStreamOpenWS()
	: ClientException("missing_credentials_for_this_method_or_class")
{};

BadStreamCloseWS::BadStreamCloseWS()
	: ClientException("missing_credentials_for_this_method_or_class")
{};

BadStreamCallbackWS::BadStreamCallbackWS()
	: ClientException("bad_stream_callback")
{};

BadSetupPathWS::BadSetupPathWS()
	: ClientException("missing_credentials_for_this_method_or_class")
{};

//  ------------------------------ WS Client Exceptions

BadQuery::BadQuery()
	: ClientException("missing_credentials_for_this_method_or_class")
{};

MissingCredentials::MissingCredentials()
	: ClientException("missing_credentials_for_this_method_or_class")
{};

MissingEndpoint::MissingEndpoint()
	: ClientException("missing_credentials_for_this_method_or_class")
{};

