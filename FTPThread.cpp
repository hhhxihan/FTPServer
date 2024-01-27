#include "FTPThread.h"
#include <thread>
void FTPThread::AddTask(FTPTask* task){
    
    task->Init();
}



void FTPThread::Init(){//线程的初始化
    base=event_base_new();

    std::thread th(Main);
    th.detach();
    
}

void FTPThread::Main(){ //线程的函数就是启动event_base；
	event_base_dispatch(base);
	event_base_free(base);

}