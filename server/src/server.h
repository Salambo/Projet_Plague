#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

typedef struct Building{

    int type;
    int people_number;
    int dead_body_number;
    int capacity_max; /*utile?*/
    double contamination_level;

} Building;