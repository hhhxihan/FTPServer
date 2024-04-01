#include "CMDSTOR.h"

#define MAXSIZE 100

void CMDSTOR::processCMD(string cmd,string msg){
    
    cout<<"enter store"<<endl;
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
    outFile=std::ofstream(filePath,ios::binary);

    // bufferevent_disable(belongTask->_bev,EV_READ); //关闭主事件的监听
    // evutil_socket_t sockfd=bufferevent_getfd(belongTask->_bev);
    // char buf[MAXSIZE]="220 able to accept file\r\n";
    // if(-1==send(sockfd,buf,sizeof(buf),0)){
    //     cout<<"send faild!"<<endl;
    // }
    if(belongTask->transMode==ACTIVEMODE){
        ConnectDataPipe(); //主动连接
    }
    else{  //被动连接
        auto it=reinterpret_cast<FTPserverCMD*>(belongTask);
        _bev=it->TaskCMD["PASV"]->_bev;
    }
    // if ( fcntl(sockfd, F_SETFL, 0)== -1) {
    //     std::perror("Error setting socket option");
    // }
    read(_bev);
    // int recvLen=recv(sockfd,buf,sizeof(buf),0);
    // if(recvLen==-1){
    //     std::cout << "CMDSTOR 45: recv data failed! Error code: " << errno << std::endl;
    // }
    // buf[recvLen]='\0';
    
    // int len=std::stoi(buf);
    // cout<<len<<endl;
    // int recvSize=0;

    // cout<<"CMDSTOR.CPP 61:read end"<<endl;
    // int oldSocketFlag = fcntl(sockfd, F_GETFL, 0);
    // int newSocketFlag = oldSocketFlag | O_NONBLOCK;
    // fcntl(sockfd, F_SETFL, newSocketFlag);

    // bufferevent_enable(belongTask->_bev,EV_READ);

        resPondimmediately("150 Here comes the directory listing.\r\n");

        bufferevent_flush(_bev, EV_WRITE, BEV_FLUSH);

        Closefd();
        resPond("226 Directory send OK.\r\n");
    
}

void CMDSTOR::read(struct bufferevent* bev){
    char buf[MAXSIZE];
    int len=0;
    int tfd=bufferevent_getfd(_bev);
    if(outFile.is_open()){
        cout<<"strat recv"<<endl;
        while((len=recv(tfd,buf,sizeof(buf),0))>0){
            cout<<"CMDSTOR.cpp 72:recv data "<<len<<endl;
            buf[len]='\0';
            outFile.write(buf, len);
        }
    }
    Closefd();
}


void CMDSTOR::event(struct bufferevent* bev,short _event){
    if(_event&(BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT)){
        Closefd();
        resPond("msg");
    }
}

void CMDSTOR::Closefd(){
    cout<<"CMDSTOR.cpp 88:bev is closed!"<<endl;
    if(_bev){
        bufferevent_free(_bev);
    }
    if(outFile.is_open()){
        outFile.close();
    }
}