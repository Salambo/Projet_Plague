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
                city->citizens[i].position.x = 0;
                city->citizens[i].position.y = 6;
                city->terrain[0][6].people_number++;
                city->citizens[i].equipment = 10; // Pulvérisateur
                is_placed = 1;
            } else if(i == 1) {
                city->citizens[i].position.x = 6;
                city->citizens[i].position.y = 0;
                city->terrain[6][0].people_number++;
                city->citizens[i].equipment = 10; // Pulvérisateur
                is_placed = 1;
            } else {
                city->citizens[i].equipment = 5; // Pulvérisateur
            }
        } else if(i >= 6 && i < 10) {
            city->citizens[i].type = DOCTOR;
            if(i == 6) {
                city->citizens[i].position.x = 3;
                city->citizens[i].position.y = 3;
                city->terrain[3][3].people_number++;
                city->citizens[i].equipment = 10; // Pochettes de soin
                is_placed = 1;
            } else {
                city->citizens[i].equipment = 5; // Pochettes de soin
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
                length = rand_between_a_b(0, 8);
                width = rand_between_a_b(0, 8);
            
                city->citizens[i].position.x = length;
                city->citizens[i].position.y = width;
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
    pthread_t self = pthread_self();

	while(day < NUM_DAYS) {
		pthread_mutex_lock(&thread_mutex);

		if (current_citizen_index <= nb_citizens_left) {
		
            for(int i = 0; i < nb_citizens_left; i++) {
                if(city->citizens[i].thread_id == self && city->citizens[i].to_remove) { // Citoyens brulé, suppression du thread
                    /*pthread_cond_signal(&thread_signal);
		            pthread_mutex_unlock(&thread_mutex);
                    pthread_cancel(city->citizens[i].thread_id);*/
                    current_citizen_index++;
                } else if(city->citizens[i].thread_id == self && !city->citizens[i].to_remove) {
                    double contamination_from_citizen;
                    double contamination_from_terrain;
                    int old_x = city->citizens[i].position.x;
                    int old_y = city->citizens[i].position.y;
                    Coord new_coord;

                    current_citizen_index++;

                    if(city->citizens[i].dead == 0 && rand_between_a_b(1, 101) > 60) { // 40% de chance que le citoyen bouge
                        // Le citoyen gagne 2% du niveau de contamination de la case, la case gagne 1% du niveau de contamination du citoyen qui arrive
                        contamination_from_citizen = 0.01;
                        contamination_from_terrain = 0.02;
                        new_coord = newPlacement(old_x, old_y);

                        // Mise à jour données de position du citoyen sur la grille
                        city->citizens[i].position.x = new_coord.x;
                        city->citizens[i].position.y = new_coord.y;
                        city->terrain[old_x][old_y].people_number--;
                        city->terrain[new_coord.x][new_coord.y].people_number++;
                    } else { // 60% de chance que le citoyen reste sur place
                        // Le citoyen gagne 5% du niveau de contamination de la case, la case gagne 1% du niveau de contamination du citoyen
                        contamination_from_citizen = 0.01;
                        contamination_from_terrain = 0.05;
                        new_coord.x = old_x;
                        new_coord.y = old_y;
                    }

                    switch(city->citizens[i].type) {
                        case FIREMAN:
                            if(city->terrain[new_coord.x][new_coord.y].type == FIRESTATION) { // Le pompier arrive sur une caserne, il récupère ses charges pour le pulvérisateur
                                city->citizens[i].equipment = 10;
                            }
                            fireman_action(city, &city->citizens[i]);
                            break;
                        
                        case DOCTOR:
                            if(city->terrain[new_coord.x][new_coord.y].type == HOSPITAL) { // Le médecin arrive sur l'hôpital, il récupère ses pochettes de soin
                                    city->citizens[i].equipment = 10;
                            }
                            doctor_action(city, &city->citizens[i]);
                            break;

                        case JOURNALIST:
                            journalist_action(city, city->citizens[i]);
                            break;

                        case CITIZEN:
                            break;
                    }

                    switch(city->terrain[new_coord.x][new_coord.y].type) {
                        case FIRESTATION: // Perte de contamination 20% par jour sur caserne de pompiers
                            if(city->citizens[i].contamination_level - 0.20 < 0) {
                                city->citizens[i].contamination_level = 0;
                            } else {
                                city->citizens[i].contamination_level -= 0.20;
                            }
                            break;

                        case HOSPITAL: // Hôpital donc 1/4 de réparcution de la contamination
                            if(city->terrain[new_coord.x][new_coord.y].contamination_level < 1) {
                                city->terrain[new_coord.x][new_coord.y].contamination_level += city->citizens[i].contamination_level * (contamination_from_citizen * 0.25);
                            }
                            break;

                        default: // Terrain vague et maison
                            if(city->terrain[new_coord.x][new_coord.y].contamination_level < 1) {
                                city->terrain[new_coord.x][new_coord.y].contamination_level += city->citizens[i].contamination_level * contamination_from_citizen;
                            }
                            if(city->citizens[i].contamination_level < 1) {
                                if(city->citizens[i].type == FIREMAN) { // Pompier
                                    city->citizens[i].contamination_level += city->terrain[new_coord.x][new_coord.y].contamination_level * contamination_from_terrain * 0.01;
                                } else {
                                    city->citizens[i].contamination_level += city->terrain[new_coord.x][new_coord.y].contamination_level * contamination_from_terrain;
                                }
                            }
                            break;
                    }

                    if(city->citizens[i].dead == 0) {
                        if(city->citizens[i].malade == 0) { // Si non malade, gérer proba de tomber malade
                            if(rand_between_a_b(1, 101) < city->citizens[i].contamination_level*100) { // Probabilité de tomber malade
                                city->citizens[i].malade++;
                            }
                        } else if(city->citizens[i].malade <= 5) { // Jours de maladie sans probabilité de décéder
                            city->citizens[i].malade++;
                            for(int j = 0; j < nb_citizens_left; j++) { // 10% de probabilité de contaminer les autres gens sur la même case et 1% de probabilité de contaminer les gens sur les terrains vagues autour si le terrain actuelle est un terrain vague
                                if(city->citizens[j].position.x == new_coord.x && city->citizens[j].position.y < new_coord.y &&
                                    city->citizens[j].thread_id != self &&
                                    city->citizens[j].dead == 0) { // Le citoyen est sur la même case, n'est pas mort et n'est pas le citoyen actuel
                                    if(rand_between_a_b(1, 101) < 1) {
                                        if(city->citizens[j].type == FIREMAN) { // Si pompier, alors 70% de chance de ne pas le rendre malade
                                            if(rand_between_a_b(1, 101) > 70) {
                                                city->citizens[j].malade = 1;
                                            }
                                        } else {
                                            city->citizens[j].malade = 1;
                                        }
                                    }
                                } else if(city->terrain[new_coord.x][new_coord.y].type == WASTELAND &&city->citizens[j].position.x > new_coord.x - 1 && city->citizens[j].position.x < new_coord.x + 1 &&
                                        city->citizens[j].position.y > new_coord.y - 1 && city->citizens[j].position.y < new_coord.y + 1 &&
                                        city->citizens[j].dead == 0) { // Le terrain actuel est un terrain vague, le citoyen est auteur et sur un terrain vague et n'est pas mort
                                    if(rand_between_a_b(1, 101) < 11) { 
                                        city->citizens[j].malade = 1;
                                    }
                                }
                            }
                        } else { // Jours de maladie avec probabilité de décéder, probabilité réduite de moité si médecin sur la même case
                            /*if(rand_between_a_b(1, 101) < (city->citizens[i].malade - 5) * (0.05 * (exist_medecin_on_case(city, city->citizens[i]) == EXIT_SUCCESS ? 0.5 : 1))) { // Le citoyen meurt de la maladie
                                city->citizens[i].dead = 1;
                            }*/

                            double proba_death;
                            double random = (double)rand_between_a_b(1, 101)/100;

                            if(exist_medecin_on_case(city, city->citizens[i]) == EXIT_SUCCESS) {
                                proba_death = (city->citizens[i].malade - 5) * 0.05 * 0.5;
                            } else {
                                proba_death = (city->citizens[i].malade - 5) * 0.05;
                            }
                            
                            if(random < proba_death) {
                                city->citizens[i].dead = 1;
                            }

                            city->citizens[i].malade++;
                        }
                    }

                    i = nb_citizens_left;
                }
            }
		}

        pthread_cond_signal(&thread_signal);
		pthread_mutex_unlock(&thread_mutex);
        usleep(500);
        //while(current_citizen_index >= nb_citizens_left);
	}

	pthread_exit(NULL);
}

int fireman_action(City *shared_memory, Citizen *fireman) {
    for(int i = 0; i < nb_citizens_left; i++) {
        if(shared_memory->citizens[i].position.x == fireman->position.x &&
            shared_memory->citizens[i].position.y == fireman->position.y) {

            if(shared_memory->citizens[i].dead == 1 && shared_memory->citizens[i].to_remove == 0) { // Brûle les morts
                shared_memory->citizens[i].to_remove = 1;
                shared_memory->terrain[fireman->position.x][fireman->position.y].people_number--;
                //nb_citizens_left--;
            } else if(shared_memory->citizens[i].contamination_level > 0 &&
                        fireman->equipment > 1 &&
                        shared_memory->citizens[i].thread_id != pthread_self()) { // Décontamine à hauteur de 20% de chaque citoyen présent sur la case, si ce n'est pas lui-même

                if(shared_memory->citizens[i].contamination_level - 0.20 < 0) {
                    shared_memory->citizens[i].contamination_level = 0;
                } else {
                    shared_memory->citizens[i].contamination_level -= 0.20;
                }

                fireman->equipment--;
            }
        }
    }

    if(fireman->equipment > 1) { // Décontamination du terrain sur lequel le pompier est
        if(shared_memory->terrain[fireman->position.x][fireman->position.x].contamination_level - 0.20 < 0) {
            shared_memory->terrain[fireman->position.x][fireman->position.x].contamination_level = 0;
        } else {
            shared_memory->terrain[fireman->position.x][fireman->position.x].contamination_level -= 0.20;
        }

        fireman->equipment--;
    }
}

int doctor_action(City *shared_memory, Citizen *doctor) {
    int index_sickest_citizen  = -1;

    if(shared_memory->terrain[doctor->position.x][doctor->position.y].type == HOSPITAL) { // Si dans hôpital, pas besoin d'utiliser pochette de soin
        if(doctor->malade > 0 && doctor->malade < 10) {
            for(int i = 0; i < nb_citizens_left; i++) {
                if(shared_memory->citizens[i].thread_id == pthread_self()) {
                    shared_memory->citizens[i].malade = 0;
                    return EXIT_SUCCESS;
                }
            }
        } else {
            for(int i = 0; i < nb_citizens_left; i++) { // Soigne le patient le plus malade
                if(shared_memory->citizens[i].position.x == doctor->position.x &&
                    shared_memory->citizens[i].position.y == doctor->position.y &&
                    shared_memory->citizens[i].thread_id != pthread_self()) {
                    
                    if(index_sickest_citizen == -1 || shared_memory->citizens[index_sickest_citizen].malade < shared_memory->citizens[i].malade) { // Récupérer le citoyen le plus malade de la case
                        index_sickest_citizen = i;
                    }
                }
            }

            shared_memory->citizens[index_sickest_citizen].malade = 0;
        }
    } else if(doctor->equipment > 0) { // Hors hôpital et si reste des pochettes de soin
        if(doctor->malade > 0 && doctor->malade < 10) {
            for(int i = 0; i < nb_citizens_left; i++) {
                if(shared_memory->citizens[i].thread_id == pthread_self()) {
                    shared_memory->citizens[i].malade = 0;
                    return EXIT_SUCCESS;
                }
            }
        } else {
            for(int i = 0; i < nb_citizens_left; i++) { // Soigne le patient le plus malade
                if(shared_memory->citizens[i].position.x == doctor->position.x &&
                    shared_memory->citizens[i].position.y == doctor->position.y &&
                    shared_memory->citizens[i].thread_id != pthread_self()) {
                    
                    if(index_sickest_citizen == -1 || shared_memory->citizens[index_sickest_citizen].malade < shared_memory->citizens[i].malade) { // Récupérer le citoyen le plus malade de la case
                        index_sickest_citizen = i;
                    }
                }
            }

            shared_memory->citizens[index_sickest_citizen].malade = 0;
        }

        doctor->equipment--;
    }
}

int journalist_action(City *shared_memory, Citizen fireman) {
    return EXIT_SUCCESS;
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
        //building_conta_display(city->terrain);
        
        if(city != NULL){
            
            if(CityContamination(city->terrain) == EXIT_FAILURE) {
                printf("Erreur lors de l'évolution de la contamination des terrains\n");
                return EXIT_FAILURE;
            }
        }
        
        printf("Appuyez sur une touche pour passer au jour suivant\n");
        getchar();
        /**
         * Envoyer un SIGNAL vers le fils d'affichage ici pour afficher l'évolution à chaque tour
         */
        
        pthread_mutex_unlock(&thread_mutex);
    }

    /*pthread_mutex_lock(&thread_mutex);
    show_dead_people(city->citizens);
    show_burn_people(city->citizens);
    show_sick_people(city->citizens);
    show_survivors(city->citizens);
    pthread_mutex_unlock(&thread_mutex);*/

	pthread_exit(NULL);
}

int exist_medecin_on_case(City *shared_memory, Citizen citizen) {
    for(int i = 0; i < nb_citizens_left; i++) {
        if(shared_memory->citizens[i].position.x == citizen.position.x &&
            shared_memory->citizens[i].position.y == citizen.position.y &&
            shared_memory->citizens[i].type == DOCTOR &&
            shared_memory->citizens[i].dead == 0) {

            return EXIT_SUCCESS;
        }
    }

    return EXIT_FAILURE;
}

int exist_fireman_on_case(City *shared_memory, Citizen citizen) {
    for(int i = 0; i < nb_citizens_left; i++) {
        if(shared_memory->citizens[i].position.x == citizen.position.x &&
            shared_memory->citizens[i].position.y == citizen.position.x &&
            shared_memory->citizens[i].type == FIREMAN) {

            return EXIT_SUCCESS;
        }
    }

    return EXIT_FAILURE;
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
