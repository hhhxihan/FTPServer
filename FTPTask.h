#include "Task.h"
#include <string>
#include <psdk_inc/_socket_types.h>
using namespace std;

class FTPTask:public Task{
    public:
        virtual int processCMD(string cmd,string msg); //用于处理命令
        void readcmd();  //读取命令
        int resPond(); //回复命令
        int Init(){}

    private:
        int readCB();   //读事件的回调函数
        int writeCB();  //写事件的回调函数
        int eventCB();  //出错时的回调函数

        SOCKET socketID;  //task中的一个传输控制命令的socket
        base_event* base; //Libevent库
        Thread_t threadID; //所属线程ID；
};