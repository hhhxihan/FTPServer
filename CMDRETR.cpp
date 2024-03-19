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
    resPond(fileLen);
    
    bufferevent_disable(belongTask->_bev,EV_READ); //
    evutil_socket_t _fd=bufferevent_getfd(belongTask->_bev);
    char buf[MAXSIZE];
    recv(_fd,buf,sizeof(buf),0);

    ConnectDataPipe(); //连接数据通道
    File.seekg(0,ios::beg);
    //传输数据
    int sendSize=0;
    
    while((sendSize=File.readsome(buf,MAXSIZE))>0){
        sendData(buf);
    }

    bufferevent_enable(belongTask->_bev,EV_READ);
    Closefd(); //关闭连接
    //传输完后还要回复currentdir
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



