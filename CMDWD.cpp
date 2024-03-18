#pragma once
#include "FTPTask.h"
#include <vector>
#include <event2/util.h>
#include <cstring>
#include <iostream>
#include "json.hpp"
using json=nlohmann::json;
class CMDWD:public FTPTask{
    public:
        void processCMD(string cmd,string msg){
            base=belongTask->base;
            if (cmd=="CWD")
            {
                int pos=msg.find(" ");
                string path=msg.substr(pos+1,msg.size()-pos-3);
                if(path.at(0)=='/'){
                    belongTask->currentDir=path;
                }
                else{
                    if(belongTask->currentDir[belongTask->currentDir.size()-1]!='/'){
                        belongTask->currentDir.append("/");
                    }
                    belongTask->currentDir.append(path);
                }
                processCMD("LIST","a"); //切换路径后，还要列出文件返回
            }
            else if(cmd=="PWD"){
                string curDir=belongTask->currentDir;
                resPond("257 \""+curDir+"\" is the current directory\r\n");
            }
            else if(cmd=="CDUP"){  //返回上一级目录
                string path=belongTask->currentDir;
                if(path[path.size()-1]=='/'){
                    path=path.substr(0,path.size()-1);
                }
                int pos=path.rfind("/");
                path=path.substr(0,pos);
                belongTask->currentDir=path;
                processCMD("LIST","a"); //切换路径后，还要列出文件返回
                resPond("220 CDUP success!");//回复消息  
            }
            else if(cmd=="LIST"){ //List要用数据通道发送
                transIP=belongTask->transIP;
                transPort=belongTask->transPort;

                ConnectDataPipe();
                string result;
                string _Command;
                cout<<"CMD.cpp 50:workDir:"<<belongTask->currentDir<<endl;
                _Command.append("ls "+belongTask->currentDir);
                file=popen(_Command.c_str(),"r");
                char buf[100];
                if(file){
                    while(fgets(buf,100,file)!=NULL){
                        result+=buf;
                    }
                }
                if(!result.size()) cout<<"CMDWD.cpp 52:respondMsg is null"<<endl;
                cout<<"the string is:"<<result<<endl;
                std::vector<string> v;
                v.push_back("");
                int j=0;
                json js;
                js["vector"]=json::array();
                for(char c:result){
                    if(c!=' '&&c!='\n'){
                        v[j].push_back(c);
                    }
                    else{
                        js["vector"].push_back(v[j]);
                        j++;
                        v.push_back("");
                    }
                }
                v.pop_back();
                sendData(js.dump()+"\r\n");
                pclose(file);
                Closefd();
            }
            
        }
        
        void event(struct bufferevent* bev, short what){
            if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT))
            {
                cout << "BEV_EVENT_EOF | BEV_EVENT_ERROR" << endl;
                // 获取通用的系统错误码
                int socket_error = EVUTIL_SOCKET_ERROR();
                cout << "Socket Error: " << evutil_socket_error_to_string(socket_error) << endl;
                Closefd();
                
            }
        }
};


