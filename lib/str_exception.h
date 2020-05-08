#pragma once

#include <exception>
#include <string>

class str_exception : public std::exception
{
public:
	str_exception(const intl::string& s) : _msg(s) 
    {}

	str_exception(const str_exception& s) : _msg(s._msg) 
    {}

	str_exception& operator=(const str_exception& s) { 
        _msg = s._msg; 
        return *this; 
    }

    virtual ~str_exception() throw() 
    {}

    virtual intl::string s_what() const throw()	{ 
        return _msg; 
    }

protected:
	// custom string representation for the exception
	intl::string _msg;

};
