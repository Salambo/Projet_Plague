#include "server.h"

Building ** MemoryAllocationCity(){

  int i;

  Building ** city= (int **)malloc(sizeof(int *)*7);

  if(city==NULL){
    printf("Erreur d'allocation de mémoire \n");

  }
  else{
    for(i=0; i<7; i++){
      city[i]= (int *)malloc(sizeof(int)*hauteur);
    }
  }

  return city; 
}

Building ** CityInitialization(){

    

}