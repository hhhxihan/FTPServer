#include "FTPTask.h"
#include <vector>
#include <random>
using namespace std;


class CMDPORT:public FTPTask{
    public:
        void processCMD(string cmd,string msg){ //处理PORT命令
            if(cmd=="PORT"){
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
            else if(cmd=="PASV"){
                belongTask->transMode=PASVMODE;
                //随机生成端口号
                //获取到自己的IP地址
                std::random_device rd;
                std::mt19937 gen(rd()); // 使用 Mersenne Twister 算法

                // 定义一个随机数分布
                std::uniform_int_distribution<> dis(160, 200); // 生成 1 到 100 之间的整数
                // 生成随机数
                int random_number = dis(gen);
                belongTask->transPort=random_number*256;
                string tcmd="ifconfig | grep -Eo 'inet (addr:)?([0-9]*\\.){3}[0-9]*' | grep -Eo '([0-9]*\\.){3}[0-9]*' | grep -v '127.0.0.1'";
                file=popen(tcmd.c_str(),"r");
                char buf[100];
                fgets(buf,100,file);
                string respStr(buf);
                respStr+=","+std::to_string(belongTask->transPort)+",0)\r\n";
                for(char& c:respStr) if(c=='.') c=',';
                resPond("227 Entering Passive Mode ("+respStr);
            }
            
        }
};
