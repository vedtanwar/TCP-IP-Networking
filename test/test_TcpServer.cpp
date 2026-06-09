#include "test_common.h"
#include "TcpServer.h"

#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

TEST_F(LoggingFixture, SetNonBlockingReturnsTrueForSocket)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    ASSERT_NE(fd, -1);

    TcpServer server;
    EXPECT_TRUE(server.SetNonBlocking(fd));

    int flags = fcntl(fd, F_GETFL, 0);
    ASSERT_NE(flags, -1);
    EXPECT_NE(flags & O_NONBLOCK, 0);

    close(fd);
}

TEST_F(LoggingFixture, InitializeSucceedsOnEphemeralPort)
{
    TcpServer server;
    EXPECT_TRUE(server.Initialize(0));
}
