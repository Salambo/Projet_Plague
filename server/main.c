#include "src/server.h"

int main(){
    pid_t pid;
    int shmd;
    City *shared_memory;

    /*Création de la mémoire partagée*/ 
    shmd = shm_open(SHARED_MEMORY, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
    if(shmd == -1){
        perror("Erreur lors de la création de l'objet de mémoire partagée POSIX\n");
        return EXIT_FAILURE;
    }
    if(ftruncate(shmd, sizeof(City))== -1){
        perror("Erreur lors du redimensionnement de l'objet mémoire partagée\n");
        if(shm_unlink(SHARED_MEMORY) == -1) {
            perror("Impossible de supprimer l'objet de mémoire partagée\n");
            return EXIT_FAILURE;
        }
        return EXIT_FAILURE;
    }
    shared_memory = mmap(NULL, sizeof(City), PROT_READ|PROT_WRITE, MAP_SHARED, shmd, 0);

    if(shared_memory == NULL) {
        perror("Impossible de créer la zone mémoire partagée\n");
        close(shmd);
        shm_unlink(SHARED_MEMORY);
        return EXIT_FAILURE;
    }

    pid = fork();

    if(pid < 0){
       perror("Erreur lors la création du premier fils\n");
       return EXIT_FAILURE;
   
    } else if(pid == 0){ /* Fils */
        if(manage_child(shared_memory) == EXIT_FAILURE) {
            perror("La simulation s'est mal déroulée\n");
        }
    } else { /* Parent */
        if(manage_parent(shared_memory, pid)) {
            perror("La Génération s'est mal déroulée\n");
        }
    }

    /*création du fils 2/processus 3 ?*/


    /*sortie de la fonction*/
    munmap(server, sizeof(City));
    close(shmd);
    shm_unlink(SHARED_MEMORY);

    return EXIT_SUCCESS;
}