#include "server.h"

int day = 0;
int nb_citizens_left = NUM_CITIZENS;
int current_citizen_index = 0;
pthread_mutex_t thread_mutex;
pthread_cond_t thread_signal;

int generate_citizens(City* city) {
    pthread_t thread_server;
    pthread_attr_t attr;
    int length;
    int width;

    /* Initialize mutex and condition variable objects */
	pthread_mutex_init(&thread_mutex, NULL);
	pthread_cond_init(&thread_signal, NULL);

    /* For portability, explicitly create threads in a joinable state */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_create(&thread_server, &attr, server, (void*)city);
    for(int i = 0; i < NUM_CITIZENS; i++) {
        int is_placed = 0;

        if(i < 6) { // 6 pompiers, dont 2 placés sur les casernes
            city->citizens[i].type = FIREMAN;
            if(i == 0) {
                city->citizens[i].position_x = 0;
                city->citizens[i].position_y = 6;
                city->terrain[0][6].people_number++;
                is_placed = 1;
            } else if(i == 1) {
                city->citizens[i].position_x = 6;
                city->citizens[i].position_y = 0;
                city->terrain[6][0].people_number++;
                is_placed = 1;
            }
        } else if(i >= 6 && i < 10) {
            city->citizens[i].type = DOCTOR;
            if(i == 6) {
                city->citizens[i].position_x = 3;
                city->citizens[i].position_y = 3;
                city->terrain[3][3].people_number++;
                is_placed = 1;
            }
        } else if(i == 10 || i == 11) {
            city->citizens[i].type = JOURNALIST;
        } else {
            city->citizens[i].type = CITIZEN;
        }

        city->citizens[i].contamination_level = 0;
        city->citizens[i].malade = 0;
        city->citizens[i].dead = 0;
        city->citizens[i].to_remove = 0;
        if(!is_placed) { // Les premiers citoyens sont les 6 pompiers, 4 médecins, 2 journalistes, total 12 citoyens spéciaux --- Les suivants sont des citoyens normaux
            do { // Positionnement aléatoire
                length = rand()%(6);
                width = rand()%(6);
            
                city->citizens[i].position_x = length;
                city->citizens[i].position_y = width;
            } while(city->terrain[length][width].capacity_max <= city->terrain[length][width].people_number);
            city->terrain[length][width].people_number++;
        }

        pthread_create(&city->citizens[i].thread_id, &attr, citizen, (void*)city);
    }

    pthread_join(thread_server, NULL);
    for(int i = 0; i < NUM_CITIZENS; i++) {
        pthread_join(&city->citizens[i].thread_id, NULL);
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

		if (current_citizen_index <= nb_citizens_left) {
		    current_citizen_index++;
		
            for(int i = 0; i < nb_citizens_left; i++) {
                if(city->citizens[i].thread_id == pthread_self() && city->citizens[i].to_remove == 0) {
                    if(rand_between_a_b(1, 101) > 60) {
                        //Citoyen bouge, gagne 2% de contamination de la case sur laquelle il va
                        int old_x = city->citizens[i].position_x;
                        int old_y = city->citizens[i].position_y;

                        Coord new_coord = newPlacement(old_x, old_y);


                        // Le citoyen gagne 2% du niveau de contamination de la case
                        if(city->citizens[i].contamination_level < 1) {
                            city->citizens[i].contamination_level += city->terrain[new_coord.x][new_coord.y].contamination_level * 0.02;
                        }

                        // La case gagne 1% du niveau de contamination du citoyen qui arrive
                        if(city->terrain[new_coord.x][new_coord.y].contamination_level < 1) {
                            city->terrain[new_coord.x][new_coord.y].contamination_level += city->citizens[i].contamination_level * 0.01;
                        }

                        if(city->citizens[i].malade == 0) { // Si non malade, gérer proba de tomber malade
                            if(rand_between_a_b(1, 101) < city->citizens[i].contamination_level*100) { // Probabilité de tomber malade
                                city->citizens[i].malade++;
                            }
                        } else if(city->citizens[i].malade <= 5) { // Jours de maladie sans probabilité de décéder
                            city->citizens[i].malade++;
                            for(int j = 0; j < nb_citizens_left; j++) { // 10% de probabilité de contaminer les autres gens sur la même case et 1% de probabilité de contaminer les gens sur les terrains vagues autour si le terrain actuelle est un terrain vague
                                if(city->citizens[j].position_x == new_coord.x && city->citizens[j].position_y < new_coord.y &&
                                    city->citizens[j].thread_id != pthread_self() &&
                                    city->citizens[j].dead == 0) { // Le citoyen est sur la même case, n'est pas mort et n'est pas le citoyen actuel
                                    if(rand_between_a_b(1, 101) < 1) { 
                                        city->citizens[j].malade = 1;
                                    }
                                } else if(city->terrain[new_coord.x][new_coord.y].type == WASTELAND &&city->citizens[j].position_x > new_coord.x - 1 && city->citizens[j].position_x < new_coord.x + 1 &&
                                        city->citizens[j].position_y > new_coord.y - 1 && city->citizens[j].position_y < new_coord.y + 1 &&
                                        city->citizens[j].dead == 0) { // Le terrain actuel est un terrain vague, le citoyen est auteur et sur un terrain vague et n'est pas mort
                                    if(rand_between_a_b(1, 101) < 11) { 
                                        city->citizens[j].malade = 1;
                                    }
                                }
                            }
                        } else { // Jours de maladie avec probabilité de décéder
                            if(rand_between_a_b(1, 101) < (city->citizens[i].malade - 5) * 0.05) { // Le citoyen meurt de la maladie
                                city->citizens[i].dead = 1;
                            }
                        }

                        // Mise à jour données de position du citoyen sur la grille
                        city->citizens[i].position_x = new_coord.x;
                        city->citizens[i].position_y = new_coord.y;
                        city->terrain[old_x][old_y].people_number--;
                        city->terrain[new_coord.x][new_coord.y].people_number++;
                    } else {
                        //Citoyen reste sur place, gagne 5% de contamination
                        int x = city->citizens[i].position_x;
                        int y = city->citizens[i].position_y;

                        // Le citoyen gagne 5% du niveau de contamination de la case
                        city->citizens[i].contamination_level += city->terrain[x][y].contamination_level * 0.05;
                    }

                    i = nb_citizens_left;
                }
            }
		}

        pthread_cond_signal(&thread_signal);
		pthread_mutex_unlock(&thread_mutex);
        usleep(500);
        while(current_citizen_index >= nb_citizens_left);
	}

	pthread_exit(NULL);
}

