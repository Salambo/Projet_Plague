#include "src/server.h"

int main(){
    pid_t pid;
    int anonymous_pipe[2];
    City *shared_memory;

    if(pipe(anonymous_pipe)  == 1){
        printf("Error creating pipe. \n");
    }

    /*Création de la mémoire partagée*/ 
    int shmd = shm_open("/city", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
    if(shmd == -1){
        printf("ça marche pas!");
    }
    if(ftruncate(shmd, sizeof(struct City))== -1){
        printf("ça marche pas bis");
    }
    shared_memory = mmap(NULL, sizeof(struct City), PROT_READ|PROT_WRITE, MAP_SHARED, shmd, 0);

    pid = fork();

    if(pid < 0){
       printf("Error using fork(). \n"); 
   
    } else if(pid == 0){ /*fils*/
        manage_child(anonymous_pipe, shared_memory);
    } else {
        manage_parent(anonymous_pipe, shared_memory);
    }

    /*création du fils 2/processus 3 ?*/


    /*sortie de la fonction*/
    munmap(server, sizeof(City));
    close(shmd);
    shm_unlink("/city");
}