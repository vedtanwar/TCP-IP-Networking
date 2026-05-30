#include "ClientSession.h"
#include "log.h"
#include <sys/socket.h>
#include <utility>

ClientSession::ClientSession(int fd)
    : m_fd{fd}
{
    rx_buffer = std::make_unique<char[]>(MAX_RX_BUFFER_SIZE);
}

ClientSession::~ClientSession()
{

    if (m_fd != -1)
    {
        close(m_fd);
    }

    if (rx_buffer)
    {
        rx_buffer.reset();
    }
}

int ClientSession::getFd() const
{
    return m_fd;
}

bool ClientSession::receive()
{
    while (true)
    {
        char buffer[1024];

        ssize_t bytes = recv( m_fd, buffer, sizeof(buffer), 0);

        if (bytes > 0)
        {
            
            std::memcpy(rx_buffer.get(), buffer, bytes);
            Logger::info("Client {} : received :: {}", m_fd, rx_buffer.get());
            sendData(rx_buffer, bytes);
            continue;
        }

        if (bytes == 0)
        {
            Logger::warn( "Client {} disconnected", m_fd);

            return false;
        }

        if (errno == EAGAIN ||
            errno == EWOULDBLOCK)
        {
            break;
        }

        Logger::error(
            "Client {} recv error",
            m_fd);

        return false;
    }

    return true;
}

bool ClientSession::sendData(const S_CHAR_PTR &data, ssize_t len)
{
    ssize_t bytes = send(m_fd, data.get(), len, 0);

    if (bytes < 0)
    {
        Logger::error("Data send Failed for the client {}", m_fd);
        return false;
    }
    else
    {
        Logger::info("Data Send SuccessFully!!!");
    }

    return true;
}
