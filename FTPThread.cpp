#include "FTPThread.h"
#include <thread>
#include <event2/event.h>
#include <event2/bufferevent.h>
void FTPThread::AddTask(FTPTask* task){
    std::unique_lock lock(mux);
    taskQueue.push(task);
    char str[]="hhh";
    if(-1==write(pipefd[1],str,strlen(str))){
        cout<<"write pipe failed"<<endl;
    }
}

void FTPThread::callback(evutil_socket_t fd,short event,void* arg){
    char buf[10];
    FTPThread* t=reinterpret_cast<FTPThread*>(arg);
    int re=read(t->pipefd[0],buf,sizeof(buf));
    std::unique_lock lock(t->mux);
    while(!t->taskQueue.empty()){
        FTPTask* task=t->taskQueue.front();
        t->taskQueue.pop();
        if(!t->base) cout<<"FTPThread.cpp 21:thread base is null"<<endl;
        task->Init(t->base);//初始化任务
    }
    
}


void FTPThread::Init(){//线程的初始化
    if(-1==pipe(pipefd)){
        cout<<"pipe create failed!"<<endl;
    }
    base=event_base_new();
    if(!base) cout<<"Thread event_base create failed"<<endl;

    struct event* ev=event_new(base,pipefd[0],EV_READ| EV_PERSIST,callback,this);
    event_add(ev,NULL);
    std::thread th(&FTPThread::Main,this);
    th.detach();
    
}

void FTPThread::Main(){ //线程的函数就是启动event_base；

	event_base_dispatch(base);
	event_base_free(base);

}