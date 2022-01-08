#ifndef __MFTP_SERVER__
#define __MFTP_SERVER__

#include "config.h"

typedef struct {
    pthread_t          tid;
    int                sockfd;
    struct sockaddr_in client_addr;
} thread_info_s;

int run_server();

#endif
