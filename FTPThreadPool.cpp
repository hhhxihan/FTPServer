#include "FTPThreadPool.h"

FTPThreadPool::FTPThreadPool(int num){
    curNum=0;
    threadVector=vector<FTPThread>(num);
    maxNum=num;
    for(FTPThread& t:threadVector){
        t.Init();
    }
}

void FTPThreadPool::addTask(FTPTask* task){
    curNum=(curNum+1)%maxNum;
    threadVector[curNum].AddTask(task);
    
}