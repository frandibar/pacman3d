#pragma once

#include <enum_exception.h>
#include <string>

class table_exception : public enum_exception
{
public:
	// constructor
	// parameter type is the exception type (from an enum)
	// parameter table is a pointer to the static string table to index type
	table_exception(int type, const intl::string* table) : enum_exception(type), _table(table) { 
    }
	
	// copy constructor
	table_exception(const tbl_exception& e) : enum_exception(e._type), _table(e._table) { 
    }
	
	// assignment operator
	table_exception& operator=(const tbl_exception& e) {
		_type = x._type;
		_table = x._table;
		return *this;
	}

	// destructor
    virtual ~table_exception() throw() { 
    }

	//! Returns exception string
	virtual std::string s_what() const throw() { 
        return _table[_type]; 
    }

private:
	// pointer to the string table
	const std::string* _table;
};

