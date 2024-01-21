#include "FTPserverCMD.h"

void FTPserverCMD::registerCMD(string cmd,FTPTask* taskObj){
    if(TaskCMD.find(cmd)==TaskCMD.end()){
        TaskCMD[cmd]=taskObj;
    }
}