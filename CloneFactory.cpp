#include "FTPTask.h"
#include "FTPserverCMD.h"
#include "CMDPORT.cpp"
#include "CMDWD.cpp"
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
        FTPTask* TaskCMD=new FTPserverCMD();
        TaskCMD->registerCMD("PORT",new CMDPORT());
        FTPTask* wd=new CMDWD();
        TaskCMD->registerCMD("LIST",wd);
        TaskCMD->registerCMD("PWD",wd);
        TaskCMD->registerCMD("CWD",wd);

        return TaskCMD;
    }
    private:
    CloneFactory(){};
    ~CloneFactory(){};
    static CloneFactory* singleFactory;
};