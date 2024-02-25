#include "CMDSTOR.h"
#include <filesystem>
#include <fstream>
#define MAXSIZE 100

void CMDSTOR::processCMD(string cmd,string msg){
    

    int pos=msg.find(" ");
    msg=msg.substr(pos+1,msg.size()-pos-3); //去掉末尾的\r\n
    std::filesystem::path _path(msg.c_str());

    string _p=_path.string();

    if(!_p.empty()){ //有父目录
        if(belongTask->currentDir.at(belongTask->currentDir.size()-1)!='/'){
            belongTask->currentDir+="/";
        }
        if(_p.at(0)=='/'){  //绝对路径
            belongTask->currentDir=_p;
        }
        else if(_p.at(0)=='.'){  // ./xxx类型相对路径
            _p=_p.substr(2,_p.size()-2);
            belongTask->currentDir+=_p;
        }
        else{
            belongTask->currentDir+=_p;
        }
    }

    std::filesystem::path filePath(belongTask->currentDir);
    outFile=std::ofstream(filePath);

    bufferevent_disable(belongTask->_bev,EV_READ); //关闭主事件的监听
    evutil_socket_t sockfd=bufferevent_getfd(belongTask->_bev);
    char buf[MAXSIZE]="220 able to accept file";
    send(sockfd,buf,sizeof(buf),0);

    ConnectDataPipe();
    bufferevent_disable(_bev,EV_READ);
    evutil_socket_t datafd=bufferevent_getfd(_bev);
    recv(sockfd,buf,sizeof(buf),0);
    int len=std::stoi(buf);
    int recvSize=0;
    while(recvSize<len){
        recvSize+=recv(datafd,buf,sizeof(buf),0);
        outFile<<buf;
    }

    Closefd();
    bufferevent_enable(belongTask->_bev,EV_READ);
    // bufferevent_enable(_bev,EV_READ);
}

void CMDSTOR::read(struct bufferevent* bev){
    char buf[MAXSIZE];
    if(outFile.is_open()){
        while(bufferevent_read(bev,buf,MAXSIZE-1)!=0){
            outFile<<buf;
        }
    }
    
}

void CMDSTOR::event(struct bufferevent* bev,short _event){
    if(_event&(BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT)){
        Closefd();
        resPond("msg");
    }
}

void CMDSTOR::Closefd(){
    if(!_bev){
        bufferevent_free(_bev);
    }
    if(outFile.is_open()){
        outFile.close();
    }
}