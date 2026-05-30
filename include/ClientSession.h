#ifndef CLIENTSESSION_H
#define CLIENTSESSION_H

#define MAX_RX_BUFFER_SIZE 1024

#include <string>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include<memory>

using S_CHAR_PTR = std::unique_ptr<char[]>;

class ClientSession
{
private:
    /* data */

    int m_fd;
    S_CHAR_PTR rx_buffer;
public:
    explicit ClientSession(int fd);
    ~ClientSession();

    int getFd() const;

    bool receive();

    bool sendData(const S_CHAR_PTR &data, ssize_t len);
};

#endif // CLIENTSESSION_H
