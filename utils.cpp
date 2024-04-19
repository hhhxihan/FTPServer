#include "utils.h"

int set_socket_blocking(int sockfd, int blocking) {
    if (sockfd < 0) return -1;

    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) return -1;

    if (blocking) {
        flags &= ~O_NONBLOCK; // 清除非阻塞标志
    } else {
        flags |= O_NONBLOCK; // 设置非阻塞标志
    }

    if (fcntl(sockfd, F_SETFL, flags) == -1) return -1;

    return 0;
}