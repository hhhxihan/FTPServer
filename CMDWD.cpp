#pragma once
#include "FTPTask.h"
#include <vector>
#include <event2/util.h>
#include <cstring>
#include "FTPserverCMD.h"
#include <iostream>
#include "json.hpp"
#include <unistd.h>
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
                    resPond("250 \""+ belongTask->currentDir+"\" is the current directory\r\n");
                }
                else if(path.at(1)=='.') processCMD("CDUP","");
                else{
                    if(belongTask->currentDir[belongTask->currentDir.size()-1]!='/'){
                        belongTask->currentDir.append("/");
                    }
                    belongTask->currentDir.append(path);
                    resPond("250 \""+ belongTask->currentDir+"\" is the current directory\r\n");
                }
                
            }
            else if(cmd=="PWD"){
                string curDir=belongTask->currentDir;
                resPond("257 \""+curDir+"\" is the current directory\r\n");
            }
            else if(cmd=="CDUP"){  //返回上一级目录
                
                string path=belongTask->currentDir;
                if(path[path.size()-1]=='/'&&path.size()>1){
                    path=path.substr(0,path.size()-1);
                }
                int pos=path.rfind("/");
                if(pos==0){
                    resPond("257 \"/\" is the current directory\r\n");
                }else{
                    path=path.substr(0,pos);
                    belongTask->currentDir=path;
                // processCMD("LIST","a"); //切换路径后，还要列出文件返回
                    resPond("257 \""+path+"\" is the current directory\r\n");
                }
                
            }
            else if(cmd=="LIST"){ //List要用数据通道发送
                transIP=belongTask->transIP;
                transPort=belongTask->transPort;
                
                bufferevent_flush(belongTask->_bev,EV_WRITE,BEV_FLUSH);
                if(belongTask->transMode==ACTIVEMODE){
                    ConnectDataPipe(); //主动连接
                }
                else{  //被动连接
                    auto it=reinterpret_cast<FTPserverCMD*>(belongTask);
                    _bev=it->TaskCMD["PASV"]->_bev; //获取到数据通道的套接字
                }

                string result;
                string _Command;
                cout<<"CMD.cpp 50:workDir:"<<belongTask->currentDir<<endl;
                _Command.append("ls -al "+belongTask->currentDir);
                file=popen(_Command.c_str(),"r");
                char buf[100];
                fgets(buf,100,file); //读出total
                //读取数据
                if(file){
                    while(fgets(buf,100,file)!=NULL){
                        result+=buf;
                        result[result.size()-1]='\r';
                        result+="\n";
                    }
                }
                if(!result.size()) cout<<"CMDWD.cpp 52:respondMsg is null"<<endl;
                
                #ifdef DEBUG_QT_JSON
                //编写格式，json格式：
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
                #endif
                // bufferevent_flush(_bev,EV_WRITE,BEV_FLUSH);
                // bufferevent_disable(_bev,EV_READ|EV_WRITE);
                sendData(result);
                resPondimmediately("150 Here comes the directory listing.\r\n");
                // int tfd=bufferevent_getfd(_bev);
                // close(tfd);
                // if(_bev!=NULL){
                //     cout<<"colse data bev"<<endl;
                //     bufferevent_free(_bev);
                // }
                
                // if(file!=nullptr) pclose(file);
                Closefd();
                resPond("226 Directory send OK.\r\n");
                
            }
            
        }
        
        void event(struct bufferevent* bev, short what){
            cout<<"event dongi 1"<<endl;
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


