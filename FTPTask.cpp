#include "FTPTask.h"
#include <iostream>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;

void FTPTask::resPond(string msg){
    cout<<"FTPTask respond！"<<endl;
    if(!belongTask) cout<<"belongTask is null"<<endl;
    if(!belongTask->_bev) cout<<"bev is null"<<endl;
    bufferevent_write(belongTask->_bev,msg.c_str(),msg.size());
}


void FTPTask::ConnectDataPipe(){
    _bev=bufferevent_socket_new(base,-1,BEV_OPT_CLOSE_ON_FREE);
    sockaddr_in sin;
    sin.sin_family=AF_INET;
    sin.sin_port=htons(belongTask->transPort);
    sin.sin_addr.s_addr=inet_addr(belongTask->transIP.c_str());

    bufferevent_setcb(_bev,readCB,writeCB,eventCB,this);
    bufferevent_enable(_bev,EV_READ|EV_WRITE);

    bufferevent_socket_connect(_bev,(struct sockaddr*)&sin,sizeof(sin));
}
void FTPTask::getConnInfo(struct sockaddr* address){
    char ipString[INET_ADDRSTRLEN+1];
    struct sockaddr_in* addr_in = (struct sockaddr_in*)address;
    inet_ntop(AF_INET, &(addr_in->sin_addr), ipString, INET_ADDRSTRLEN);
    int port = ntohs(addr_in->sin_port);
    transIP=string(ipString);
}

void FTPTask::sendData(string msg){
    if(_bev!=NULL){
        bufferevent_write(_bev,msg.c_str(),msg.size());
    }
    
}

void FTPTask::readCB(struct bufferevent* bev,void* arg){
    FTPTask* t=static_cast<FTPTask*>(arg);
    if(!t) cout<<"task err"<<endl;
    t->read(bev);
}

void FTPTask::writeCB(struct bufferevent* bev,void* arg){
    FTPTask* t=static_cast<FTPTask*>(arg);
    t->write(bev);
}

void FTPTask::eventCB(struct bufferevent* bev,short event,void* arg){
    FTPTask* t=static_cast<FTPTask*>(arg);
    t->event(bev,event);
}

void FTPTask::Closefd(){
    if(!_bev){
        bufferevent_free(_bev);
    }
    if(!file){
        fclose(file);
    }
}
