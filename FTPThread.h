#ifndef FTPTHREAD_H
#define FTPTHREAD_H
#include "FTPTask.h"
#include <iostream>
using namespace std;

class FTPThread{
    public:
        void AddTask(FTPTask* task);
        void Main();
        void Init();
    private:
        event_base* base;
};

#endif