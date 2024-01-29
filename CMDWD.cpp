#include "FTPTask.h"

class CMDWD:public FTPTask{
    public:
        void processCMD(string cmd,string msg){
            if (cmd=="CWD")
            {
                /* code */
            }
            else if(cmd=="PWD"){

            }
            else if(cmd=="CDUP"){

            }
            else if(cmd=="LIST"){
                
            }
            
        }
};