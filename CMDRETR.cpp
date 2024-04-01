#include "CMDRETR.h"

void CMDRETR::processCMD(string cmd,string msg){
    int pos=msg.find(" ");
    msg=msg.substr(pos+1,msg.size()-pos-3); //去掉末尾的\r\n
    std::filesystem::path _path(msg.c_str());

    string _p=_path.string();

    if(!_p.empty()){ 
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
        cout<<"The file is:"<<belongTask->currentDir;
    }

    std::filesystem::path filePath(belongTask->currentDir);
    File=std::ifstream(filePath,std::ios::binary | std::ios::ate);
    std::streampos size=File.tellg();
    std::streamoff tsize=static_cast<streamoff>(size);
    string fileLen=std::to_string(tsize);
    // resPond(fileLen);
    
    // bufferevent_disable(belongTask->_bev,EV_READ); //
    // evutil_socket_t _fd=bufferevent_getfd(belongTask->_bev);
    
    // recv(_fd,buf,sizeof(buf),0);
    char buf[MAXSIZE];
    if(belongTask->transMode==ACTIVEMODE){
        ConnectDataPipe(); //主动连接
    }
    else{  //被动连接
        auto it=reinterpret_cast<FTPserverCMD*>(belongTask);
        _bev=it->TaskCMD["PASV"]->_bev; //获取到数据通道的套接字
        if(_bev==nullptr) cout<<"CMDWD.cpp 63: _bev is NULL"<<endl;
    }//连接数据通道
    File.seekg(0,ios::beg);
    //传输数据
    int sendSize=0;
    
    while((sendSize=File.readsome(buf,MAXSIZE))>0){
        sendData(buf);
    }
    
    // bufferevent_enable(belongTask->_bev,EV_READ);
    resPondimmediately("150 Opening BINARY mode data connection for favicon.ico (15086 bytes).\r\n");

    bufferevent_flush(_bev, EV_WRITE, BEV_FLUSH);

    Closefd();
    resPond("226 Transfer complete.\r\n");
    
}
void CMDRETR::sendData(string msg){
    if(_bev!=NULL){
        int tfd=bufferevent_getfd(_bev);
        send(tfd,msg.c_str(),msg.size(),0);
        // bufferevent_write(_bev,msg.c_str(),msg.size());
        // bufferevent_flush(_bev, EV_WRITE, BEV_FLUSH);
    }else{
        cout<<"FTPTask:94 _bev is NULL\n";
    }
}

void CMDRETR::read(struct bufferevent* bev){
    string buf;
    if(File.is_open()&&!bev){
        while(getline(File,buf)){
            bufferevent_write(bev,buf.c_str(),buf.size());
        }
    }
}

void CMDRETR::event(struct bufferevent* bev,short _event){
    if(_event&(BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT)){
        Closefd();
        resPond("msg");
    }
}
void CMDRETR::Closefd(){
    if(_bev){
        bufferevent_free(_bev);
        _bev=nullptr;
    }
    if(File.is_open()){
        File.close();
    }
}




