#include "FTPThread.h"
#include <thread>
#include <event2/event.h>
#include <event2/bufferevent.h>
void FTPThread::AddTask(FTPTask* task){

    task->Init(base);//初始化任务
}



void FTPThread::Init(){//线程的初始化
    base=event_base_new();
    if(!base) cout<<"Thread event_base create failed"<<endl;
    std::thread th(&FTPThread::Main,this);
    th.detach();
    
}

void FTPThread::Main(){ //线程的函数就是启动event_base；

	event_base_dispatch(base);
	event_base_free(base);

}