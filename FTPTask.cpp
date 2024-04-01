#include "FTPTask.h"
#include <iostream>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include "FTPserverCMD.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
using namespace std;
class FTPserverCMD;

void FTPTask::resPond(string msg){
    cout<<"FTPTask respond:"<<msg<<endl;
    if(!belongTask) cout<<"belongTask is null"<<endl;
    if(!belongTask->_bev) cout<<"bev is null"<<endl;
    bufferevent_write(belongTask->_bev,msg.c_str(),msg.size());
}
void FTPTask::resPondimmediately(string msg){
    cout<<"FTPTask respond:"<<msg<<endl;
    int sock=bufferevent_getfd(belongTask->_bev);
    send(sock,msg.c_str(),msg.size(),0);
}

 void FTPTask::setIP(struct sockaddr* address){
    	char ipString[INET_ADDRSTRLEN];
        struct sockaddr_in* addr_in = (struct sockaddr_in*)address;
        inet_ntop(AF_INET, &(addr_in->sin_addr), ipString, INET_ADDRSTRLEN);
        transIP=string(ipString);
        transPort= 20;
 }
void FTPTask::passConnCallback(struct evconnlistener* listener,int fd,sockaddr* addr,int socklen,void* arg){
    cout<<"new data connect"<<endl;
    FTPTask* t=reinterpret_cast<FTPTask*>(arg);
    t->_bev=bufferevent_socket_new(t->belongTask->base,fd,BEV_OPT_CLOSE_ON_FREE);
    if(t->_bev==nullptr) cout<<"FTPTask.cpp 36:_bev is null"<<endl;

    bufferevent_setcb(t->_bev,readCB,nullptr,eventCB,t);
    bufferevent_enable(t->_bev,EV_WRITE);

    FTPserverCMD* CMDTask=reinterpret_cast<FTPserverCMD*>(t->belongTask);
    //  调用处理函数
    CMDTask->pasvCMD();
    CMDTask->waitConn=0;
}
void FTPTask::pasvConnect(){
    belongTask->waitConn=1;
    if(!base) cout<<"base is null"<<endl;
    if(ev) {
        evconnlistener_free(ev); 
        ev=nullptr;
    }
    // _bev=bufferevent_socket_new(base,-1,BEV_OPT_CLOSE_ON_FREE);
    // if(!_bev) cout<<"bev create failed!"<<endl;
    sockaddr_in sin;
    sin.sin_family=AF_INET;
    sin.sin_port=htons(belongTask->transPort);
    sin.sin_addr.s_addr=INADDR_ANY;

    ev=evconnlistener_new_bind(base,passConnCallback,this, LEV_OPT_REUSEABLE,100,
                                            (struct sockaddr*)&sin,sizeof(sin));
    if(!ev) cout<<"ev create failed"<<endl;
    evconnlistener_enable(ev);
}
void FTPTask::ConnectDataPipe(){
    cout<<"active connect"<<endl;
    if(!base) cout<<"base is null"<<endl;
    _bev=bufferevent_socket_new(base,-1,BEV_OPT_CLOSE_ON_FREE);
    if(!_bev) cout<<"bev create failed!"<<endl;
    sockaddr_in sin;
    sin.sin_family=AF_INET;
    sin.sin_port=htons(belongTask->transPort);
    sin.sin_addr.s_addr=inet_addr(belongTask->transIP.c_str());
    cout<<"connect "<<belongTask->transIP<<":"<<belongTask->transPort<<endl;

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
    cout<<"FTPTask.cpp:87 send data:"+msg;
    if(_bev!=NULL){
        // int tfd=bufferevent_getfd(_bev);
        // send(tfd,msg.c_str(),msg.size(),0);
        bufferevent_write(_bev,msg.c_str(),msg.size());
        bufferevent_flush(_bev, EV_WRITE, BEV_FLUSH);
    }else{
        cout<<"FTPTask:94 _bev is NULL\n";
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
    cout<<"eventCB doing"<<endl;
    FTPTask* t=static_cast<FTPTask*>(arg);
    t->event(bev,event);
}

void FTPTask::Closefd(){
    if(_bev){
        bufferevent_free(_bev);
        _bev=nullptr;
    }
    if(file){
        fclose(file);
        file=nullptr;
    }
}
