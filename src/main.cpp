#include <iostream>
#include <string>
#include "console.hpp"
#include "crypto.hpp"
#include "editor.hpp"
#include "file.hpp"

int main(int argc,char* argv[])
{
	try
	{
		if(argc!=2)
			throw std::runtime_error("Usage: enano filename.txt");
		std::string filename(argv[1]);

		/*stdin_echo(false);
		std::string password;
		std::cout<<"Password: "<<std::flush;
		bool got_password=false;
		if(std::getline(std::cin,password))
			got_password=true;
		stdin_echo(true);
		std::cout<<std::endl;*/
		std::string password="hello";
		bool got_password=true;
		if(!got_password)
			throw std::runtime_error("Empty passwords are not allowed.");

		std::string cipher_text;
		if(!file_to_string(filename,cipher_text))
			throw std::runtime_error("Failed to read \""+filename+"\".");

		password=pbkdf2(password,"",32,20);
		std::string plain_text;

		if(cipher_text.size()>0)
			plain_text=decrypt_aes256(cipher_text,password,"01234567890123456");

		editor_t editor;
		editor.start(filename,"hello\n123\ntest\nworld\na\nb\nc\nd\ne\nf\ng");

		/*std::cout<<"Old Data: "<<plain_text<<std::endl;
		std::cout<<"New Data: "<<std::flush;
		std::getline(std::cin,plain_text);
		cipher_text=encrypt_aes256(plain_text,password,"01234567890123456");
		string_to_file(cipher_text,filename);*/
	}
	catch(std::exception& error)
	{
		std::cerr<<error.what()<<std::endl;
		return 1;
	}
	catch(...)
	{
		std::cerr<<"Unknown error occured."<<std::endl;
		return 1;
	}

	return 0;
}