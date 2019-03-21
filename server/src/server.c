#include "server.h"

Building ** MemoryAllocationCity(){

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

Building ** CityInitialization(Building ** city){

    /*
    Type: 0 = terrain vague capacité: 16
            1 = maison 6
            2 = caserne 8
            3 = Hopital 12
    */
    
    return city;

}