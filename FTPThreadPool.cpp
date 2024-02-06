#include "FTPThreadPool.h"

FTPThreadPool::FTPThreadPool(int num){
    threadVector=vector<FTPThread>(num);
    for(FTPThread& t:threadVector){
        t.Init();
    }
}

void FTPThreadPool::addTask(FTPTask* task){
    curNum=(curNum+1)%maxNum;
    threadVector[curNum].AddTask(task);
}