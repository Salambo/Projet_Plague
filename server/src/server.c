#include "server.h"
#include <time.h>

/*Building** MemoryAllocationCity(){

    int i;

    Building ** city= (Building **)malloc(sizeof(Building *)*7);

    if(city==NULL){
        printf("Erreur d'allocation de mémoire \n");

    }
    else{
        for(i=0; i<7; i++){
            city[i]= (Building *)malloc(sizeof(Building)*7);
        }
    }

    return city; 
}*/
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
            city[length][width].contamination_level= 0;
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
    
    for(var=0; var<3; var++){
        do{
            i=rand()%(6);
            j=rand()%(6);
        }while(city[i][j].type!=0);
        double niv_contamination = rand()%(20);
        niv_contamination = (niv_contamination+20)/100;
        city[i][j].contamination_level=niv_contamination;

    }

    return EXIT_SUCCESS;
}

void building_type_display(Building city[7][7]){
    int length;
    int width;
    
    for(length=0; length<7; length++){
        for(width=0; width<7; width++){
            printf("%lf ", city[length][width].type);
        }
        printf("\n");
    }
}