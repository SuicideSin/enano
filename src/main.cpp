#include <iostream>
#include <string>
#include "console.hpp"
#include "crypto.hpp"
#include "editor.hpp"
#include "file.hpp"

int main(int argc,char* argv[])
{
	editor_t editor;

	try
	{
		if(argc!=2)
			throw std::runtime_error("Usage: enano filename.txt");
		std::string filename(argv[1]);

		stdin_echo(false);
		std::string password;
		std::cout<<"Password: "<<std::flush;
		bool got_password=false;
		if(std::getline(std::cin,password))
			got_password=true;
		stdin_echo(true);
		std::cout<<std::endl;
		if(!got_password)
			throw std::runtime_error("Empty passwords are not allowed.");

		std::string cipher_text;
		if(!file_to_string(filename,cipher_text))
			throw std::runtime_error("Failed to read \""+filename+"\".");

		std::string salt_and_iv;
		if(cipher_text.size()==0)
			salt_and_iv=crypto_rand(16);
		else
		{
			salt_and_iv=cipher_text.substr(0,16);
			cipher_text=cipher_text.substr(16,cipher_text.size()-16);
		}

		password=pbkdf2(password,salt_and_iv,32,20);
		std::string plain_text;

		if(cipher_text.size()>0)
			plain_text=decrypt_aes256(cipher_text,password,salt_and_iv);

		editor.start(filename,plain_text,[&](const std::string& data)
		{
			cipher_text=encrypt_aes256(data,password,salt_and_iv);
			return string_to_file(salt_and_iv+cipher_text,filename);
		});
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