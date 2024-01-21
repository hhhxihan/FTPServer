#include <winsock2.h>
using namespace std;

void callback(void* arg){
    
}

int main()
{

    event_base* base=event_base_new();


    //设置服务器的地址信息
    struct sockaddr_in server_addr;
    server_addr.sin_addr.s_addr=INADDR_ANY;
    server_addr.sin_port=htons(21);
    server_addr.sin_family=AF_INET;

    struct evconnlistener* ev=evconnlistener_new_bind(base,callback,NULL, LEV_OPT_REUSEABLE,100,
                                            (struct sockaddr*)&server_addr,sizeof(server_addr));
    event_base_dispatch(base);
    
    event_evconnlistener_free(ev);
    event_base_free(base);
}


