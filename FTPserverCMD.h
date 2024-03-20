#ifndef FTPSERVERCMD_H
#define FTPSERVERCMD_H
#include "FTPTask.h"
#include <map>
#include <string>
using namespace std;

class FTPserverCMD:public FTPTask{
    public:
        void registerCMD(string cmd,FTPTask* taskObj); //关联命令和命令处理对象
        void readcmd();  //读取命令
        void read(bufferevent* bev);
        void resPond(string msg);
        void respWD();
        void pasvCMD();

        map<string,FTPTask*> TaskCMD;  //映射字符串和相对的命令处理对象
};
#endif


