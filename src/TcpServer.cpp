#include "TcpServer.h"
#include <arpa/inet.h>
#include <sys/epoll.h>
#include<fcntl.h>

TcpServer::TcpServer()
{
}

TcpServer::~TcpServer()
{
}

bool TcpServer::Initialize(uint16_t port)
{

    // =============== Socket Creation ===============
    m_listenFd = socket(AF_INET, SOCK_STREAM, 0);

    if (m_listenFd == -1)
    {
        Logger::error("Socket Creation failed!!!");
        return false;
    }

    if (!SetNonBlocking(m_listenFd))
    {
        Logger::error("Failed to make listen socket non-blocking");
        return false;
    }

    // =============== Socket Configuration ================

    int opt = 1;
    if (setsockopt(m_listenFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        Logger::error("SetSocketOpt Failed for SO_REUSEADDR");
        return false;
    }

    if (setsockopt(m_listenFd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0)
    {
        Logger::error("SetSocketOpt Failed for SO_REUSEPORT");
        return false;
    }

    // =============== Address Configuration ================

    sockaddr_in addr{};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    // =============== Bind Socket ================

    if (bind(m_listenFd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0)
    {
        Logger::error("Socket Bind Failed!!!");
        return false;
    }

    // =============== Socket Listen ================

    if (listen(m_listenFd, SOMAXCONN) < 0)
    {
        Logger::error("Listen Failed!!");
        return false;
    }

    // =============== Epoll Cfg ================

    m_epoll_Fd = epoll_create1(0);

    if (m_epoll_Fd == -1)
    {
        Logger::error("Epoll Failed!!");
        return false;
    }

    epoll_event ev{};

    ev.events = EPOLLIN;
    ev.data.fd = m_listenFd;

    epoll_ctl(m_epoll_Fd, EPOLL_CTL_ADD, m_listenFd, &ev);

    Logger::info("TCP Server initialization completed!!");

    return true;
}

/*
    Function: HandleNewConnection
*/

void TcpServer::HandleNewConnection()
{

    while (true)
    {
        int clientFd = accept(m_listenFd, nullptr, nullptr);

        if (clientFd == -1)
        {
            if (errno == EAGAIN ||
                errno == EWOULDBLOCK)
            {
                break;
            }

            Logger::error("accept() failed : {}", strerror(errno));

            return;
        }

        if (!SetNonBlocking(clientFd))
        {
            Logger::error("Failed to make client {} non-blocking", clientFd);

            close(clientFd);

            return;
        }

        epoll_event ev{};

        ev.events = EPOLLIN;

        ev.data.fd = clientFd;

        if (epoll_ctl(m_epoll_Fd, EPOLL_CTL_ADD, clientFd, &ev) == -1)
        {
            Logger::error("epoll_ctl failed for client {} : {}", clientFd, strerror(errno));
            close(clientFd);
            continue;
        }

        auto session = std::make_unique<ClientSession>(clientFd);

        m_clients.emplace(clientFd, std::move(session));

        Logger::info( "Client connected ✅ fd={}", clientFd);
    }
}

void TcpServer::HandleClientEvents(int clientFd)
{

    auto it = m_clients.find(clientFd);

    if (it == m_clients.end())
    {
        return;
    }

    if (!it->second->receive())
    {
        RemoveClient(clientFd);
    }
}

void TcpServer::RemoveClient(int clientFd)
{
    epoll_ctl(m_epoll_Fd, EPOLL_CTL_DEL, clientFd, nullptr);

    m_clients.erase(clientFd);

    Logger::warn("Client Disconnected : {}", clientFd);
}

void TcpServer::eventLoop()
{
    epoll_event events[64];

    while (true)
    {
        int ready = epoll_wait(m_epoll_Fd, events, 64, 100);

        if (ready == -1)
        {
            Logger::error("************Epoll Wait error ************** ");
            continue;
        }

        for (int i = 0; i < ready; i++)
        {
            int fd = events[i].data.fd;

            if (fd == m_listenFd)
            {
                HandleNewConnection();
            }
            else
            {
                HandleClientEvents(fd);
            }
        }
    }
}

bool TcpServer::SetNonBlocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);

    if (flags == -1)
    {
        return false;
    }

    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        return false;
    }

    return true;
}
