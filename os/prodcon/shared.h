#define _GNU_SOURCE
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define SEM_BUF_CNT_NAME_P "/sem-buf-cnt-p"
#define SEM_BUF_CNT_NAME_C "/sem-buf-cnt-c"
#define SEM_ONE_BUF "/sem-one-buf"
#define SHM_NAME "/shm_name"
#define BUFFER_SIZE 1024
#define FAILED 1
#define SUCCESS 0
