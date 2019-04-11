#include "journalist.h"

/*Type message: 1 nombre morts - int
                2 contamination moyenne de la ville - double
                3 nombres citoyens contaminés - int
                4 contamination du journaliste -double */
void traitement_message(int type_message, char *donnees){
    if(type_message == 5){
        double donnee = atof(donnees);
        donnee = donnee - donnee * 0.1;
        donnee = donnee * 100;
        if(donnee > 0){
            printf("La contamination moyenne de la ville est de %lf \n", donnee);
        }
    } else if(type_message == 1){
        double donnee = atof(donnees);
        donnee = donnee * 100;
        if(donnee > 80){
            printf("Un de nos envoyés spéciaux est contaminés à %lf \n", donnee);
        }
    } else if(type_message == 10){
        int donnee = atoi(donnees);
        donnee = donnee - donnee * 0.35;
        if(donnee > 0){
            printf("L'épidémie a déjà causé %i mort(s) \n", donnee);
        }
    } else if(type_message == 2){
        int donnee = atoi(donnees);
        donnee = donnee - donnee * 0.1;
        if(donnee > 0){
            printf("Actuellement, %i citoyens sont contaminés \n", donnee);
        }
    } else {
        printf("Erreur de transmission \n");
    }
}

int ajout_tableau_a_traiter(int priority, char* buffer, int tableau_a_traiter_size, Message *tableau_a_traiter){
    int occurence = 0;
    if(priority != 1 && tableau_a_traiter_size != 0){
        /*recherche occurence priority return*/
        if(occurence != 0){
            return tableau_a_traiter_size;
        }
    }
    tableau_a_traiter[tableau_a_traiter_size].priority = priority;
    tableau_a_traiter[tableau_a_traiter_size].donnee = buffer;
    return tableau_a_traiter_size ++;
    
}