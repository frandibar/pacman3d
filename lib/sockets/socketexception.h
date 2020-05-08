#include <table_exception.h>
#include <string>

class SocketException : public table_exception
{
public:
    enum type { SOCKET_ERROR };
    SocketException(int type);
    virtual ~SocketException();
    std::string s_what() {

private:
    static std::string[] _table; 
};
