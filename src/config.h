#ifndef __MFTP_CONFIG__
#define __MFTP_CONFIG__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // strcmp(), strncpy()
#include <strings.h> // bzero()
#include <stdbool.h> // bool type
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define VERSION               "1.0.2"
#define MAX_CLIENTS                16
#define MAX_PATH_LENGTH          4096
#define BUFFER_SIZE              4096
#define DEFAULT_PORT            55555
#define DEFAULT_SERVER_IP   "0.0.0.0"
#define DEFAULT_CLIENT_IP "127.0.0.1"

// Common Options
#define MODE_NONE   0
#define MODE_SERVER 1
#define MODE_CLIENT 2
extern char* ip;
extern int   port;
extern int   mode;
extern bool  debug;

// Server Options
#define USE_MULTI_PROCESS 1
#define USE_MULTI_THREAD  2
extern int accept_mode;

// Client Options
extern bool   force_overwriting;
extern int    file_cnt;
extern char** file_path;

#endif
