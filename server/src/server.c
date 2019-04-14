#include "server.h"

int manage_parent(City *shared_memory, pid_t pid_child){
    /*Initialisation ville*/
    if(CityInitialization(shared_memory->terrain) == EXIT_FAILURE) {
        perror("Erreur lors de l'initialisation de la ville\n");
        return EXIT_FAILURE;
    }

    /*Initialisation des niveaux de contamination des terrains*/
    

    /*Création des tubes nommés/files de messages pour les clients/autres programmes*/
    
    usleep(500);
    kill(pid_child, SIGUSR1);
    printf("signal envoyé au fils %d \n", pid_child);
    wait(NULL); /*Doit attendre que son fils/processus 2 soit mort*/

    printf("SIMULATION TERMINÉE\n");

    /*Simulation : 100 tours*/
    return EXIT_SUCCESS;
}

int manage_child(City *shared_memory){
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGUSR1);
    sigprocmask(SIG_SETMASK, &sigset, NULL);

    sigwaitinfo(&sigset, NULL);
    printf("signal reçu\n");
    
    /*Initialisation threads - citoyen */

    /*fin fils 1/processus 2*/

    generate_citizens(shared_memory);

    return EXIT_SUCCESS;
}

int CityInitialization(Building city[CITY_SIZE][CITY_SIZE]){
    int length;
    int width;
    int var;
    int i;
    int j;
    /*
    Type: 0 = terrain vague capacité: 16
            1 = maison 6
            2 = caserne 8
            3 = Hopital 12
    */

    for(length=0; length<CITY_SIZE; length++){
        for(width=0; width<CITY_SIZE; width++){
            city[length][width].type= 0;
            city[length][width].contamination_level= 0;
            city[length][width].people_number = 0;
        }
    }

    city[3][3].type = HOSPITAL;
    city[0][6].type = FIRESTATION;
    city[6][0].type = FIRESTATION;

    for(var=0; var<12; var++){ /*Créations des maisons*/
        do{
            i=rand()%(6);
            j=rand()%(6);
        }while(city[i][j].type!=0);
        city[i][j].type = HOUSE;
    }
    
    for(var=0; var<3; var++){
        do{
            i=rand()%(7);
            j=rand()%(7);
        }while(city[i][j].type!=0);
        double niv_contamination = rand()%(20);
        niv_contamination = (niv_contamination+20)/100;
        city[i][j].contamination_level = niv_contamination;

    }

    for(length=0; length<CITY_SIZE; length++) {
        for(width=0; width<CITY_SIZE; width++) {
            if(city[length][width].type == WASTELAND) {
                city[length][width].capacity_max = 16;
            } else if(city[length][width].type == HOUSE) {
                city[length][width].capacity_max = 6;
            } else if(city[length][width].type == FIRESTATION) {
                city[length][width].capacity_max = 8;
            } else {
                city[length][width].capacity_max = 12;
            }
        }
    }

    return EXIT_SUCCESS;
}

void updateHistory(City *shared_memory, FILE *file) {
    if(shared_memory == NULL) {
        perror("Mémoire partagée récuérée invalide\n");
        return;
    }

    if(file == NULL) {
        perror("Fichier invalide\n");
        return;
    }

    double average_contamination = 0;
    int i, j;
    int nb_other_citizen = 0;
    int nb_sick_citizen = 0;
    int nb_dead_citizen = 0;
    int nb_burn_citizen = 0;

    printf("Nombre citoyens /  Niveau de contamination:\n");
    for(i = 0; i < CITY_SIZE; i++) {
        for(j = 0; j < CITY_SIZE; j++) {
            average_contamination += shared_memory->terrain[i][j].contamination_level;
            printf("%d %.2lf      ", shared_memory->terrain[i][j].people_number, shared_memory->terrain[i][j].contamination_level);
        }
        printf("\n");
    }

    average_contamination /= CITY_SIZE*CITY_SIZE;
    fprintf(file, "%lf ", average_contamination);


    for(i = 0; i < NUM_CITIZENS; i++) {
        if(shared_memory->citizens[i].to_remove) {
            nb_burn_citizen++;
        } else if(shared_memory->citizens[i].dead) {
            nb_dead_citizen++;
        } else if(shared_memory->citizens[i].malade > 0) {
            nb_sick_citizen++;
        } else {
            nb_other_citizen++;
        }
    }

    printf("Population restant : \n\
                -  Citoyens brûlés : %d\n\
                - Citoyens morts : %d\n\
                - Citoyens malades : %d\n\
                - Citoyens en bonne santé : %d\n",
                nb_burn_citizen, nb_dead_citizen, nb_sick_citizen, nb_other_citizen);

    fprintf(file, "%d %d %d %d\n", nb_other_citizen, nb_sick_citizen, nb_dead_citizen, nb_burn_citizen);
}

int rand_between_a_b(int a, int b){
    return rand()%(b-a) +a;
}

double CaseContamination(Building casse, double niv_contamination){
    if(casse.contamination_level < niv_contamination && casse.type == 0){
        if(rand_between_a_b(0, 100) < 16){
            double niv_conta_plus = niv_contamination - casse.contamination_level;
            niv_conta_plus = (rand_between_a_b(1,20)*0.01)* niv_conta_plus;
            casse.contamination_level = casse.contamination_level + niv_conta_plus;
        }
    }
    return casse.contamination_level;
}

int CityContamination(Building city[CITY_SIZE][CITY_SIZE]){
    int length;
    int height;

    for(length = 0; length < CITY_SIZE; length++){
        for(height = 0; height < CITY_SIZE; height++){
            
            if(city[length][height].type == 0 && city[length][height].contamination_level > 0){
                if(length != 0){

                    city[length-1][height].contamination_level= CaseContamination(city[length-1][height], city[length][height].contamination_level);

                    if(height != 0){

                        /*fonction pour contaminer city[length-1][height-1]*/
                        city[length-1][height-1].contamination_level=CaseContamination(city[length-1][height-1], city[length][height].contamination_level);
                    }

                    if(height != 6){

                        /*fonction pour contaminer city[length-1][height+1]*/
                        city[length-1][height+1].contamination_level=CaseContamination(city[length-1][height+1], city[length][height].contamination_level);
                    }

                }

                if(length != 6){
                    /*fonction pour contaminer city[length+1][height]*/
                    city[length+1][height].contamination_level=CaseContamination(city[length+1][height], city[length][height].contamination_level);
                    if(height != 0){
                        /*fonction pour contaminer city[length+1][height-1]*/
                        city[length+1][height-1].contamination_level=CaseContamination(city[length+1][height-1], city[length][height].contamination_level);
                    }
                    if(height != 6){
                        /*fonction pour contaminer city[length+1][height+1]*/
                        city[length+1][height+1].contamination_level=CaseContamination(city[length+1][height+1], city[length][height].contamination_level);
                    }
                }
                if(height != 0){
                    /*fonction pour contaminer city[length][height-1]*/
                    city[length][height-1].contamination_level=CaseContamination(city[length][height-1], city[length][height].contamination_level);
                }
                if(height != 6){
                    /*fonction pour contaminer city[length][height+1]*/
                    city[length][height+1].contamination_level=CaseContamination(city[length][height+1], city[length][height].contamination_level);
                }
                
            }
        }
    }

    return EXIT_SUCCESS;
}