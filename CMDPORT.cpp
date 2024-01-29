#include "FTPTask.h"
#include <vector>
using namespace std;


class CMDPORT:public FTPTask{
    public:
        void processCMD(string cmd,string msg){ //处理PORT命令
            vector<string> v;
            string s="";
            for(char i: msg){
                if(i==' '||i==','){
                    v.push_back(s);
                    s="";
                }
                s.push_back(i);
            }

            this->transIP=v[1]+'.'+v[2]+'.'+v[3]+'.'+v[4];
            this->transPort=256*stoi(v[5])+stoi(v[6]);  //解析出IP和port

            belongTask->transIP=this->transIP;
            belongTask->transPort=this->transPort;
        }
};
