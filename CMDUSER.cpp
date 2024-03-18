#include "CMDUSER.h"

void CMDUSER::processCMD(string cmd,string msg){
    int pos=msg.find(" ");
    msg=msg.substr(pos+1,msg.size()-pos-3); //去掉末尾的\r\n
    
    if(cmd=="USER"){
        cout<<"process USER CMD"<<endl;
        //now，msg is username

        //process username

        char buf[]="331 Password required for username.\r\n";
        resPond(buf);
    }
    else if(cmd=="PASS"){
        cout<<"process PASS CMD"<<endl;
        //now,msg is password

        char buf[]="230 Login success!\r\n";
        resPond(buf);
    }
    

}