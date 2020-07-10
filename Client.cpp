class Client
{
private:
	const char* api_key;
	const char* api_secret;

public:
	Client();
	Client(const char* key, const char* secret);
};
