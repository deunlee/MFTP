#include "bar.h"

#include <time.h>
#include <sys/time.h>

int            bar_cnt = 0;
progress_bar   bars[MAX_CLIENTS];
struct timeval bar_last_time;

time_t time_diff(struct timeval* a, struct timeval* b) { // returns (a - b)
    return (a->tv_sec - b->tv_sec) * 1000000 + (a->tv_usec - b->tv_usec);
}

void print_size_with_unit(double size) {
    if (size < 1024.0) { printf("%.0lf B", size); return; }
    size /= 1024.0;
    if (size < 1024.0) { printf("%.1f KB", size); return; }
    size /= 1024.0;
    if (size < 1024.0) { printf("%.1f MB", size); return; }
    size /= 1024.0;
    if (size < 1024.0) { printf("%.1f GB", size); return; }
    printf("%.1f TB", size);
}

void bar_init(int count) {
    bzero(&bars, sizeof(bars));
    bar_cnt = count;
    bar_last_time.tv_sec  = 0;
    bar_last_time.tv_usec = 0;
}

void bar_config(int index, size_t total, char* title) {
    bars[index].total = total;
    strncpy(bars[index].title, title, BAR_TITLE_LENGTH);
    bars[index].title[BAR_TITLE_LENGTH] = '\0';
}

void bar_message(int index, char* message) {
    if (message == NULL) {
        bars[index].message[0] = '\0';
    } else {
        strncpy(bars[index].message, message, BAR_LENGTH);
        bars[index].message[BAR_LENGTH] = '\0';
    }
}

void bar_add(int index, size_t delta) {
    bars[index].current += delta;
    bars[index].delta   += delta;
}

void bar_print() {
    int i, j, k;
    double per;
    struct timeval current_time;
    time_t diff;
    gettimeofday(&current_time, NULL);
    diff = time_diff(&current_time, &bar_last_time);
    printf("\033[%dA", bar_cnt); // move cursor
    for (i = 0; i < bar_cnt; i++) {
        if (bars[i].total) { // prevent division by zero
            per = (double)bars[i].current / bars[i].total;
        } else {
            per = 0.0;
        }
        printf("%3d%% ", (int)(per * 100.0));
        if (bars[i].message[0]) {
            printf("%s", bars[i].message);
            j = (BAR_LENGTH + BAR_TITLE_LENGTH + 15) - strlen(bars[i].message);
            for (k = 0; k < j; k++) printf(" ");
            printf("\n");
            continue;
        }
        j = per * BAR_LENGTH;
        printf("[");
        if (j) {
            for (k = 0; k < j - 1; k++) printf("=");
            printf(">");
        }
        for (k = 0; k < BAR_LENGTH - j; k++) printf(" ");
        printf("] %-15s ", bars[i].title);
        if (bars[i].total > 0) {
            if (bars[i].total == bars[i].current) {
                printf("(FINISHED)     \n"); // 10+5
            } else {
                printf("(");
                print_size_with_unit((double)bars[i].delta * 1000.0 / diff * 1000.0);
                printf("/s)     \n"); // 10+5
            }
        } else {
            printf("                \n");
        }
        bars[i].delta = 0;
    }
    bar_last_time = current_time;
    fflush(stdout);
}
