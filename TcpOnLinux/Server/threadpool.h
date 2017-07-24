#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <stdio.h>
#include <pthread.h>
#include <vector>


class ThreadPool
{
public:
    ThreadPool(const int size);
    virtual ~ThreadPool();


protected:
    static void doneWithFd(const int fd);
    static void *thread_proc(void *arg);

private:
    std::vector<int> mTaskFds;
    pthread_mutex_t mMut;
    pthread_cond_t  mCond;

    std::vector<pthread_t> mBusyThread;
    std::vector<pthread_t> mBlockThread;
};

#endif // THREADPOOL_H
