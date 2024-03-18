#include "CMDTYPE.h"

void CMDTYPE::processCMD(string cmd,string msg){
    int pos=msg.find(" ");
    msg=msg.substr(pos+1,msg.size()-pos-3); //去掉末尾的\r\n

    if(msg[0]=='A'){ //文本模式
        resPond("200 Switching to text mode.\r\n");

    }else if(msg[0]=='I'){ //二进制模式

        resPond("200 Switching to ASCII mode.\r\n");
    }
}