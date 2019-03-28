#include "server.h"

int generate_citizens(City* city, int nb_citizens) {
    Citizen* citizens;

    citizens = (Citizen*)malloc(sizeof(Citizen) * nb_citizens);

    for(int i = 0; i < nb_citizens; i++) {
        citizens[i].type = i;
        citizens[i].contamination_level = 0;
        citizens[i].position_x = 0;
        citizens[i].position_y = 0;

        thread_plug plug;
        plug.citizen = &citizens[i];
        plug.city = city;

        pthread_create(&citizens[i].thread_id, NULL, &live_citizen, (void*)&plug);
        printf("increment %d\n", i);
    }

    for(int i = 0; i < nb_citizens; i++) {
        pthread_join(citizens[i].thread_id, NULL);
    }

    exit(EXIT_SUCCESS);
}

void *live_citizen(void *citizen) {
    for(int i = 0; i < 1; i++) {
        printf("thread executé %ld\n", (long)pthread_self());
        thread_plug *plug = citizen;

        printf("variable dans thread : %d\n", plug->citizen->type);

        sleep(0.1);
    }

    //pthread_exit(EXIT_SUCCESS);
}
