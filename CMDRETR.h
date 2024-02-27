#include "FTPTask.h"
#include <fstream>
#include <filesystem>

#define MAXSIZE 1024

class CMDRETR:public FTPTask{
    public:
        void read(struct bufferevent* bev);
        // void write(struct bufferevent* bev);
        void event(struct bufferevent* bev,short _event);
        void processCMD(string cmd,string msg);
        void Closefd(){};
    protected:
         ifstream File; 
};