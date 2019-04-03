#include "src/server.h"

struct City *shared_memory;


int manage_parent(int pipe[]){

    /*Création de la mémoire partagée*/ 
    int shmd = shm_open("/city", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
    if(shmd == -1){
        printf("ça marche pas!");
    }
    if(ftruncate(shmd, sizeof(struct City))== -1){
        printf("ça marche pas bis");
    }
    shared_memory = mmap(NULL, sizeof(struct City), PROT_READ|PROT_WRITE, MAP_SHARED, shmd, 0);

    /*Initialisation ville*/
    /*= MemoryAllocationCity(); /*création du tableau city[7][7]*/

    Building city[7][7];
    if(CityInitialization(city) == EXIT_FAILURE) {
        printf("Erreur lors de l'initialisation de la ville");
        return EXIT_FAILURE;
    }


    /*test*/
    building_type_display(city);

    /*Initialisation des niveaux de contamination des terrains*/
    

    /*Création des tubes nommés/files de messages pour les clients/autres programmes*/



    wait(NULL); /*Doit attendre que son fils/processus 2 soit mort*/

    /*Placement des citoyens*/
    printf("Fin initialisation. \n");

    /*Simulation : 100 tours*/

    /*sortie de la fonction*/
    munmap(server, sizeof(City));
    close(shmd);
    shm_unlink("/city");
}

void manage_child(int pipe[]){
    
    /*Initialisation threads - citoyen */

    /*fin fils 1/processus 2*/

}

int main(){

   
    pid_t pid;
    int anonymous_pipe[2];
   
    if(pipe(anonymous_pipe)  == 1){
        printf("Error creating pipe. \n");
    }

    pid = fork();

    if(pid < 0){
       printf("Error using fork(). \n"); 
   
    } else if(pid == 0){ /*fils*/
        manage_child(anonymous_pipe);
    } else {
        manage_parent(anonymous_pipe);
    }

    /*création du fils 2/processus 3 ?*/



}