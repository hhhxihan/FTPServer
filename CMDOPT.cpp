#include "CMDOPT.h"

void CMDOPT::processCMD(string cmd,string msg){ //处理PORT命令
        
        vector<string> v;
        string s="";
        for(char i: msg){
            if(i==' '||i==','){
                v.push_back(s);
                s="";
            }
            s.push_back(i);
        }
        resPond("200 always in UTF8 mode!\r\n");
        
    }        