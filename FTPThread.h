#pragma once
#include "FTPTask.h"

class FTPThread{
    public:
        void AddTask(FTPTask* task);
        void Main();
        void Init();
    private:
        event_base* base=0;
};