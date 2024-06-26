#include <event2/event.h>
#include <event2/listener.h>
#include "FTPThreadPool.h"
#include "CloneFactory.cpp"
#include <event2/util.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h> // 包含TCP_NODELAY定义
int cnt=0;

using namespace std;

FTPThreadPool *threadPool=nullptr;
CloneFactory* CloneFactory::singleFactory=nullptr;

void callback(struct evconnlistener* evlistener,evutil_socket_t fd,struct sockaddr* address,int socklen,void* arg){
    cout<<"new conncet:"<<++cnt<<endl;
    FTPTask* t=CloneFactory::get()->createTask();
    
    if(!t) cout<<"FTPTask create failed"<<endl;
    t->socketID=fd;
    int flag = 1;
    int result = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));
    if (result < 0) {
        // 处理错误
        cout<<"setsocketopt failed"<<endl;
    }
    t->setIP(address);
    t->getConnInfo(address);
    threadPool->addTask(t);
    //把t分配给一个线程
}

int main()
{

    event_base* base=event_base_new(); //创建Libevent事件库
    if(!base) cout<<"event_base create failed"<<endl;

    threadPool=new FTPThreadPool(4);
    if(!threadPool) cout<<"threadPool create failed"<<endl;

    //设置服务器的地址信息
    struct sockaddr_in server_addr;
    server_addr.sin_addr.s_addr=INADDR_ANY;
    server_addr.sin_port=htons(21);
    server_addr.sin_family=AF_INET;

    struct evconnlistener* ev=evconnlistener_new_bind(base,callback,base, LEV_OPT_REUSEABLE,100,
                                            (struct sockaddr*)&server_addr,sizeof(server_addr));
    if (!ev) { 
        cout << "evconnlistener create failed: " << evutil_socket_error_to_string( evutil_socket_geterror()) << endl;
    }

    cout<<"start listen..."<<endl;
    event_base_dispatch(base);
    cout<<"close..."<<endl;
    evconnlistener_free(ev);
    event_base_free(base);
    delete threadPool;

}


