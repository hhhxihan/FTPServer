#include "FTPTask.h"

int FTPTask::Init(struct event_base* tbase){  
    this->base=tbase;
    struct bufferevent* bev=bufferevent_socket_new(base,socketID,BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb(bev,readCB,writeCB,eventCB,this);
    bufferevent_enable(bev,EV_READ|EV_WRITE);
}

void FTPTask::readCB(struct bufferevent* bev,void* arg){
    FTPTask* t=static_cast<FTPTask*>(arg);
    t->readCMD(bev);
}

void FTPTask::writeCB(struct bufferevent* bev,void* arg){
    
}