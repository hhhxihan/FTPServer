#include "FTPTask.h"
#include <fstream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <filesystem>
#include <sys/socket.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <errno.h>

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