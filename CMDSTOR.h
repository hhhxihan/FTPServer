#include "FTPTask.h"
#include <fstream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

class CMDSTOR:public FTPTask{
    public:
        void read(struct bufferevent* bev);
        // void write(struct bufferevent* bev);
        void event(struct bufferevent* bev,short _event);
        void processCMD(string cmd,string msg);
        void Closefd();
    protected:
        ofstream outFile;        
};