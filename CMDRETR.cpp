#include "CMDRETR.h"

void CMDRETR::processCMD(string cmd,string msg){
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
    File=std::ifstream(filePath);

    ConnectDataPipe();
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



