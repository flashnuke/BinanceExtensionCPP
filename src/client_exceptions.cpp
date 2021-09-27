#include "../include/Binance_Client.h"

//  ------------------------------ Base Exception Class

ClientException::ClientException(std::string error_text)
	: error_desc{ error_text }
{};

/**
	returns a description of the exception
*/
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
	: ClientException("bad_rest_client_request")
{};

BadCleanupREST::BadCleanupREST()
	: ClientException("bad_rest_client_cleanup")
{};

BadSetupHeadersREST::BadSetupHeadersREST()
	: ClientException("bad_rest_client_headers_setup")
{};

//  ------------------------------ WS Client Exceptions

BadStreamOpenWS::BadStreamOpenWS()
	: ClientException("bad_ws_client_stream_open")
{};

BadStreamCloseWS::BadStreamCloseWS()
	: ClientException("bad_ws_client_stream_close")
{};

BadStreamCallbackWS::BadStreamCallbackWS()
	: ClientException("bad_ws_client_stream_callback")
{};

BadSetupPathWS::BadSetupPathWS()
	: ClientException("bad_ws_client_stream_path_setup")
{};

//  ------------------------------ WS Client Exceptions

BadQuery::BadQuery()
	: ClientException("bad_exchange_client_request_query")
{};

MissingCredentials::MissingCredentials()
	: ClientException("missing_exchange_client_credentials_for_method")
{};

MissingEndpoint::MissingEndpoint()
	: ClientException("missing_exchange_client_method_endpoint")
{};

//  ------------------------------ Custom Client Exception

CustomException::CustomException(const std::string error_msg)
	: ClientException(error_msg)
{};