void *server(void *plug)
{
	City *city = (City*)plug;

    pthread_mutex_lock(&thread_mutex);
    while(day < NUM_DAYS) {
        day++;
        while (current_citizen_index < nb_citizens_left) {
            pthread_cond_wait(&thread_signal, &thread_mutex);
        }
        current_citizen_index = 0;
        printf("jour : %d\n", day);
        building_population_display(city->terrain);
        //show_citoyen_contamination_level(city);
        printf("Appuyez sur une touche pour passer au jour suivant\n");
        getchar();
        /**
         * Envoyer un SIGNAL vers le fils d'affichage ici pour afficher l'évolution à chaque tour
         */

        pthread_mutex_unlock(&thread_mutex);
    }
	pthread_exit(NULL);
}

int exist_medecin_on_case(City *shared_memory) {
    // Rien pour le moment
}

void show_citoyen_contamination_level(City *shared_memory) {
    for(int i = 0; i < nb_citizens_left; i++) {
        printf("Contamination level %d : %lf\n", i, shared_memory->citizens[i].contamination_level);
    }
}


Coord newPlacement(int x, int y) {
    Coord coord = {x, y};
    int found = 0;

    while(!found) {
        int valeur = rand_between_a_b(1, 9);
        switch(valeur) {
            case 1: //Haut-Gauche
                if(x > 0 && y > 0) {
                    coord.x -= 1;
                    coord.y -= 1;
                    found = 1;
                    break;
                }

            case 2: //Haut
                if(y > 0) {
                    coord.y -= 1;
                    found = 1;
                    break;
                }

            case 3: //Haut-Droite
                if(x < CITY_SIZE-1 && y > 0) {
                    coord.x += 1;
                    coord.y -= 1;
                    found = 1;
                    break;
                }

            case 4: //Gauche
                if(x > 0) {
                    coord.x -= 1;
                    found = 1;
                    break;
                }

            case 5: //Droite
                if(x < CITY_SIZE-1) {
                    coord.x += 1;
                    found = 1;
                    break;
                }

            case 6: //Bas-Gauche
                if(x > 0 && y < CITY_SIZE-1) {
                    coord.x -= 1;
                    coord.y += 1;
                    found = 1;
                    break;
                }

            case 7: //Bas
                if(y < CITY_SIZE-1) {
                    coord.y += 1;
                    found = 1;
                    break;
                }

            case 8: //Bas-Droite
                if(x > CITY_SIZE-1 && y < CITY_SIZE-1) {
                    coord.x += 1;
                    coord.y += 1;
                    found = 1;
                    break;
                }
        }
    }

    return coord;
}
