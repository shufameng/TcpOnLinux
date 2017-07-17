#include "tcpserver.h"
#include "log.h"

TcpServer::TcpServer()
{
    mFd = -1;
    mConnCount = 0;
    mHost.clear();
    mPort = 8888;
}

TcpServer::~TcpServer()
{

}

void TcpServer::connCountIncrease(const int num)
{
    mConnCountMutex.lock();
    mConnCount += num;
    mConnCountMutex.unlock();
}

void TcpServer::connCountDecrease(const int num)
{
    mConnCountMutex.lock();
    mConnCount -= num;
    mConnCountMutex.unlock();
}

int TcpServer::start()
{
    char log[256];
    memset(&log, 0, sizeof(log));
    int rc = 0;
    mStatus = S_Starting;

    // socket()
    mFd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (-1 == mFd) {
        sprintf(log, "socket() error, %d:%s", errno, strerror(errno));
        mStatus = S_Stopped;
        return -1;
    }

    // bind()
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(mHost.data());
    addr.sin_port = htons(mPort);
    rc = ::bind(mFd, (struct sockaddr*)(&addr), sizeof(addr));
    if (-1 == rc) {
        sprintf(log, "bind() error, %d:%s", errno, strerror(errno));
        mStatus = S_Stopped;
        return -1;
    }

    // listen()
    rc = ::listen(mFd, SOMAXCONN);
    if (-1 == rc) {
        sprintf(log, "listen() error, %d:%s", errno, strerror(errno));
        mStatus = S_Stopped;
        return -1;
    }

    // accept()
    std::thread acceptThread(startAccept, this);
    acceptThread.detach();

    mStatus = S_Started;
    return 0;
}

int TcpServer::stop()
{
    return 0;
}

std::vector<std::__cxx11::string> TcpServer::getLocalHosts()
{
    char hostName[128];
    struct hostent *ht = NULL;
    std::vector<std::string> hosts;

    gethostname(hostName, sizeof(hostName));
    ht = gethostbyname(hostName);
    if (!ht)
        return hosts;

    struct in_addr addr;
    for (int i = 0; i < ht->h_length; i ++) {
        if (ht->h_addr_list[i]) {
            memcpy(&addr, ht->h_addr_list[i], sizeof(addr));
            hosts.push_back(std::string(inet_ntoa(addr)));
        }
    }

    return hosts;
}

void TcpServer::startAccept(TcpServer *server)
{
    int cliFd;
    while (1) {
        cliFd = ::accept(server->fd(), NULL, 0);
        if (-1 == cliFd) {
            continue;
        } else {
            server->connCountIncrease();
            std::thread recvThread(startRecv, server, cliFd);
            recvThread.detach();
        }

    }
}

void TcpServer::startRecv(TcpServer *server, const int fd)
{
    int bufLen = 1024;
    char *buf = new char[bufLen];
    int rc;

    while (1) {
        rc = ::recv(fd, buf, bufLen, 0);
        if (rc <= 0) {
            server->connCountDecrease();
            break;
        } else {
            Log::addLog(buf);
        }
    }

    delete []buf;
    buf = NULL;
}



