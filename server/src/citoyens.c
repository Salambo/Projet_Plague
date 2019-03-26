#include "server.h"

int generate_citizens(City* city, int nb_citizens) {
    Citizen* citizens;

    citizens = (Citizen*)malloc(sizeof(Citizen) * nb_citizens);

    for(int i = 0; i < nb_citizens; i++) {
        citizens[i].type = 1;
        citizens[i].contamination_level = 0;
        citizens[i].position_x = 0;
        citizens[i].position_y = 0;

        thread_plug plug;
        plug.citizen = &citizens[i];
        plug.city = city;

        pthread_create(citizens[i].thread_id, NULL, live_citizen, (void*)&plug);
    }

    pthread_exit(EXIT_SUCCESS);
}

int *live_citizen(Citizen* citizen) {
    printf("thread executé %d", pthread_self());

    return EXIT_SUCCESS;
}
