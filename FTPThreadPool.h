#ifndef FTPTHREADPOOL_H
#define FTPTHREADPOOL_H

#include <thread>
#include <vector>
#include "FTPThread.h"
#include "FTPTask.h"
using namespace std;

class FTPThreadPool{
    public:
        int curNum; //当前线程的下标
        vector<FTPThread> threadVector;

        FTPThreadPool(int num=10);
        void addTask(FTPTask* task);
    private:
        int maxNum; //最大线程数  
};

#endif
