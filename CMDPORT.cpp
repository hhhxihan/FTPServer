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
            }
            
        }
};
