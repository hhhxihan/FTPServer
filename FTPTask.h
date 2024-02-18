#ifndef FTPTASK_H
#define FTPTASK_H

#include "Task.h"
#include <string>
#include <iostream>
#include <event2/event.h>
#include <event2/bufferevent.h>

using namespace std;

class FTPTask:public Task{
    public:
        std::string currentDir="/";  //目录的绝对路径
        std::string rootDir=".";     //当前目录的相对路径

        std::string transIP="";
        int transPort=0;

        FTPTask* belongTask;
        event_base* base; //Libevent库int
        evutil_socket_t  socketID; 

        virtual void processCMD(string cmd,string msg){}; //用于处理命令
        
        void resPond(string msg); //回复命令

        void ConnectDataPipe();

        void sendData(string msg);
        int Init(struct event_base* tbase) {  //将任务添加到base中进行监听，并添加回调函数
            this->base=tbase;
            if(!base){
                cout<<"base is null"<<endl;
            }
            struct bufferevent* bev=bufferevent_socket_new(base,socketID,BEV_OPT_CLOSE_ON_FREE);
            if(bev){
                cout<<"enter bev process!"<<endl;
                bufferevent_setcb(bev,readCB,writeCB,eventCB,this);
                cout<<"set callback success!"<<bev<<endl;
                if(-1==bufferevent_enable(bev,EV_READ)){
                    cout<<"buffevent_enable faild!"<<endl;
                }
                cout<<"buffevent_enable success!"<<endl;
                char buf[]="220 connect success!";
                bufferevent_write(bev,buf,sizeof(buf));
                cout<<"connect success!"<<endl;
            }
            else{
                cout<<"bufferevent create failed2"<<endl;
             }
             
            return 0;
        }  

        void virtual Closefd();

        void virtual read(struct bufferevent* bev){}
        void virtual write(struct bufferevent* bev){}
        void virtual event(struct bufferevent* bev,short _event){}

    protected:
        static void readCB(struct bufferevent* bev,void* arg);   //读事件的回调函数
        static void writeCB(struct bufferevent* bev,void* arg);  //写事件的回调函数
        static void eventCB(struct bufferevent* bev,short event,void* arg);  //出错时的回调函数

        struct bufferevent* bev;  //task中的一个传输控制命令的socket
        
        int threadID; //所属线程ID；
        
        FILE* file;
};

#endif