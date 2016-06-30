//Windows Dependencies:
//		-lcrypto -lgdi32

//Linux Dependencies:
//		-lcrypto

#include "crypto.hpp"

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/md5.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>
#include <cstdint>

#define AES_BLOCK_SIZE					16
#define AES256_KEY_SIZE					32

class crypto_initialize_t
{
	public:
		crypto_initialize_t()
		{
			ERR_load_crypto_strings();
			OpenSSL_add_all_algorithms();
		}

		crypto_initialize_t(const crypto_initialize_t& copy)=delete;

		~crypto_initialize_t()
		{
			ERR_free_strings();
			EVP_cleanup();
		}

		crypto_initialize_t& operator=(const crypto_initialize_t& copy)=delete;
};

static crypto_initialize_t crypto_initializer;

static void aes_cleanup(EVP_CIPHER_CTX* ctx)
{
	EVP_CIPHER_CTX_free(ctx);
	ERR_remove_state(0);
}

std::string encrypt_aes256(const std::string& plain,const std::string& key,const std::string& iv)
{
	std::string cipher;
	cipher.resize((plain.size()/AES_BLOCK_SIZE+1)*AES_BLOCK_SIZE);
	EVP_CIPHER_CTX* ctx=nullptr;
	try
	{
		ctx=EVP_CIPHER_CTX_new();
		if(key.size()!=AES256_KEY_SIZE)
			throw std::runtime_error("encrypt_aes256() - Given key size is invalid ("+
				std::to_string(AES256_KEY_SIZE)+"bytes ).");
		int temp_length;
		int temp_unaligned_length;
		if(ctx==nullptr)
			throw std::runtime_error("encrypt_aes256() - Creating a EVP_CIPHER_CTX failed.");
		if(EVP_CIPHER_CTX_set_padding(ctx,1)==0)
			throw std::runtime_error("encrypt_aes256() - EVP_CIPHER_CTX_set_padding failed.");
		if(EVP_EncryptInit(ctx,EVP_aes_256_cbc(),(uint8_t*)key.data(),(uint8_t*)iv.data())==0)
			throw std::runtime_error("encrypt_aes256() - EVP_EncryptInit failed.");
		if(EVP_EncryptUpdate(ctx,(uint8_t*)cipher.data(),&temp_length,(uint8_t*)plain.data(),plain.size())==0)
			throw std::runtime_error("encrypt_aes256() - EVP_EncryptUpdate failed.");
		if(EVP_EncryptFinal(ctx,(uint8_t*)cipher.data()+temp_length,&temp_unaligned_length)==0)
			throw std::runtime_error("encrypt_aes256() - EVP_EncryptFinal failed.");
		cipher.resize(temp_length+temp_unaligned_length);
	}
	catch(...)
	{
		aes_cleanup(ctx);
		throw;
	}
	aes_cleanup(ctx);
	return cipher;
}

std::string decrypt_aes256(const std::string& cipher,const std::string& key,const std::string& iv)
{
	std::string plain;
	plain.resize((cipher.size()/AES_BLOCK_SIZE+1)*AES_BLOCK_SIZE);
	EVP_CIPHER_CTX* ctx=nullptr;
	try
	{
		std::string error_str="Decryption failed.";
		ctx=EVP_CIPHER_CTX_new();
		if(key.size()!=AES256_KEY_SIZE)
			throw std::runtime_error(error_str);
		int temp_length;
		int temp_unaligned_length;
		if(ctx==nullptr)
			throw std::runtime_error(error_str);
		if(EVP_CIPHER_CTX_set_padding(ctx,1)==0)
			throw std::runtime_error(error_str);
		if(EVP_DecryptInit(ctx,EVP_aes_256_cbc(),(uint8_t*)key.data(),(uint8_t*)iv.data())==0)
			throw std::runtime_error(error_str);
		if(EVP_DecryptUpdate(ctx,(uint8_t*)plain.data(),&temp_length,(uint8_t*)cipher.data(),cipher.size())==0)
			throw std::runtime_error(error_str);
		if(EVP_DecryptFinal(ctx,(uint8_t*)plain.data()+temp_length,&temp_unaligned_length)==0)
			throw std::runtime_error(error_str);
		plain.resize(temp_length+temp_unaligned_length);
	}
	catch(...)
	{
		aes_cleanup(ctx);
		throw;
	}
	aes_cleanup(ctx);
	return plain;
}

std::string pbkdf2(const std::string& password,const std::string& salt,const size_t key_byte_size,
	const size_t iterations)
{
	std::string key;
	key.resize(key_byte_size);
	if(PKCS5_PBKDF2_HMAC_SHA1(password.data(),password.size(),
		(uint8_t*)salt.data(),salt.size(),iterations,key_byte_size,(uint8_t*)key.data())==0)
		throw std::runtime_error("pbkdf2() - PKCS5_PBKDF2_HMAC_SHA1 failed.");
	return key;
}

std::string crypto_rand(const size_t size)
{
	std::string bytes;
	bytes.resize(size);
	if(RAND_bytes((uint8_t*)bytes.c_str(),size)!=1)
		throw std::runtime_error("crypto_rand() - Error getting cryptographically strong random numbers.");
	return bytes;
}