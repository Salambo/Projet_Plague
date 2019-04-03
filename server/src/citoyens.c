#include "server.h"

int day = 0;
int nb_citizens_left = NUM_CITIZENS;
int current_citizen_index = 0;
pthread_mutex_t thread_mutex;
pthread_cond_t thread_signal;

int generate_citizens(City* city) {
    Citizen citizens[NUM_CITIZENS];
    pthread_t thread_server;
    long thread_id_server;
    //long thread_id_citizen[NUM_CITIZENS];
    pthread_attr_t attr;

    /* Initialize mutex and condition variable objects */
	pthread_mutex_init(&thread_mutex, NULL);
	pthread_cond_init(&thread_signal, NULL);

    /* For portability, explicitly create threads in a joinable state */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_create(&thread_server, &attr, server, (void*)thread_id_server);
    for(int i = 0; i < NUM_CITIZENS; i++) {
        citizens[i].type = i;
        citizens[i].contamination_level = 0;
        citizens[i].position_x = 0;
        citizens[i].position_y = 0;

        /*thread_plug *plug = (thread_plug*)malloc(sizeof(thread_plug));
        plug->shared_memory = citizens;
        plug->thread_id_citizen = i;*/

        //pthread_create(&citizens[i].thread_id, &attr, citizen, (void*)thread_id_citizen[i]);
        pthread_create(&citizens[i].thread_id, &attr, citizen, (void*)city);
    }

    /*Envoi des citizens dans la mémoire partagée*/
    city->citizens = citizens;

    pthread_join(thread_server, NULL);
    for(int i = 0; i < NUM_CITIZENS; i++) {
        pthread_join(citizens[i].thread_id, NULL);
    }

    pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&thread_mutex);
	pthread_cond_destroy(&thread_signal);
	pthread_exit(NULL);
}

void *citizen(void *plug)
{
	City *city = (City*)plug;

	while(day < NUM_DAYS) {
		pthread_mutex_lock(&thread_mutex);
		current_citizen_index++;

		if (current_citizen_index == nb_citizens_left) {
			pthread_cond_signal(&thread_signal);
		}
		
        for(int i = 0; i < nb_citizens_left; i++) {
            printf("je boucle\n");
            printf("je suis : %d", city->citizens[i].type);
            /*if(city->citizens[i].thread_id == pthread_self()) {
                printf("je suis %ld et je me suis trouvé\n", pthread_self());
                i = nb_citizens_left;
            }*/
        }

        //printf("thread id : %d\n", pthread_self());
        printf("jour : %d\n", day);
        printf("citoyen : %d\n", current_citizen_index);
		pthread_mutex_unlock(&thread_mutex);
        usleep(500);
	}
	pthread_exit(NULL);
}

void *server(void *plug)
{
	long id = (long)plug;

    pthread_mutex_lock(&thread_mutex);
    while(day < NUM_DAYS) {
        day++;
        while (current_citizen_index < nb_citizens_left) {
            pthread_cond_wait(&thread_signal, &thread_mutex);
        }
        current_citizen_index = 0;
        printf("Appuyez sur une touche pour passer au jour suivant");
        getchar();
        /**
         * Envoyer un SIGNAL vers le fils d'affichage ici pour afficher l'évolution à chaque tour
         */

        pthread_mutex_unlock(&thread_mutex);
    }
	pthread_exit(NULL);
}
