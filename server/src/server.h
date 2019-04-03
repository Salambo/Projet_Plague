#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

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
    Citizen* citizens;

} Building;

typedef struct City {
    Building** terrain;
    Citizen* citizens;
} City;

typedef struct thread_plug {
    Citizen* citizen;
    City* city;
} thread_plug;

/**
 * Génération des citoyens et placement sur la grille
 * @param City* city
 * @param int nb_citizens
 */
int generate_citizens(City* city);

void *citizen(void *plug);
void *server(void *plug);

int manage_parent(int pipe[], City *shared_memory);
void manage_child(int pipe[], City *shared_memory);

int CityInitialization(Building[7][7]);
void building_type_display(Building[7][7]);

