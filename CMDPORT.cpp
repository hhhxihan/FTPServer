#include "FTPTask.h"
#include <vector>
#include <random>
using namespace std;


class CMDPORT:public FTPTask{
    public:
    void event(struct bufferevent* bev,short _event){
        cout<<"event is doing2"<<endl;
        if(_event&BEV_EVENT_EOF){
            cout<<"_bev disconn"<<endl;
             resPond("226 Directory send OK.\r\n");
        }else if(_event&BEV_EVENT_ERROR){
            cout<<"data _bev error"<<endl;
            resPond("226 Directory send OK.\r\n");
        }else if(_event&BEV_EVENT_TIMEOUT){
            cout<<"time out"<<endl;
        }
    }
        void processCMD(string cmd,string msg){ //处理PORT命令
            if(cmd=="PORT"){
                belongTask->transMode=ACTIVEMODE;
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
                belongTask->waitConn=1;
                belongTask->transMode=PASVMODE;
                //随机生成端口号
                //获取到自己的IP地址
                std::random_device rd,rd1;
                std::mt19937 gen(rd()),gen1(rd1()); // 使用 Mersenne Twister 算法

                // 定义一个随机数分布
                std::uniform_int_distribution<> dis(160, 200);
                std::uniform_int_distribution<> dis2(0, 255); 
                // 生成随机数
                int random_number = dis(gen);
                int random_number1 =dis2(gen1);
                belongTask->transPort=random_number*256+random_number1;
                string tcmd="ifconfig | grep -Eo 'inet (addr:)?([0-9]*\\.){3}[0-9]*' | grep -Eo '([0-9]*\\.){3}[0-9]*' | grep -v '127.0.0.1'";
                file=popen(tcmd.c_str(),"r");
                char buf[100];
                fgets(buf,100,file);
                pclose(file);
                string respStr(buf);
                for(char& c:respStr) if(c=='.'||c=='\n') c=',';
                respStr+=std::to_string(random_number)+","+std::to_string(random_number1)+")\r\n";
                cout<<"cmdPort.cpp 61:"<<respStr<<endl;
                pasvConnect();
                resPond("227 Entering Passive Mode ("+respStr);
            }
            
        }

};
