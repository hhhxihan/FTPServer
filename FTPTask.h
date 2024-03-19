#ifndef FTPTASK_H
#define FTPTASK_H

#include "Task.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <event2/listener.h>
#include <event2/event.h>
#include <event2/bufferevent.h>

using namespace std;

class FTPTask:public Task{
    public:
        std::string currentDir="/home";  //目录的绝对路径
        std::string rootDir=".";     //当前目录的相对路径

        std::string transIP="";
        int transMode=ACTIVEMODE;
        int transPort=20;

        FTPTask* belongTask;
        event_base* base; //Libevent库int
        evutil_socket_t  socketID; 
        struct bufferevent* _bev;  //task中的一个传输控制命令的socket
        struct evconnlistener* ev; //被动模式下，要监听新的连接请求
        void getConnInfo(struct sockaddr* address);

        virtual void processCMD(string cmd,string msg){}; //用于处理命令
        
        virtual void resPond(string msg); //回复命令
        virtual void respWD(){}; //回复当前路径

        void ConnectDataPipe(); //主动链接
        void PassConnect(); //被动连接

        void sendData(string msg);
        int Init(struct event_base* tbase) {  //将任务添加到base中进行监听，并添加回调函数
            base=tbase;
            if(!base){
                cout<<"base is null"<<endl;
            }
            _bev=bufferevent_socket_new(base,socketID,BEV_OPT_CLOSE_ON_FREE);
            if(_bev){
                cout<<"enter bev process!"<<endl;
                bufferevent_setcb(_bev,readCB,writeCB,eventCB,this);
                if(-1==bufferevent_enable(_bev,EV_READ)){
                    cout<<"buffevent_enable faild!"<<endl;
                }
                char buf[]="220 this is libevent Ftp_Server, Welcome!\r\n";
                bufferevent_write(_bev,buf,sizeof(buf));

                respWD();

            }
            else{
                cout<<"bufferevent create failed2"<<endl;
             }
             
            return 0;
        }  
        void setIP(struct sockaddr* address);
        

        virtual void Closefd();
        virtual void read(bufferevent* bev){}
        virtual void write(bufferevent* bev){}
        virtual void event(struct bufferevent* bev,short _event){}
        

    protected:
        static void readCB(struct bufferevent* bev,void* arg);   //读事件的回调函数
        static void writeCB(struct bufferevent* bev,void* arg);  //写事件的回调函数
        static void eventCB(struct bufferevent* bev,short event,void* arg);  //出错时的回调函数
        static void passConnCallback(struct evconnlistener* listener,evutil_socket_t fd,struct sockaddr* addr,int socklen,void* arg);

        
        
        int threadID; //所属线程ID；
        bool dataPipeConn;
        
        FILE* file;
};

#endif