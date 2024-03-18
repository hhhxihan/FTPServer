#ifndef CLONEFACTORY_CPP
#define CLONEFACTORY_CPP

#include "FTPTask.h"
#include "FTPserverCMD.h"
#include "CMDPORT.cpp"
#include "CMDWD.cpp"
#include "CMDSTOR.h"
#include "CMDRETR.h"
#include "CMDOPT.h"
#include "CMDUSER.h"
//单例模式的工厂

class CloneFactory{
    public:
    static CloneFactory* get(){
        if(!singleFactory){
            singleFactory=new CloneFactory();
        }
        return singleFactory;
    }
    FTPTask* createTask(){
        FTPserverCMD* TaskCMD=new FTPserverCMD();
        TaskCMD->registerCMD("PORT",new CMDPORT());
        FTPTask* wd=new CMDWD();
        
        TaskCMD->registerCMD("LIST",wd);
        TaskCMD->registerCMD("PWD",wd);
        TaskCMD->registerCMD("CWD",wd);
        TaskCMD->registerCMD("CDUP",wd);

        FTPTask* user=new CMDUSER();
        TaskCMD->registerCMD("USER",wd);
        TaskCMD->registerCMD("PASS",wd);
        
        TaskCMD->registerCMD("STOR",new CMDSTOR());
        TaskCMD->registerCMD("RETR",new CMDRETR());
        TaskCMD->registerCMD("OPTS",new CMDOPT());

        return TaskCMD;
    }
    private:
    CloneFactory(){};
    ~CloneFactory(){};
    static CloneFactory* singleFactory;
};
#endif