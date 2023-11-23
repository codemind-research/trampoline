#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <execinfo.h>

/*
#define log_writeln(str) fprintf(crichton_log,"%s\n",str); \
    fflush(crichton_log)
#define log_write(str) fprintf(crichton_log,"%s",str); \
    fflush(crichton_log)

#define log_write_time(str) print_time(crichton_log); \
    fprintf(crichton_log,"%s",str); \
    fflush(crichton_log)
*/
#ifdef POSIX
#define log_writeln(str) fprintf(stdout,"%s\r\n",str); \
    fflush(stdout)
#define log_write(str) fprintf(stdout,"%s",str); \
    fflush(stdout)

#define log_write_time(str) print_time(stdout); \
    fprintf(stdout,"%s",str); \
    fflush(stdout)
#else
#define log_writeln(str) trace_printf("%s\r\n",str); \
    fflush(stdout)
#define log_write(str) trace_printf("%s",str); \
    fflush(stdout)

#define log_write_time(str) print_time(stdout); \
    trace_printf("%s",str); \
    fflush(stdout)
#endif


typedef struct proc_info {
    int run_cnt;
    char *name;
} proc_info;

typedef struct bitflip {
    char *name;
    int mode;
} bitflip;

extern FILE* crichton_log;
extern proc_info* proc_log;

//extern proc_info proc_log[TASK_COUNT+ISR_COUNT];
void print_time(FILE *);

void crichton_free(void *);
void* crichton_malloc(size_t);

void printCallStack() {
    printf("ent1");
    void *callStack[100];
    int frames = backtrace(callStack, 100);

    printf("ent2");
    char **strs = backtrace_symbols(callStack, frames);
    printf("#frame: %d\n",frames);
    for (int i = 0; i < frames; i++) {
        printf("[%i] %s\n", frames - i - 1, strs[i]);
    }

    free(strs);
}
