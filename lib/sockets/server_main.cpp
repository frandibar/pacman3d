#include "Server.h"
#include "RailwaySimulator.h"
#include "helpers.h"
#include "Callback.h"

#include <iostream>
#include <string>

using std::cout;
using std::endl;


// this class is used as a callback to process events from the simulator
class ShowStatus: public Callback
{
public:
    ShowStatus(Server* ps) : _pServer(ps) {}
    void call(void* vp) {
        string msg = static_cast<char*>(vp);
        DEBUGME<string>(msg, "broadcasting: ");
        _pServer->broadcast(msg);
    }

private:
    Server* _pServer;
};


int main(int argc, char** argv)
{
    const int ERR_INVALID_ARGS = 1;
    //const int ERR_OTHER = 2;

    // validate arguments
    if (argc != 4) {
        cout << "uso: " << argv[0] << " archivo_de_red archivo_de_trenes puerto" << endl;
        return ERR_INVALID_ARGS;
    }

    Server server(fromString<int>(argv[3]));
    ShowStatus st(&server);
    RailwaySimulator sim(argv[1], argv[2], &st);

    // start threads
    server.start();
    sim.start();

    // wait till threads finish
    sim.join();
    DEBUGME<string>("simulation finished, stopping server...");
    server.stop(); // simulation finished
    server.join();
    return 0;
}
