#include "FTPserverCMD.h"
#define MAXSIZE 1024

void FTPserverCMD::registerCMD(string cmd,FTPTask* taskObj){
    if(TaskCMD.find(cmd)==TaskCMD.end()){
        TaskCMD[cmd]=taskObj;
    }
}

void FTPserverCMD::readcmd(struct bufferevent* bev){
    //从一个缓冲事件中读取
    char data[MAXSIZE];

    int len=bufferevent_read(bev,data,sizeof(data)-1);
    if(len<=0) ; //未读取到数据
    data[len]='\0';

    string Command="";
    for(char i:data){
        if(i==' '||i=='\r') break;
       Command+=i;
    } //读取出命令

    if(TaskCMD.find(Command)!=TaskCMD.end()){
        FTPTask* t=TaskCMD[Command];
            t->transIP = transIP;
			t->transPort = transPort;
			t->base = base;
			t->belongTask=this; //所属的CMD对象
			t->processCMD(Command, data);

    }
    else{ //没有找到相应的命令时的处理方式

    }

}