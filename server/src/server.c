#include "server.h"

int manage_parent(int pipe[], City *shared_memory, pid_t pid_child){
    /*Initialisation ville*/
    /*= MemoryAllocationCity(); /*création du tableau city[7][7]*/

    Building city[CITY_SIZE][CITY_SIZE];
    if(CityInitialization(shared_memory->terrain) == EXIT_FAILURE) {
        printf("Erreur lors de l'initialisation de la ville");
        return EXIT_FAILURE;
    }


    /*test*/
    /*building_type_display(city);*/

    building_type_display(shared_memory->terrain);

    /*Initialisation des niveaux de contamination des terrains*/
    

    /*Création des tubes nommés/files de messages pour les clients/autres programmes*/


    /*Placement des citoyens*/
    printf("Fin initialisation. \n");
    
    usleep(500);
    kill(pid_child, SIGUSR1);
    printf("signal envoyé au fils %d \n", pid_child);
    wait(NULL); /*Doit attendre que son fils/processus 2 soit mort*/

    printf("Le fils se termine\n");
    building_type_display(shared_memory->terrain);

    /*Simulation : 100 tours*/
}

void manage_child(int pipe[], City *shared_memory){
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGUSR1);
    sigprocmask(SIG_SETMASK, &sigset, NULL);

    sigwaitinfo(&sigset, NULL);
    printf("signal reçu\n");
    
    /*Initialisation threads - citoyen */

    /*fin fils 1/processus 2*/

    building_type_display(shared_memory->terrain);
    generate_citizens(shared_memory);
    printf("Je me termine en tant que fils\n");
}

int CityInitialization(Building city[CITY_SIZE][CITY_SIZE]){
    int length;
    int height;
    int var;
    int i;
    int j;
    /*
    Type: 0 = terrain vague capacité: 16
            1 = maison 6
            2 = caserne 8
            3 = Hopital 12
    */

    for(length = 0; length < CITY_SIZE; length ++){
        for(height = 0; height < CITY_SIZE; height ++){
            city[length][height].type= 0;
            city[length][height].contamination_level= 0;
            city[length][height].people_number = 0;
            city[length][height].dead_body_number = 0;
        }
    }

    city[3][3].type = HOSPITAL;
    city[0][6].type = FIRESTATION;
    city[6][0].type = FIRESTATION;

    for(var=0; var<12; var++){ /*Créations des maisons*/
        do{
            i=rand()%(CITY_SIZE);
            j=rand()%(CITY_SIZE);
        }while(city[i][j].type!=0);
        city[i][j].type = HOUSE;
    }
    
    for(var=0; var<3; var++){
        do{
            i=rand()%(CITY_SIZE);
            j=rand()%(CITY_SIZE);
        }while(city[i][j].type!=0);
        double niv_contamination = rand()%(20);
        niv_contamination = (niv_contamination+20)/100;
        city[i][j].contamination_level = niv_contamination;

    }

    for(length = 0; length < CITY_SIZE; length++){
        for(height = 0; height < CITY_SIZE; height++){
            if(city[length][height].type == 0){
                city[length][height].capacity_max = 16;
            }
            else if(city[length][height].type == 1){
                city[length][height].capacity_max = 6;
            }
            else if(city[length][height].type == 2){
                city[length][height].capacity_max = 8;
            }
            else{
                city[length][height].capacity_max = 12;
            }
        }
    }

    return EXIT_SUCCESS;
}

void building_type_display(Building city[CITY_SIZE][CITY_SIZE]){
    int length;
    int height;
    
    for(length=0; length < CITY_SIZE; length++){
        for(height=0; height < CITY_SIZE; height++){
            printf("%i ", city[length][height].contamination_level);
        }
        printf("\n");
    }
}

int rand_between_a_b(int a, int b){
    return rand()%(b-a) +a;
}

int CaseContamination(Building case, double niv_contamination){
    if(rand_between_a_b(0, 100) > 15 && case.type == 0){
        double niv_conta_plus = rand_between_a_b(1, 20)/100 * (niv_contamination - case.niv_contamination);
        case.niv_contamination = case.niv_contamination + niv_conta_plus;
    }
}

int CityContamination(Building city[CITY_SIZE][CITY_SIZE]){

    int length;
    int height;

    for(length = 0; length < CITY_SIZE; length++){
        for(height = 0; height < CITY_SIZE; height++){
            if(city[length][height].type == 0 && city[length][height].contamination_level > 0){
                if(length != 0){
                    CaseContamination(city[length-1][height], city[length][height].contamination_level);
                    if(height != 0){
                        /*fonction pour contaminer city[length-1][height-1]*/
                        CaseContamination(city[length-1][height-1], city[length][height].contamination_level);
                    }
                    if(height != 6){
                        /*fonction pour contaminer city[length-1][height+1]*/
                        CaseContamination(city[length-1][height+1], city[length][height].contamination_level);
                    }
                }
                if(length != 6){
                    /*fonction pour contaminer city[length+1][height]*/
                    CaseContamination(city[length+1][height], city[length][height].contamination_level);
                    if(height != 0){
                        /*fonction pour contaminer city[length+1][height-1]*/
                        CaseContamination(city[length+1][height-1], city[length][height].contamination_level);
                    }
                    if(height != 6){
                        /*fonction pour contaminer city[length+1][height+1]*/
                        CaseContamination(city[length+1][height+1], city[length][height].contamination_level);
                    }
                }
                if(height != 0){
                    /*fonction pour contaminer city[length][height-1]*/
                    CaseContamination(city[length][height-1], city[length][height].contamination_level);
                }
                if(height != 6){
                    /*fonction pour contaminer city[length][height+1]*/
                    CaseContamination(city[length][height+1], city[length][height].contamination_level);
                }

            }
        }
    }

    return EXIT_SUCCESS;
}