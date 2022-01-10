#include "server.h"

#include <signal.h>
#include <errno.h>
#include <sys/wait.h>

void print_prefix() {
    if (accept_mode == USE_MULTI_PROCESS) {
        if (debug) {
            printf("[Child]  (PID=%d) ", getpid());
        } else {
            printf("[Child]  ");
        }
    } else if (accept_mode == USE_MULTI_THREAD) {
        if (debug) {
            printf("[Thread] (TID=%ld) ", pthread_self());
        } else {
            printf("[Thread] ");
        }
    }
}

int send_file(int sockfd, struct sockaddr_in* client_addr) {
    #define RETURN(n) { if (fp) fclose(fp); close(sockfd); free(buffer); return n; }

    size_t size, total_size;
    char* buffer = malloc(BUFFER_SIZE);
    FILE* fp = NULL;

    print_prefix();
    printf("Client is connected. (%s:%d)\n", inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port));

    // Receive length of file path and file path.
    if (recv(sockfd, &size, sizeof(size), MSG_WAITALL) == -1) {
        perror("[Error] Failed to receive length of file name");
        RETURN(1);
    }
    if (recv(sockfd, buffer, size, MSG_WAITALL) == -1) {
        perror("[Error] Failed to receive a file name");
        RETURN(1);
    }
    print_prefix();
    printf("Requested file: %s\n", buffer);

    // Open the file with read-only mode.
    fp = fopen(buffer, "rb"); 
    if (fp == NULL) {
        perror("[Error] Failed to open the file");
        RETURN(1);
    }
    
    // Send file size to show a progress bar on the client.
    fseek(fp, 0, SEEK_END);
    total_size = ftell(fp); // Get file size.
    fseek(fp, 0, SEEK_SET);
    if (send(sockfd, &total_size, sizeof(total_size), 0) == -1) { // Send file size.
        perror("[Error] Failed to send a file data");
        RETURN(1);
    }

    // Wait until the client is ready.
    recv(sockfd, buffer, 1, MSG_WAITALL);

    // Read the file and send it.
    while (size = fread(buffer, 1, BUFFER_SIZE, fp)) {
        if (send(sockfd, buffer, size, 0) == -1) {
            perror("[Error] Failed to send a file data");
            RETURN(1);
        }
    }
    print_prefix();
    printf("File transfer complete!\n");
    RETURN(0);
}

void* send_file_by_thread(void* arg) {
    thread_info_s* info = (thread_info_s*)arg;
    send_file(info->sockfd, &info->client_addr);
    free(info);
    pthread_exit(0);
}

void signal_handler(int signal) {
    pid_t pid;
    int status;
    if (debug) {
        psignal(signal, "[Server] Received signal");
    }
    if (signal == SIGCHLD) { // When the child process exits
        // To wait for any child process, use -1 as PID. Use WNOHANG flag for non-blocking mode.
        // If several children are terminated at the same time, 
        // SIGCHLD signal may be delivered less than the number of children, so while loop should be used.
        while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
            if (debug) {
                printf("[Server] (PID=%d) Child returned: %d\n", pid, WEXITSTATUS(status));
            }
        }
    }
}

int run_server() {
    int sockfd, sockfd_accept, option = 1;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_size;
    thread_info_s* info;
    pid_t pid;
    struct sigaction act;

    act.sa_flags   = 0;
    act.sa_handler = signal_handler;
    sigaction(SIGCHLD, &act, NULL); // Register signal for child process return handling.

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    // Enable socket reuse. (Ignore address already in use error)
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("[Error] Cannot bind a socket");
        return 1;
    }

    if (accept_mode == USE_MULTI_PROCESS) {
        printf("[Server] Use multi-process to accept clients.\n");
    } else if (accept_mode == USE_MULTI_THREAD) {
        printf("[Server] Use multi-thread to accept clients.\n");
    }

    listen(sockfd, MAX_CLIENTS);
    printf("[Server] Server is running at %s:%d\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));

    while (1) {
        client_addr_size = sizeof(client_addr);
        sockfd_accept = accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_size);
        if (sockfd_accept == -1) {
            if (errno == EINTR) {
                continue; // Retry when accpet() is interrupted by receiving a signal from the child.
            }
            perror("[Error] Failed accept a connection");
            return 1;
        }

        if (accept_mode == USE_MULTI_PROCESS) {
            pid = fork(); // Create a child process.
            if (pid == 0) {
                close(sockfd); // Since the listen socket is not used in child, close it.
                return send_file(sockfd_accept, &client_addr);
            } else if (pid < 0) {
                perror("[Error] Failed to create a child process");
                close(sockfd_accept);
                close(sockfd);
                return 1;
            }
            close(sockfd_accept);
        } else if (accept_mode == USE_MULTI_THREAD) {
            info = malloc(sizeof(thread_info_s));
            info->sockfd      = sockfd_accept;
            info->client_addr = client_addr;
            if (pthread_create(&info->tid, NULL, send_file_by_thread, info) != 0) { // Create a thread.
                perror("[Error] Failed to create a thread");
                close(sockfd_accept);
                close(sockfd);
                return 1;
            }
            pthread_detach(info->tid);
        }
    }

    return 0;
}
