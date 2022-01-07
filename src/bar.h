#ifndef __MFTP_PROGRESS_BAR__
#define __MFTP_PROGRESS_BAR__

#include "config.h"

#define BAR_LENGTH        50
#define BAR_TITLE_LENGTH  15

typedef struct {
    size_t total;
    size_t current;
    size_t delta;
    char   title  [BAR_TITLE_LENGTH + 1];
    char   message[BAR_LENGTH + 1];
} progress_bar;

void bar_init   (int count);
void bar_config (int index, size_t total, char* title);
void bar_message(int index, char* message);
void bar_add    (int index, size_t delta); // accumulate
void bar_print  (); // print all progress bars

#endif
