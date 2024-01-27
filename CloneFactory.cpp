#include "FTPTask.h"
#include "FTPserverCMD.h"
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
        // TaskCMD->registerCMD();
        // TaskCMD->registerCMD();
        // TaskCMD->registerCMD();
        // TaskCMD->registerCMD();

        return TaskCMD;
    }
    private:
    CloneFactory(){};
    ~CloneFactory(){};
    static CloneFactory* singleFactory;
};