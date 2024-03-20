#include "CMDOPT.h"

void CMDOPT::processCMD(string cmd,string msg){ //处理PORT命令
    int pos=msg.find(" ");
    msg=msg.substr(pos+1,msg.size()-pos-3); //去掉末尾的\r\n
    
    if(cmd=="SYST"){

        resPond("215 UNIX Type: L8\r\n");
    }else if(cmd=="FEAT"){
        resPond("211-Features:\r\n211 End\r\n");
    }
    // resPond("200 always in UTF8 mode!\r\n");
    
}        