#include "FTPTask.h"
#include <vector>
using namespace std;


class CMDOPT:public FTPTask{
    public:
        void processCMD(string cmd,string msg);
};
