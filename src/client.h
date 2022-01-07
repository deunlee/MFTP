#ifndef __MFTP_CLIENT__
#define __MFTP_CLIENT__

#include "config.h"
#include "bar.h"

typedef struct {
    pthread_t tid;
    bool      is_finished;
    char      file_path[MAX_PATH_LENGTH];
    int       bar_index;
} thread_info_c;

int run_client();

#endif
