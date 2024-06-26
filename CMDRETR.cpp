#include "CMDRETR.h"

void CMDRETR::processCMD(string cmd,string msg){
    int pos=msg.find(" ");
    msg=msg.substr(pos+1,msg.size()-pos-3); //去掉末尾的\r\n
    std::filesystem::path _path(msg.c_str());

    string _p=_path.string();
    string tempDir="";
    if(!_p.empty()){ 
        if(belongTask->currentDir.at(belongTask->currentDir.size()-1)!='/'){
            belongTask->currentDir+="/";
        }
        
        if(_p.at(0)=='/'){  //绝对路径
            tempDir=belongTask->currentDir+_p;
        }
        else if(_p.at(0)=='.'){  // ./xxx类型相对路径
            _p=_p.substr(2,_p.size()-2);
            tempDir=belongTask->currentDir+_p;
        }
        else{
            tempDir=belongTask->currentDir+_p;
        }
        cout<<"The file is:"<<tempDir<<endl;
    }
    //现在tempDir是要下载的文件
    //如果是文件夹。
    std::filesystem::path filePath(tempDir);
    File=std::ifstream(filePath,std::ios::binary | std::ios::ate);
    std::streampos size=File.tellg();
    std::streamoff tsize=static_cast<streamoff>(size);
    string fileLen=std::to_string(tsize); //打开文件，并且计算大小。

    
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
    
    resPondimmediately("150 Opening BINARY mode data connection.\r\n");
    sendData(File);
    
    // bufferevent_enable(belongTask->_bev,EV_READ);
    

    bufferevent_flush(_bev, EV_WRITE, BEV_FLUSH);

    Closefd();
    resPond("226 Transfer complete.\r\n");
    
}
void CMDRETR::sendData(std::ifstream& File){
    int filesize=0;
    int sendSize=0;
    if(_bev!=NULL){
        int tfd=bufferevent_getfd(_bev);
        set_socket_blocking(tfd,1);
        char buf[MAXSIZE];
        while((sendSize=File.readsome(buf,MAXSIZE))>0){
            filesize+=sendSize;
            send(tfd,buf,sizeof(buf),0);
        }
        cout<<"CMDRETR.cpp 56: filesize:"<<filesize<<endl;
        set_socket_blocking(tfd,0);
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




