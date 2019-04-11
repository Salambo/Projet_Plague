#include "src/journalist.h"

int main(){
    
    /*int day = 0;*/
    int n, priority;
    mqd_t mq;
    struct mq_attr attr;
    char *buffer = NULL;
    Message tableau_a_traiter[5];
    /*int i;
    int j;*/
    int nb_messages;
    int nb_journaliste_morts = 0;
    /*int tableau_a_traiter_size;*/

    mq = mq_open("/cable_foxnews", O_RDONLY | O_CREAT, 0644, NULL);
    sleep(10);

    printf("Bonjour et bienvenue sur FoxNewsTV! \n");
    printf("FLASH INFOS : Nous revenons en direct sur l'épimédie détectée à Racoon City \n");
    printf("Tout de suite des informations de la part de nos envoyés spéciaux Jacky et Michelle \n ");
    while(nb_journaliste_morts != 2) {
       /* printf("FLASH INFOS : Nous revenons en direct sur l'épimédie détectée à Racoon City \n", day);
        printf("Tout de suite des informations de la part de nos envoyés spéciaux Jacky et Michelle \n ");
        /*attente file de message pleine envoi signal une fois boulot journaliste fait*/
        mq_getattr(mq, &attr);
        nb_messages = attr.mq_curmsgs;
        while(nb_messages == 0){
            nb_messages = attr.mq_curmsgs;
        }
        /*if(nb_messages != 0){
            if(nb_messages > 4){
                tableau_a_traiter_size = 0;
                for(i=0; i<nb_messages; i++){
                    if(tableau_a_traiter_size == 5){
                        buffer = malloc(attr.mq_msgsize);
                        n = mq_receive(mq, buffer, attr.mq_msgsize, &priority);
                        tableau_a_traiter_size = ajout_tableau_a_traiter(priority, buffer, tableau_a_traiter_size, tableau_a_traiter[5]);
                    }
                }
                /*tri tableau?*/
               /* for(j=0; j<5; j++){
                    traitement_message(tableau_a_traiter[i].priority, tableau_a_traiter[i].donnee);
                }

            } else {
                for(i=0; i<nb_messages; i++){*/

                    buffer = malloc(attr.mq_msgsize);
                    n = mq_receive(mq, buffer, attr.mq_msgsize, &priority);
                    if(priority == 0){
                        nb_journaliste_morts ++;
                    } else {
                    traitement_message(priority, buffer);
                    }
            /*    }
            }
        } else {
            printf("Désolé, nous n'avons plus d'envoyés spéciaux....");
            return 0;
        }
        /*affichage message prioritaire transmis pour le journaliste*/
        /*envoi message comme quoi c'est fini pour aujourd'hui
        day ++;*/
    }

}