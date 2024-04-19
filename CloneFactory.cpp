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
#include "CMDTYPE.h"
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

        FTPTask* port=new CMDPORT();
        TaskCMD->registerCMD("PORT",port);
        TaskCMD->registerCMD("PASV",port);

        FTPTask* wd=new CMDWD();
        TaskCMD->registerCMD("LIST",wd);
        TaskCMD->registerCMD("PWD",wd);
        TaskCMD->registerCMD("CWD",wd);
        TaskCMD->registerCMD("CDUP",wd);

        FTPTask* user=new CMDUSER();
        TaskCMD->registerCMD("USER",user);
        TaskCMD->registerCMD("PASS",user);

        FTPTask* opt=new CMDOPT();
        TaskCMD->registerCMD("SYST",opt);
        TaskCMD->registerCMD("FEAT",opt);
        TaskCMD->registerCMD("OPTS",opt);

        TaskCMD->registerCMD("TYPE",new CMDTYPE());
        FTPTask* stor=new CMDSTOR();
        TaskCMD->registerCMD("STOR",stor);
        TaskCMD->registerCMD("MDTM",stor);
        TaskCMD->registerCMD("RETR",new CMDRETR());


        

        return TaskCMD;
    }
    private:
    CloneFactory(){};
    ~CloneFactory(){};
    static CloneFactory* singleFactory;
};
#endif