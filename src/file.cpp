#include "file.hpp"
#include <fstream>
#include <iostream>

#if(defined(_WIN32)&&!defined(__CYGWIN__))
	#include <windows.h>
#else
	#include <sys/stat.h>
	#include <unistd.h>
#endif

bool file_to_string(const std::string& filename,std::string& data)
{
	char buffer;
	std::ifstream istr(filename.c_str(),std::ios_base::in|std::ios_base::binary);
	istr.unsetf(std::ios_base::skipws);

	if(!istr)
		return false;

	data="";

	while(true)
	{
		if(!(istr>>buffer))
		{
			if(istr.eof())
				break;
			return false;
		}

		data+=buffer;
	}

	istr.close();
	return true;
}

bool string_to_file(const std::string& data,const std::string& filename)
{
	bool saved=false;
	std::ofstream ostr(filename.c_str(),std::ios_base::out|std::ios_base::binary);
	saved=(bool)(ostr<<data);
	ostr.close();
	return saved;
}

bool exists(const std::string& filename)
{
	#if(defined(_WIN32)&&!defined(__CYGWIN__))
	return (_access(filename.c_str(),0)!=-1);
	#else
		return (access(filename.c_str(),F_OK)!=-1);
	#endif
}