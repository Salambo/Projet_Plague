#include "server.h"

int manage_parent(int pipe[], City *shared_memory){
    /*Initialisation ville*/
    /*= MemoryAllocationCity(); /*création du tableau city[7][7]*/

    Building city[7][7];
    if(CityInitialization(city) == EXIT_FAILURE) {
        printf("Erreur lors de l'initialisation de la ville");
        return EXIT_FAILURE;
    }


    /*test*/
    building_type_display(city);

    shared_memory->terrain = city;

    /*Initialisation des niveaux de contamination des terrains*/
    

    /*Création des tubes nommés/files de messages pour les clients/autres programmes*/



    wait(NULL); /*Doit attendre que son fils/processus 2 soit mort*/

    /*Placement des citoyens*/
    printf("Fin initialisation. \n");

    /*Simulation : 100 tours*/
}

void manage_child(int pipe[], City *shared_memory){
    
    /*Initialisation threads - citoyen */

    /*fin fils 1/processus 2*/

    int shmd = shm_open("/city", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
    if(shmd == -1){
        printf("ça marche pas!");
    }
    if(ftruncate(shmd, sizeof(struct City))== -1){
        printf("ça marche pas bis");
    }
    shared_memory = mmap(NULL, sizeof(struct City), PROT_READ|PROT_WRITE, MAP_SHARED, shmd, 0);

    generate_citizens(shared_memory);
}

int CityInitialization(Building city[7][7]){
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
    
    for(length=0; length<7; length++){
        for(width=0; width<7; width++){
            city[length][width].type= 0;
        }
    }

    city[3][3].type= 3; /*Création de l'hopital*/
    city[0][6].type= 2; /*Création des casernes*/
    city[6][0].type= 2;

    for(var=0; var<12; var++){ /*Créations des maisons*/
        do{
            i=rand()%(6);
            j=rand()%(6);
        }while(city[i][j].type!=0);
        city[i][j].type= 1;
    }

    return EXIT_SUCCESS;
}

void building_type_display(Building city[7][7]){
    printf("ça marche\n");
    int length;
    int width;
    
    for(length=0; length<7; length++){
        for(width=0; width<7; width++){
            printf("%i ", city[length][width].type);
        }
        printf("\n");
    }
}