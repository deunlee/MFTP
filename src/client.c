#include "client.h"

#include <libgen.h>

thread_info_c threads[MAX_CLIENTS];

int receive_file(char* file_path, int bar_index) {
    #define RETURN(n) { if (fp) fclose(fp); close(sockfd); free(buffer); return n; }

    int sockfd;
    struct sockaddr_in server_addr;
    size_t size, total_size, received_size = 0;
    char* buffer = malloc(BUFFER_SIZE);
    FILE* fp = NULL;

    sprintf(buffer, "(Info) Connecting to server... (%s:%d)", ip, port);
    bar_message(bar_index, buffer);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        bar_message(bar_index, "(Error) Cannot connect to the server.");
        free(buffer);
        return 1;
    }
    bar_message(bar_index, "(Info) Connected to the server.");
    usleep(500 * 1000); // 500ms

    // Send length of file path and file path to the server.
    size = strlen(file_path) + 1;
    if (send(sockfd, &size, sizeof(size), 0) == -1) {
        bar_message(bar_index, "(Error) Failed to send length of file name.");
        RETURN(1);
    }
    if (send(sockfd, file_path, size, 0) == -1) {
        bar_message(bar_index, "(Error) Failed to send file name.");
        RETURN(1);
    }

    // Open the file with write mode.
    fp = fopen(basename(file_path), "wb");
    if (! fp) {
        bar_message(bar_index, "(Error) Failed to create the file.");
        RETURN(1);
    }

    // Receive file size to show a progress bar.
    if (recv(sockfd, &total_size, sizeof(total_size), MSG_WAITALL) != sizeof(total_size)) {
        bar_message(bar_index, "(Error) Failed to receive file size.");
        RETURN(1);
    }
    sprintf(buffer, "(Info) File size is %ld bytes.", total_size);
    bar_message(bar_index, buffer); // Show a message instead of bar for 1 second.
    usleep(1000 * 1000); // 1000ms
    bar_config(bar_index, total_size, basename(file_path));
    bar_message(bar_index, NULL); // Clear message to show bar.

    // Notify the server that the file is ready to be downloaded.
    send(sockfd, &sockfd, 1, 0); // just one byte

    // Download and save the file.
    while ((size = recv(sockfd, buffer, BUFFER_SIZE, 0)) > 0) {
        received_size += size;
        bar_add(bar_index, size);
        fwrite(buffer, size, 1, fp);
    }

    // If recv() returns -1 or the file is not received to the end, print error message. 
    if (size == -1 || total_size != received_size) {
        bar_message(bar_index, "(Error) Failed to receive file data.");
        RETURN(1);
    }

    RETURN(0);
}

void* receive_file_by_thread(void* arg) {
    thread_info_c* info = (thread_info_c*)arg;
    receive_file(info->file_path, info->bar_index);
    info->is_finished = true;
    pthread_exit(0);
}

int run_client() {
    int i;

    if (file_cnt < 1) {
        printf("[Error] Please specify the file to be downloaded.\n");
        return 1;
    } else if (file_cnt > MAX_CLIENTS) {
        printf("[Error] Too many files. Up to %d files are supported.\n", MAX_CLIENTS);
        return 1;
    }
    if (! force_overwriting) {
        for (i = 0; i < file_cnt; i++) {
            if (access(basename(file_path[i]), F_OK) == 0) { // If a file already exists:
                printf("[Error] The file already exists. (%s)\n", basename(file_path[i]));
                return 1;
            }
        }
    }

    bzero(&threads, sizeof(threads));
    bar_init(file_cnt);
    printf("[Client] Start downloading %d file(s).\n", file_cnt);

    for (i = 0; i < file_cnt; i++) { // Create threads.
        strncpy(threads[i].file_path, file_path[i], MAX_PATH_LENGTH);
        threads[i].file_path[MAX_PATH_LENGTH - 1] = '\0';
        threads[i].bar_index = i;
        if (pthread_create(&threads[i].tid, NULL, receive_file_by_thread, &threads[i]) != 0) {
            perror("[Error] Failed to create a thread");
            return 1;
        }
        pthread_detach(threads[i].tid);
        printf("\n");
    }
    printf("\n");

    while (1) {
        bar_print(); // Print all progress bars.
        usleep(300 * 1000); // 300ms

        for (i = 0; i < file_cnt; i++) {
            if (! threads[i].is_finished) break;
        }
        if (i == file_cnt) {
            break; // Exit when all download tasks are finished.
        }
    }

    bar_print();
    printf("\n[Client] Finished!\n");
    return 0;
}
