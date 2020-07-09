#ifndef curl
#include <curl/curl.h>
#endif
#include <iostream>

long _IDLE_TIME_TCP = 120L;
long _INTVL_TIME_TCP = 60L;

class RestSession
{
private:
	CURL* session;

public:
	RestSession();
};

RestSession::RestSession()
{
	std::cout << "init";
	session = curl_easy_init();
	curl_easy_setopt(session, CURLOPT_TCP_KEEPALIVE, 1L);
	curl_easy_setopt(session, CURLOPT_TCP_KEEPIDLE, _IDLE_TIME_TCP);
	curl_easy_setopt(session, CURLOPT_TCP_KEEPINTVL, _INTVL_TIME_TCP);

}
