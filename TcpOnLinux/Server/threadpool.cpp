#include "threadpool.h"
#include "tcpserver.h"

ThreadPool::ThreadPool(const int size)
{
    pthread_mutex_init(&mMut, NULL);
    pthread_cond_init(&mCond, NULL);

    pthread_t tId;
    for (int i = 0; i < size; ++ i) {
        pthread_create(&tId, NULL, thread_proc, this);
        mBlockThread.push_back(tId);
    }
}

ThreadPool::~ThreadPool()
{

}

void ThreadPool::doneWithFd(const int fd)
{

}

void *ThreadPool::thread_proc(void *arg)
{
    ThreadPool *p = static_cast<ThreadPool*>(arg);
    if (!p)
        return;

    while (1) {
        pthread_mutex_lock(&p->mMut);
        while (p->mTaskFds.size() <= 0) {
            pthread_cond_wait(&p->mCond, &p->mMut);
        }

        std::vector<int>::iterator iter = p->mTaskFds.begin();
        int fd;
        if (iter != p->mTaskFds.end()) {
            fd = *iter;
            p->mTaskFds.erase(iter);
        }

        pthread_mutex_unlock(&p->mMut);

        // done with this fd
        doneWithFd(fd);
    }

    return NULL;
}
