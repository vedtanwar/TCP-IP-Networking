#include "test_common.h"
#include "TcpClient.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>

TEST_F(LoggingFixture, ConnectToServerFailsOnInvalidIp)
{
    TcpClient client;
    EXPECT_FALSE(client.ConnectToServer("256.256.256.256", 12345));
}

TEST_F(LoggingFixture, SendMessageFailsBeforeConnect)
{
    TcpClient client;
    EXPECT_FALSE(client.sendMessage("test"));
}

TEST_F(LoggingFixture, ConnectToServerAndEchoWorks)
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    ASSERT_NE(server_fd, -1);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = 0;

    ASSERT_EQ(bind(server_fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)), 0);
    ASSERT_EQ(listen(server_fd, 1), 0);

    socklen_t len = sizeof(addr);
    ASSERT_EQ(getsockname(server_fd, reinterpret_cast<sockaddr *>(&addr), &len), 0);
    int port = ntohs(addr.sin_port);

    std::thread acceptThread([server_fd]() {
        sockaddr_in remote{};
        socklen_t remoteLen = sizeof(remote);
        int conn_fd = accept(server_fd, reinterpret_cast<sockaddr *>(&remote), &remoteLen);
        if (conn_fd == -1)
        {
            return;
        }

        char buffer[64] = {0};
        ssize_t readBytes = recv(conn_fd, buffer, sizeof(buffer), 0);
        if (readBytes > 0)
        {
            send(conn_fd, buffer, readBytes, 0);
        }

        close(conn_fd);
    });

    TcpClient client;
    EXPECT_TRUE(client.ConnectToServer("127.0.0.1", port));
    EXPECT_TRUE(client.sendMessage("ping"));
    EXPECT_TRUE(client.receiveMessage());

    close(server_fd);
    acceptThread.join();
}
