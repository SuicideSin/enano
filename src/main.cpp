#include <iostream>
#include <string>
#include "console.hpp"
#include "crypto.hpp"
#include "editor.hpp"
#include "file.hpp"

std::string get_password(const std::string& prompt)
{
	stdin_echo(false);
	std::string password;
	std::cout<<prompt<<std::flush;
	bool got_password=false;
	if(std::getline(std::cin,password)&&password.size()>0)
		got_password=true;
	stdin_echo(true);
	std::cout<<std::endl;
	if(!got_password)
		throw std::runtime_error("Empty passwords are not allowed.");
	return password;
}

int main(int argc,char* argv[])
{
	editor_t editor;

	try
	{
		if(argc!=2)
			throw std::runtime_error("Usage: enano filename.txt");

		std::string filename(argv[1]);
		std::string password;
		std::string salt;
		std::string iv;
		std::string plain_text;
		std::string cipher_text;

		if(!file_to_string(filename,cipher_text)&&exists(filename))
			throw std::runtime_error("Failed to read \""+filename+"\".");

		if(cipher_text.size()==0)
		{
			salt=crypto_rand(16);
			iv=crypto_rand(16);
		}
		else
		{
			salt=cipher_text.substr(0,16);
			iv=cipher_text.substr(16,16);
			cipher_text=cipher_text.substr(32,cipher_text.size()-32);
		}

		while(true)
		{
			try
			{
				password=get_password("Enter password: ");
				if(!exists(filename))
				{
					std::string password_verify=get_password("Re-enter password: ");
					bool matches=(password==password_verify);
					for(auto& ii:password_verify)
						ii=' ';
					if(!matches)
						throw std::runtime_error("Passwords do not match");
				}
				if(cipher_text.size()>0)
					plain_text=decrypt_aes256(cipher_text,pbkdf2(password,salt,32,20),iv);
				break;
			}
			catch(std::exception& error)
			{
				std::cerr<<error.what()<<std::endl;
			}
		}

		editor.start(filename,plain_text,[&](const std::string& data)
		{
			salt=crypto_rand(16);
			iv=crypto_rand(16);
			cipher_text=encrypt_aes256(data,pbkdf2(password,salt,32,20),iv);
			return string_to_file(salt+iv+cipher_text,filename);
		});
		for(auto& ii:plain_text)
			ii=' ';
		for(auto& ii:password)
			ii=' ';
		for(auto& ii:salt)
			ii=' ';
		for(auto& ii:iv)
			ii=' ';
		for(auto& ii:cipher_text)
			ii=' ';
	}
	catch(std::exception& error)
	{
		editor.stop();
		std::cerr<<error.what()<<std::endl;
		return 1;
	}
	catch(...)
	{
		editor.stop();
		std::cerr<<"Unknown error occured."<<std::endl;
		return 1;
	}

	return 0;
}