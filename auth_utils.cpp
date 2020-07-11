#include <openssl/hmac.h>
#include <openssl/sha.h>


std::string gettime(void);
inline auto binary_to_hex_digit(unsigned a) -> char;
auto binary_to_hex(unsigned char const* binary, unsigned binary_len)->string;
std::string HMACsha256(std::string const& message, std::string const& key);


inline auto binary_to_hex_digit(unsigned a) -> char { // binary to hex for signature - digit
	return a + (a < 10 ? '0' : 'a' - 10);
}

auto binary_to_hex(unsigned char const* binary, unsigned binary_len) -> std::string {// binary to hex for signature
	std::string r(binary_len * 2, '\0');
	for (unsigned i = 0; i < binary_len; ++i) {
		r[i * 2] = binary_to_hex_digit(binary[i] >> 4);
		r[i * 2 + 1] = binary_to_hex_digit(binary[i] & 15);
	}
	return r;
}

std::string HMACsha256(string const& message, string const& key) // hmac signature
{
	unsigned char result[EVP_MAX_MD_SIZE];
	unsigned result_len = 0;
	HMAC(EVP_sha256(), key.data(), key.size(), reinterpret_cast<unsigned char const*>(message.data()), message.size(), result, &result_len);
	std::string signature = binary_to_hex(result, result_len);
	return signature;
}