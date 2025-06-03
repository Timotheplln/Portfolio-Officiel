#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

// déclaration des constantes symboliques
#define MAX_MESSAGES 20
#define MAX_CAR 80
// définition des types
typedef char t_message[MAX_CAR];
typedef struct{
    t_message message;
}t_element;
typedef struct{
    t_element tabElt[MAX_MESSAGES];
    int nb;
}t_file;

// definition des constantes
const t_element ELTVIDE = {"------message vide------"};
// prototypes des fonctions
t_file initialiser();
void enfiler(t_file *adrF, t_element elt);
t_element defiler(t_file *adrF);
void vider(t_file *adrF);
t_element tete(t_file f);
void estVide(t_file f);
void estPleine(t_file f);
void afficherTous(t_file f);

// programme principal
int main(){
    // Declaration des variables
    t_file maFile;
    t_element elt;
    t_message msg;
    int choix;
    // initialisation
    maFile = initialiser();
    // ajouter quelques elements
    for (int i = 0 ; i<4 ; i++){
        sprintf(elt.message,"message %d", i);
        enfiler(&maFile, elt);
    }
    do{ // menu
        printf("----------------------------------------------------\n");
        printf("0 : quitter\n");
        printf("1 : afficher le nombre d'elements dans la file ?\n");
        printf("2 : ajouter un element a la file\n");
        printf("3 : retirer un element et afficher le message \n");
        printf("4 : afficher le message de la tete de file\n");
        printf("5 : vider la file\n");
        printf("6 : la file est-elle vide ?\n");
        printf("7 : la file est-elle pleine ?\n");
        printf("8 : supprimer les messages trop anciens\n");
        printf("9 : sauvegarde dans un fichier texte et vider\n");
        printf("10: lecture des messages du fichier texte\n");
        printf("votre choix : ");
        scanf("%d", &choix);
        printf("----------------------------------------------------\n");
        // traitement
        switch(choix){
            case -1 :   afficherTous(maFile);
            case 0:     break;
            case 1:     // afficher le nombre d'elements dans la file
                        afficherTous(maFile);
                        break;
            case 2:     // ajouter un element (à donner aux étudiants)
                        break;
            case 3:     elt = defiler(&maFile);
                        printf("%s\n",elt.message);
                        break;
            case 4:    // afficher le message de la tete de file
                        break; 
            case 5:    initialiser();
                        break;
            case 6:    // la file est-elle vide ?
                         break;
            case 7:    // la file est-elle pleine ?
                        break;
            case 8:     // supprimer les messages trop anciens
                        break;
            case 9:     //sauvegarde dans un fichier texte et vider
                        break;
            case 10:    //lecture des messages du fichier texte
                        break;
            default :   printf("erreur de saisie\n");
        }
    }while(choix != 0);
    return EXIT_SUCCESS;
}
// Definitions des fonctions
void afficherTous(t_file f){
    for (int i = 0; i < f.nb; i++)
    {
        printf("%s\n", f.tabElt[i]);
    }
}

t_file initialiser(){
    t_file f;
    f.nb = 0;
    for (int i = 0; i < MAX_MESSAGES; i++)
    {
        f.tabElt[i]=ELTVIDE;
    }
    return f;
}

void enfiler(t_file *adrF, t_element elt){
    adrF->tabElt[adrF->nb] = elt;
    (adrF->nb) ++;
}

t_element defiler(t_file *adrF){
    t_element elt;

    elt = adrF->tabElt[0];
    for (int i = 0; i < (adrF->nb)-1; i++)
    {
        adrF->tabElt[i] = adrF->tabElt[i+1];
    }
    adrF->tabElt[(adrF->nb)-1]=ELTVIDE;
    return elt;
}