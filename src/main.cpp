#include"TcpServer.h"

int main()
{
    Logger::init();

    TcpServer server;

    if(!server.Initialize(13400))
    {
        return -1;
    }

    server.eventLoop();
}

