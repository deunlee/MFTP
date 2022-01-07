#ifndef __MFTP_SERVER__
#define __MFTP_SERVER__

#include "config.h"

typedef struct {
    pthread_t tid;
    int       sockfd;
} thread_info_s;

int run_server();

#endif
