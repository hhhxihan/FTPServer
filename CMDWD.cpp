#pragma once
#include "FTPTask.h"
#include <iostream>
class CMDWD:public FTPTask{
    public:
        void processCMD(string cmd,string msg){
            base=belongTask->base;
            if (cmd=="CWD")
            {
                int pos=msg.find(" ");
                string path=msg.substr(pos+1,path.size()-pos-2);
                if(path.at(0)=='/'){
                    belongTask->currentDir=path;
                }
                else{
                    if(belongTask->currentDir[belongTask->currentDir.size()-1]!='/'){
                        belongTask->currentDir.append("/");
                    }
                    belongTask->currentDir.append(path);
                }
            }
            else if(cmd=="PWD"){
                string curDir=belongTask->currentDir;
                resPond(curDir);
            }
            else if(cmd=="CDUP"){  //返回上一级目录
                string path=currentDir;
                if(path[path.size()-1]=='/'){
                    path=path.substr(0,path.size()-1);
                }
                int pos=path.rfind("/");
                path=path.substr(0,pos);
                belongTask->currentDir=path;

                resPond("CDUP success!");
            }
            else if(cmd=="LIST"){ //List要用数据通道发送
                transIP=belongTask->transIP;
                transPort=belongTask->transPort;

                ConnectDataPipe();
                string result;
                string _Command;
                _Command.append("ls -l "+belongTask->currentDir);
                file=popen(_Command.c_str(),"r");
                char buf[100];
                if(!file){
                    while(fgets(buf,100,file)!=NULL){
                        result+=buf;
                    }
                }
                sendData(result);
            }
            
        }
        
        void event(struct bufferevent* bev, short what){
            if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT))
            {
                cout << "BEV_EVENT_EOF | BEV_EVENT_ERROR" << endl;
                Closefd();
                resPond("226 Transfer complete\r\n");
            }
        }
};


