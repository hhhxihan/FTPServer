#include "CMDSTOR.h"
#include <filesystem>
#include <fstream>
#define MAXSIZE 100

void CMDSTOR::processCMD(string cmd,string msg){
    int pos=msg.find(" ");
    msg=msg.substr(pos+1,msg.size()-pos-2); //去掉末尾的\r\n
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

    ConnectDataPipe();
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