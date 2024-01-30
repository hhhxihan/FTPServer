#pragma once
#include "FTPTask.h"

class CMDWD:public FTPTask{
    public:
        void processCMD(string cmd,string msg){
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
            }
            
        }
};

    