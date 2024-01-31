#include "FTPTask.h"
#include <psdk_inc/_ip_types.h>
#include <winsock2.h>



void FTPTask::resPond(string msg){
    bufferevent_write(belongTask->bev,msg.c_str(),msg.size());
}

void FTPTask::ConnectDataPipe(){
    bev=bufferevent_socket_new(base,-1,BEV_OPT_CLOSE_ON_FREE);
    sockaddr_in sin;
    sin.sin_family=AF_INET;
    sin.sin_port=htons(belongTask->transPort);
    sin.sin_addr.s_addr=inet_addr(belongTask->transIP.c_str());

    bufferevent_setcb(base,readCB,writeCB,eventCB,this);
    bufferevent_enable(bev,EV_READ|EV_WRITE);

    bufferevent_socket_connect(bev,(struct sockaddr*)&sin,sizeof(sin));
}

void FTPTask::sendData(string msg){
    if(bev!=NULL){
        bufferevent_write(bev,msg.c_str(),msg.size());
    }
    
}

void FTPTask::readCB(struct bufferevent* bev,void* arg){
    FTPTask* t=static_cast<FTPTask*>(arg);
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
    if(!bev){
        bufferevent_free(bev);
    }
    if(!file){
        fclose(file);
    }
}
