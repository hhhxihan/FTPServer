#include "FTPserverCMD.h"

void FTPserverCMD::registerCMD(string cmd,FTPTask* taskObj){
    if(TaskCMD.find(cmd)==TaskCMD.end()){
        TaskCMD[cmd]=taskObj;
    }
}

void FTPserverCMD::readcmd(struct bufferevent* bev){
    //从一个缓冲事件中读取
}