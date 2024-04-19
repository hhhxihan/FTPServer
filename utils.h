#ifndef UTILS_H
#define UTILS_H
enum MODE{
    ACTIVEMODE=0,
    PASVMODE
};
#include <fcntl.h>
int set_socket_blocking(int sockfd, int blocking);


#endif