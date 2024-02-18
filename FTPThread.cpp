#include "FTPThread.h"
#include <thread>
#include <event2/event.h>
#include <event2/bufferevent.h>
void FTPThread::AddTask(FTPTask* task){
    std::unique_lock lock(mux);
    taskQueue.push(task);
    char str[]="h";
    write(pipefd[1],str,sizeof(str));
    
}

void FTPThread::callback(evutil_socket_t fd,short event,void* arg){
    char buf[2];
    int re=read(fd,buf,sizeof(buf));
    FTPThread* t=reinterpret_cast<FTPThread*>(arg);
    std::unique_lock lock(t->mux);
    while(!t->taskQueue.empty()){
        FTPTask* task=t->taskQueue.front();
        t->taskQueue.pop();
        task->Init(t->base);//初始化任务
    }
    
}


void FTPThread::Init(){//线程的初始化
    if(-1==pipe(pipefd)){
        cout<<"pipe create failed!"<<endl;
    }
    base=event_base_new();
    if(!base) cout<<"Thread event_base create failed"<<endl;

    struct event* ev=event_new(base,pipefd[0],EV_READ,callback,this);
    event_add(ev,0);
    std::thread th(&FTPThread::Main,this);
    th.detach();
    
}

void FTPThread::Main(){ //线程的函数就是启动event_base；

	event_base_dispatch(base);
	event_base_free(base);

}