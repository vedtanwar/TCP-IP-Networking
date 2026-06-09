#include "test_common.h"
#include "ClientSession.h"

#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

TEST_F(LoggingFixture, GetFdReturnsConstructedFd)
{
    int fds[2];
    ASSERT_EQ(socketpair(AF_UNIX, SOCK_STREAM, 0, fds), 0);

    ClientSession session(fds[0]);
    EXPECT_EQ(session.getFd(), fds[0]);

    close(fds[1]);
}

TEST_F(LoggingFixture, ReceiveEchoesDataBackToPeer)
{
    int fds[2];
    ASSERT_EQ(socketpair(AF_UNIX, SOCK_STREAM, 0, fds), 0);

    int flags = fcntl(fds[0], F_GETFL, 0);
    ASSERT_NE(flags, -1);
    ASSERT_EQ(fcntl(fds[0], F_SETFL, flags | O_NONBLOCK), 0);

    ClientSession session(fds[0]);
    const char message[] = "hello world";
    ssize_t written = write(fds[1], message, sizeof(message) - 1);
    ASSERT_EQ(written, static_cast<ssize_t>(sizeof(message) - 1));

    EXPECT_TRUE(session.receive());

    char buffer[32] = {0};
    ssize_t readBytes = read(fds[1], buffer, sizeof(buffer));
    EXPECT_EQ(readBytes, static_cast<ssize_t>(sizeof(message) - 1));
    EXPECT_EQ(std::string(buffer, readBytes), std::string(message));

    close(fds[1]);
}

TEST_F(LoggingFixture, ReceiveReturnsFalseWhenPeerCloses)
{
    int fds[2];
    ASSERT_EQ(socketpair(AF_UNIX, SOCK_STREAM, 0, fds), 0);

    ClientSession session(fds[0]);
    close(fds[1]);

    EXPECT_FALSE(session.receive());
}
