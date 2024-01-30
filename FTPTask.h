#include "Task.h"
#include <string>
#include <psdk_inc/_socket_types.h>
using namespace std;

class FTPTask:public Task{
    public:
        std::string currentDir="/";
        std::string rootDir=".";

        std::string transIP="";
        int transPort=0;

        FTPTask* belongTask;
        struct event_base* base; //Libevent库

        virtual void processCMD(string cmd,string msg){}; //用于处理命令
        
        void resPond(string msg); //回复命令

        void ConnectDataPipe();

        void sendData(string msg);
        int Init(struct event_base* tbase) override {  //将任务添加到base中进行监听，并添加回调函数
            // this->base=tbase;
            // struct bufferevent* bev=bufferevent_socket_new(base,socketID,BEV_OPT_CLOSE_ON_FREE);
            // bufferevent_setcb(bev,readCB,writeCB,eventCB,this);
            // bufferevent_enable(bev,EV_READ|EV_WRITE);
            return 0;
        }  

    protected:
        void readCB(struct bufferevent* bev,void* arg);   //读事件的回调函数
        void writeCB(struct bufferevent* bev,void* arg);  //写事件的回调函数
        void eventCB(struct bufferevent* bev,void* arg);  //出错时的回调函数

        struct bufferevent* bev;  //task中的一个传输控制命令的socket
        
        int threadID; //所属线程ID；
};

