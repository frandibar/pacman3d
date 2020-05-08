
#include "ClientSocket.h"
#include "SocketException.h"
#include "helpers.h"

#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

int main(int argc, char** argv)
{
    const int ERR_INVALID_ARGS = 1;
    // validate arguments
    if (argc != 3) {
        cout << "uso: " << argv[0] << " IP puerto" << endl;
        return ERR_INVALID_ARGS;
    }

    try {
        ClientSocket client_socket(argv[1], fromString<int>(argv[2]));
        string reply;

        while (true) {  
            client_socket >> reply;
            cout << reply;
        }
    }
    catch (SocketException& e) {
    }

    return 0;
}
