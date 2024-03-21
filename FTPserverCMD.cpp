#include "FTPserverCMD.h"
#define MAXSIZE 1024

void FTPserverCMD::registerCMD(string cmd,FTPTask* taskObj){
    if(TaskCMD.find(cmd)==TaskCMD.end()){
        taskObj->belongTask=this;
        TaskCMD[cmd]=taskObj;
    }
}

void FTPserverCMD::respWD(){

        FTPTask* t=TaskCMD["LIST"];
        t->processCMD("LIST","a");
}
void FTPserverCMD::readcmd(){
    //从一个缓冲事件中读取
    char data[MAXSIZE];

    int len=bufferevent_read(_bev,data,sizeof(data)-1);
    if(len<=0) ; //未读取到数据
    data[len]='\0';
    cout<<"the command is:"<<data<<endl;
    string Command="";
    for(char i:data){
        if(i==' '||i=='\r') break;
       Command+=i;
    } //读取出命令
    
    if(waitConn==1){
        storeCMD[0]=Command;
        storeCMD[1]=data;
        waitConn=0;
    }
    else if(TaskCMD.find(Command)!=TaskCMD.end()){
        FTPTask* t=TaskCMD[Command];
            t->transIP = transIP;
			t->transPort = transPort;
			t->base = base;
			t->belongTask=this; //所属的CMD对象
            t->processCMD(Command, data);
    }
    else{ //没有找到相应的命令时的处理方式
        resPond("202 command failed");
    }
    
}
void FTPserverCMD::pasvCMD(){
    cout<<"the pasv command is:"<<storeCMD[1]<<endl;
    if(TaskCMD.find(storeCMD[0])!=TaskCMD.end()){
        FTPTask* t=TaskCMD[storeCMD[0]];
            t->transIP = transIP;
			t->transPort = transPort;
			t->base = base;
			t->belongTask=this; //所属的CMD对象
            t->processCMD(storeCMD[0], storeCMD[1]);
    }
    else{ //没有找到相应的命令时的处理方式
        resPond("202 command failed");
    }
}

void FTPserverCMD::read(bufferevent* bev){
    readcmd();
}
void FTPserverCMD::resPond(string msg){  //这里不使用FTPTask中的resPond，因为那个是调用belongtask->bev；
    if(!_bev) {
        cout<<"bev is null"<<endl;
        bufferevent_write(_bev,msg.c_str(),msg.size());
    }
}