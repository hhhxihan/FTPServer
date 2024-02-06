#include <event2/event.h>
#include <event2/listener.h>
#include "FTPThreadPool.h"
#include "CloneFactory.cpp"

using namespace std;

FTPThreadPool *threadPool;
CloneFactory* CloneFactory::singleFactory=nullptr;

void callback(struct evconnlistener* evlistener,evutil_socket_t fd,struct sockaddr* address,int socklen,void* arg){
    FTPTask* t=CloneFactory::get()->createTask();
    t->socketID=fd;
    threadPool->addTask(t);
    //把t分配给一个线程
}

int main()
{

    FTPTask* t=new FTPTask();
    event_base* base=event_base_new(); //创建Libevent事件库

    threadPool=new FTPThreadPool(10);
    //设置服务器的地址信息
    struct sockaddr_in server_addr;
    server_addr.sin_addr.s_addr=INADDR_ANY;
    server_addr.sin_port=htons(21);
    server_addr.sin_family=AF_INET;

    struct evconnlistener* ev=evconnlistener_new_bind(base,callback,NULL, LEV_OPT_REUSEABLE,100,
                                            (struct sockaddr*)&server_addr,sizeof(server_addr));
    event_base_dispatch(base);
    
    evconnlistener_free(ev);
    event_base_free(base);
    delete threadPool;
}


