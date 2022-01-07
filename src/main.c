#include "config.h"
#include "server.h"
#include "client.h"

// Common Options
char* ip    = DEFAULT_CLIENT_IP;
int   port  = DEFAULT_PORT;
int   mode  = MODE_NONE;
bool  debug = false;

// Server Options
int accept_mode = USE_MULTI_THREAD;

// Client Options
bool   force_overwriting = false;
int    file_cnt  = 0;
char** file_path = NULL;

void print_usage() {
    printf("  __  __ ______ _______ _____  \n");
    printf(" |  \\/  |  ____|__   __|  __ \\ \n");
    printf(" | \\  / | |__     | |  | |__) |\n");
    printf(" | |\\/| |  __|    | |  |  ___/ \n");
    printf(" | |  | | |       | |  | |     \n");
    printf(" |_|  |_|_|       |_|  |_|     \n\n");
    printf("MFTP: Multiple File Transfer Program by Deun Lee\n\n");

    printf("Usage: mftp -s      [-p port] [options]\n");
    printf("       mftp -c host [-p port] [options] files\n");

    printf("\nCommon Options: \n");
    printf("  -p, --port      #         server port to listen on/connect to\n");
    printf("                            (default port: %d)\n", DEFAULT_PORT);
    printf("  -v, --version             show version and quit\n");
    printf("  -h, --help                show usage message and quit\n");
    printf("  -d, --debug               print debug messages\n");

    printf("\nServer Options: \n");
    printf("  -s, --server              run in server mode\n");
    printf("  -b, --bind      <ip>      bind ip address (default: %s)\n", DEFAULT_SERVER_IP);
    printf("      --fork                use multi-process to accept clients\n");
    printf("      --thread              use multi-thread to accept clients (default)\n");

    printf("\nClient Options: \n");
    printf("  -c, --client    <host>    run in client mode, connecting to <host>\n");
    printf("  -f, --force               force overwriting of files when they exist\n");
    printf("\n");
}

int main(int argc, char* argv[]) {
    #define ARG_ERROR      { print_usage(); exit(1); }
    #define CHECK_ARG(str) (strcmp(str, argv[i]) == 0)
    #define NEED_ARG       if (++i == argc) ARG_ERROR;

    int i;
    for (i = 1; i < argc; i++) {
        if (argv[i][0] != '-') break;

        // Common Options
        if (CHECK_ARG("-p") || CHECK_ARG("--port")) {
            NEED_ARG;
            port = atoi(argv[i]);
        } else if (CHECK_ARG("-v") || CHECK_ARG("--version")) {
            printf("MFTP v.%s\n", VERSION);
            return 0;
        } else if (CHECK_ARG("-h") || CHECK_ARG("--help")) {
            print_usage();
            return 0;
        } else if (CHECK_ARG("-d") || CHECK_ARG("--debug")) {
            debug = true;
        }

        // Server Options
        else if (CHECK_ARG("-s") || CHECK_ARG("--server")) {
            if (mode == MODE_CLIENT) ARG_ERROR;
            mode = MODE_SERVER;
        } else if (CHECK_ARG("-b") || CHECK_ARG("--bind")) {
            NEED_ARG;
            ip = argv[i];
        } else if (CHECK_ARG("--fork")) {
            accept_mode = USE_MULTI_PROCESS;
        } else if (CHECK_ARG("--thread")) {
            accept_mode = USE_MULTI_THREAD;
        }

        // Client Options
        else if (CHECK_ARG("-c") || CHECK_ARG("--client")) {
            NEED_ARG;
            ip = argv[i];
            if (mode == MODE_SERVER) ARG_ERROR;
            mode = MODE_CLIENT;
        } else if (CHECK_ARG("-f") || CHECK_ARG("--force")) {
            force_overwriting = 1;
        }
    }

    if (mode == MODE_SERVER) {
        return run_server();
    } else if (mode == MODE_CLIENT) {
        file_cnt  = argc - i;
        file_path = &argv[i];
        return run_client();
    } else {
        ARG_ERROR;
    }

    return 0;
}

/*
mkdir -p bin
gcc main.c -o bin/main.o -c
gcc server.c -o bin/server.o -c
gcc client.c -o bin/client.o -c
gcc bar.c -o bin/bar.o -c
gcc -pthread -o mftp bin/main.o bin/server.o bin/client.o bin/bar.o
*/
