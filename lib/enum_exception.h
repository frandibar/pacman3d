#pragma once

#include <exception>
#include <string>

class enum_exception : public exception
{
public:
	// constructor
	// parameter type is the exception type (from an enum)
	enum_exception(int type) : _type(type) {
    }
	
	// copy constructor
	enum_exception(const enum_exception& e) : _type(e._type) {
    }
	
	// assignment operator
	enum_exception& operator=(const enum_exception& e) {
		_type = e._type;
		return *this;
	}

	// destructor
    virtual ~enum_exception() throw() { 
    }

	int get_type() const { return _type; }

	virtual std::string s_what() const throw() = 0;

protected:
	int _type;      // exception type
};
