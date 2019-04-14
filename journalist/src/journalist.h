#include <mqueue.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h> /*pour sleep*/ 


#define NUM_DAYS		3

typedef struct Message{
    int priority;
    char donnee;
} Message;