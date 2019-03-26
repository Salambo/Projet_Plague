#include "server.h"
#include <time.h>

Building** MemoryAllocationCity(){

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
}

Building** CityInitialization(Building ** city){

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

    for(var=0; var<7; var++){
        do{
            i=rand_b(length-1);
            j=rand_b(width-1);
        }while(city[i][j].type!=0);
        city[i][j].type= 1;
    }

    return city;

}

void building_type_display(Building ** city){
    int length;
    int width;
    
    for(length=0; length<7; length++){
        for(width=0; width<7; width++){
            printf("%i ", city[length][width].type);
        }
        printf("\n");
    }
}