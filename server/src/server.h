#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct Citizen {
    int type;
    double contamination_level;
    int position_x;
    int position_y;
    pthread_t thread_id;
} Citizen;

typedef struct Building{

    int type;
    int people_number;
    int dead_body_number;
    int capacity_max; /*utile?*/
    double contamination_level;

} Building;

typedef struct City {
    Building** terrain;
    Citizen* citizens;
} City;

typedef struct thread_plug {
    Citizen* citizen;
    City* city;
} thread_plug;


int CityInitialization(Building[7][7]);
void building_type_display(Building[7][7]);

