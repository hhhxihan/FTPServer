
class Task{  //接口
    public:
        virtual int Init(struct event_base* tbase)=0;
};