#ifndef FTPTHREAD_H
#define FTPTHREAD_H
#include "FTPTask.h"
#include <unistd.h>
#include <queue>
#include <cstring>
#include <iostream>
#include <mutex>
using namespace std;

class FTPThread{
    public:
        void AddTask(FTPTask* task);
        void Main();
        void Init();
        static void callback(evutil_socket_t fd,short event,void* arg);
    private:
        event_base* base;
        int pipefd[2];
        queue<FTPTask*> taskQueue;
        mutex mux;
};

#endif