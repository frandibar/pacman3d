#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include "Thread.h"
#include "ServerSocket.h"

#include <list>
#include <string>
#include <queue>

// class for handling clients
class ClientHandler : public Thread
{
public:
    ClientHandler(int num, ServerSocket* ssock, Mutex* socketMutex) : _num(num), _pServerSocket(ssock) {
        _pNewMsgAlert = new ThreadCondition<bool>(false);
    }
    ~ClientHandler() {          // destructor
        delete _pServerSocket; 
        delete _pNewMsgAlert;
    }
    void run();
    void onStop();
    void receiveMsg(const std::string& msg) { 
        _msgMutex.lock();
        _pendingMessages.push(msg); 
        _pNewMsgAlert->set(true);
        _msgMutex.unlock();
    }
    bool hasQuit() { return _pServerSocket->isAlive(); }

private:
    int _num;                                 // client number
    ServerSocket* _pServerSocket;
    Mutex* _pSocketMutex;                     // mutex for _pServerSocket
    std::queue<std::string> _pendingMessages; // messages for clients
    Mutex _msgMutex;                          // mutex for _pendingMessages
    ThreadCondition<bool>* _pNewMsgAlert;     // condition when _pendingMessages has messages
};


class Server : public Thread
{
public:
    Server(int port) : _port(port) {}
    ~Server();
    void run();
    void onStop();
    void broadcast(const std::string& msg);
    unsigned int getNumberOfClients() const { return _clientThreads.size(); }

private:
    int _port;
    std::list<ClientHandler*> _clientThreads;
    Mutex _socketMutex;     // mutex for the socket
};

#endif
