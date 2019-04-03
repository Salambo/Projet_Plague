#include "src/server.h"

void manage_parent(int pipe[]){
    
    /*Création de la mémoire partagée*/ 

    /*Initialisation ville*/
    Building ** city = MemoryAllocationCity(); /*création du tableau city[7][7]*/


    

    /*Création des tubes nommés/files de messages pour les clients/autres programmes*/



    wait(NULL); /*Doit attendre que son fils/processus 2 soit mort*/

    /*Placement des citoyens*/
    printf("Fin initialisation. \n");

    /*Simulation : 100 tours*/

    /*sortie de la fonction*/
}
/*envoyer un morceau par un morceau du tableau*/
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