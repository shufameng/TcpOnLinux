#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include <string>
#include <iostream>
#include <vector>

#include <mutex>
#include <thread>

class TcpServer
{   
public:
    enum Status {
        S_Started,
        S_Starting,
        S_Stopped,
        S_Stopping
    };

    TcpServer();
    virtual ~TcpServer();

    inline int fd() const
    { return mFd; }

    inline int connCount() const
    { return mConnCount; }

    void connCountIncrease(const int num = 1);
    void connCountDecrease(const int num = 1);

    inline std::string host() const
    { return mHost; }

    inline void setHost(const std::string host)
    { mHost = host; }

    inline ushort port() const
    { return mPort; }

    inline void setPort(const ushort port)
    { mPort = port; }

    int start();
    int stop();

    static std::vector<std::string> getLocalHosts();

    static void startAccept(TcpServer *server);
    static void startRecv(TcpServer *server, const int fd);

private:
    int         mFd;
    int         mConnCount;
    std::mutex  mConnCountMutex;

    Status      mStatus;

    std::string mHost;
    ushort      mPort;
};

#endif // TCPSERVER_H
