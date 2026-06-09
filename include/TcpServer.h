#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "ClientSession.h"
#include "log.h"

#include <memory>
#include <unordered_map>
#include <unistd.h>

using S_CLIENT_PTR = std::unique_ptr<ClientSession>;


class TcpServer
{
private:
    /* data */

    int m_listenFd = -1;
    int m_epoll_Fd = -1;

    std::unordered_map<int, S_CLIENT_PTR> m_clients;   
    
public:
    TcpServer();
    ~TcpServer();

    bool Initialize(uint16_t port);

    void HandleNewConnection();

    void HandleClientEvents(int clientFd);

    void RemoveClient(int clientFd);

    void eventLoop();

    bool SetNonBlocking(int fd);
};

#endif // TCPSERVER_H
