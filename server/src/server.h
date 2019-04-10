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
#include <signal.h>

#define NUM_CITIZENS	25+12 // 25 Citoyens normaux + 12 citoyens spéciaux
#define NUM_DAYS		100
#define CITY_SIZE       7

#define CITIZEN         1
#define FIREMAN         2
#define DOCTOR          3
#define JOURNALIST      4

#define WASTELAND       0
#define HOUSE           1
#define FIRESTATION     2
#define HOSPITAL        3

#define TRUE            1
#define FALSE           0

typedef struct Citizen {
    int type;
    double contamination_level;
    int position_x;
    int position_y;
    int malade;
    int dead;
    int to_remove;
    int equipment;
    int countdown_before_to_enter_hospital;
    pthread_t thread_id;
} Citizen;

typedef struct Building{
    int type;
    int people_number;
    int capacity_max; /*utile?*/
    double contamination_level;
} Building;

typedef struct City {
    Building terrain[CITY_SIZE][CITY_SIZE];
    Citizen citizens[NUM_CITIZENS];
} City;

typedef struct thread_plug {
    long *thread_id_citizen;
    City* shared_memory;
} thread_plug;

typedef struct Coord {
    int x;
    int y;
} Coord;

/**
 * Génération des citoyens et placement sur la grille
 * @param City* city
 * @param int nb_citizens
 */
int generate_citizens(City* city);

void *citizen(void *plug);
void *server(void *plug);

int manage_parent(int pipe[], City *shared_memory, pid_t pid_child);
void manage_child(int pipe[], City *shared_memory);

int CityInitialization(Building[CITY_SIZE][CITY_SIZE]);
int CityContamination(Building[CITY_SIZE][CITY_SIZE]);
void building_type_display(Building[CITY_SIZE][CITY_SIZE]);
void building_population_display(Building city[CITY_SIZE][CITY_SIZE]);
int rand_between_a_b(int a, int b);
Coord newPlacement(int x, int y);
void building_conta_display(Building[CITY_SIZE][CITY_SIZE]);
