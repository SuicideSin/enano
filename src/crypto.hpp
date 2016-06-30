//Windows Dependencies:
//		-lcrypto -lgdi32

//Linux Dependencies:
//		-lcrypto

#ifndef CRYPTO_HPP
#define CRYPTO_HPP

#include <string>

std::string encrypt_aes256(const std::string& plain,const std::string& key,const std::string& iv);
std::string decrypt_aes256(const std::string& cipher,const std::string& key,const std::string& iv);
std::string pbkdf2(const std::string& password,const std::string& salt,
	const size_t key_byte_size,const size_t iterations);

#endif