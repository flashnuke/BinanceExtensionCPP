#ifndef CLIENT_EXCEPTIONS_H
#define CLIENT_EXCEPTIONS_H

class ClientException
{
	std::string error_desc;
	std::vector<std::string> traceback;
	std::string final_error_body;

public:
	explicit ClientException(std::string error_reason);
	void append_to_traceback(const std::string& loc);
	void append_to_traceback(std::string&& loc);

	const char* what(); // returns body
};

// REST Client Exceptions

class BadSetupSessionREST : public ClientException // for bad REST requests
{
public:
	BadSetupSessionREST();
};

class BadRequestREST : public ClientException // for trying methods where auth is needed but keys are missing
{
public:
	BadRequestREST();
};

class BadCleanupREST : public ClientException
{
public:
	BadCleanupREST();
};

class BadSetupHeadersREST : public ClientException
{
public:
	BadSetupHeadersREST();
};

// WS Client Exceptions

class BadStreamOpenWS : public ClientException
{
public:
	BadStreamOpenWS();
};

class BadStreamCloseWS : public ClientException
{
public:
	BadStreamCloseWS();
};

class BadStreamCallbackWS : public ClientException
{
public:
	BadStreamCallbackWS();
};

class BadSetupPathWS : public ClientException
{
public:
	BadSetupPathWS();
};

// Exchange Client Exceptions

class BadQuery : public ClientException
{
public:
	BadQuery();
};

class MissingCredentials : public ClientException
{
public:
	MissingCredentials();
};

class MissingEndpoint : public ClientException
{
public:
	MissingEndpoint();
};

#endif
