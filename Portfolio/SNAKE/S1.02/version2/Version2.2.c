/**
* @brief deuxième version de l'automatisation du snake
* @brief Faire comprendre au serpent l'existence des portails & interdiction de se retourner sur soi-même
* @author Armel KERMANAC'H & Timothé PELLEN 1C1
* @version 2.2
* @date 16/12/2024
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <time.h>

/*******************************
*  DÉCLARATION DES CONSTANTES  *
*******************************/

const int TAILLE_MIN=10;
/**
* @def TAILLE_MIN
* @brief Taille initiale du serpent
*/
const int TAILLE_MAX=TAILLE_MIN+10; // PAS UTILISÉE
/**
* @def TAILLE_MAX
* @brief La taille maximum que peut atteindre le serpent
*/
const int TEMPORISATION=200000;
/**
* @def TEMPORISATION
* @brief Nombre initiale de µs pour la pause entre chaque affichage du serpent
*/
const int TEMP_INCREMENT=15000; // PAS UTILISÉE
/**
* @def TEMP_INCREMENT
* @brief Nombre de µs enlever à la pause du serpent pour chaque pomme mangée
*/
const int X_TAB=0;
/**
* @def X_TAB
* @brief Colonne dans les tableau posPave & pomme répertoriant les colonnes des pavés et de la pomme
*/
const int Y_TAB=1;
/**
* @def Y_TAB
* @brief Colonne dans les tableau posPave & pomme répertoriant les lignes des pavés et de la pomme
*/
const int MIN=1;
/**
* @def MIN
* @brief Coordonnée minimale pour la position du serpent
*/
const int MAX_LONG=80;
/**
* @def MAX_LONG
* @brief Coordonnée maximale sur la longueur pour la position du serpent
*/
const int MAX_HAUT=40;
/**
* @def MAX_HAUT
* @brief Coordonnée maximale sur la hauteur pour la position du serpent
*/
const int TAILLE_TROU=1;
/**
* @def TAILLE_TROU
* @brief Taille des portailles dans les bordures
*/
#define NB_POMMES 10
/**
* @def NB_POMMES
* @brief Nombre de pomme qu'il faut manger pour gagner le jeu
*/
const int X=40;
/**
* @def X
* @brief Position initiale de la tête sur les colonnes
*/
const int Y=20;
/**
* @def Y
* @brief Position initiale de la tête sur les lignes
*/
const char TETE='O';
/**
* @def TETE
* @brief Forme de la tête du serpent
*/
const char ANNEAUX='X';
/**
* @def ANNEAUX
* @brief Forme du reste du corps du serpent
*/
const char BORDURE='#';
/**
* @def BORDURE
* @brief Caractère délimitant la bordure du terrain de jeu
*/
const char POMME='6';
/**
* @def POMME
* @brief Caractère représentant une pomme pouvant être mangé pour le serpent
*/
const char HAUT='z';
/**
* @def HAUT
* @brief Touche à appuyer pour faire avancer le serpent vers le haut
*/
const char GAUCHE='q';
/**
* @def GAUCHE
* @brief Touche à appuyer pour faire avancer le serpent vers la gauche
*/
const char BAS='s';
/**
* @def BAS
* @brief Touche à appuyer pour faire avancer le serpent vers le bas
*/
const char DROITE='d';
/**
* @def DROITE
* @brief Touche à appuyer pour faire avancer le serpent vers la droite
*/
const char ARRET='a';
/**
* @def ARRET
* @brief Touche à appuyer pour arrêter le programme
*/
const char VIDE=' ';
/**
* @def VIDE
* @brief Le caractère par lequel les caractères "effacés" sont remplacés
*/
const int NB_GHOST=5;
/**
* @def NB_GHOST
* @brief Combien de serpent fantome vont être calculer ?
*/

/*******************************************
*  DÉCLARATION DES FONCTIONS & PROCÉDURES  *
*******************************************/

void dessinerSerpent(int lesX[TAILLE_MAX], int lesY[TAILLE_MAX], int taille); // Affiche un à un les éléments du serpent sur l'écran
void progresser(int lesX[TAILLE_MAX], int lesY[TAILLE_MAX], char direction, bool *collision, int taille, int pomme[2], bool *mangee); // Met à jour les positions du serpent et vérifie les collisions
void progresserghost(int lesX[TAILLE_MAX], int lesY[TAILLE_MAX], char direction, int taille, int destination[2]); // Met à jour les positions du serpent fantome

void afficher(int x, int y, char c); // Affiche un caractère à une position donnée sur le terminal
void effacer(int x, int y); // Remplace le car à la position x y par un espace

void initPlateau(char plateau[MAX_LONG+1][MAX_HAUT+1]); // Initialise le plateau de jeu avec des bordures et des espaces
void dessinerPlateau(char plateau[MAX_LONG+1][MAX_HAUT+1]); // Affiche le plateau de jeu sur le terminal

void ajouterPomme(int pomme[2],int pommeMangee); // Ajoute une pomme aléatoirement sur le plateau

void gotoXY(int x, int y); // Aller aux coordonnées données

int kbhit(); // Vérifie si une touche à été pressée

void disableEcho(); // Désactive l'affichage d'un caractère à l'écran
void enableEcho(); // Réactive l'affichage des caractères à l'écran

/************************
*  PROGRAMME PRINCIPAL  *
************************/

int main(){
    disableEcho();
    srand(time(NULL)); // Initialisation du générateur de nombres aléatoires

    // DÉCLARATION DES VARIABLES
    
    int i; // Compteur
    int temp; // Nombre de µs pour la pause entre chaque affichage du serpent
    char toucheAppuyer; // Vérifie quelle touche à été appuyée
    char direction; // Indique dans quel direction le serpent doit se diriger
    int lesY[TAILLE_MAX],lesX[TAILLE_MAX]; // Position de chaques parties du serpent
    int lesYghost[TAILLE_MAX],lesXghost[TAILLE_MAX]; // Un serpent invsible qui va faire les chemins possibles vers la pomme et les comparer
    int taille; // Taille du serpent
    int pommeMangee; // Combien de pomme ont été mangée
    int deplacement; // Combien de déplacement unitaire le serpent a fait
    int deplacementghost[NB_GHOST];
    int pomme[2]; // indique l'emplacementde la pomme sur le plateau
    int portail[2]; // indique l'emplacement d'un portail
    char plateau[MAX_LONG+1][MAX_HAUT+1]; // Plateau de jeu
    bool collision; // Indique si le serpent a touché la bordure ou son corps
    bool mangee; // Indique si le serpent a mangé une pomme

    // INITIALISATION DES DONNÉES

    system("clear"); // Passer à l'affichage du serpent
    initPlateau(plateau);
    pommeMangee=0;
    deplacement=0;
    mangee=false;
    collision=false;
    temp=TEMPORISATION; // La pause commence à 70000µs (0.07s)
    taille=TAILLE_MIN; // Le serpent commence avec une taille de 10 cases
    direction=DROITE; // Le serpent commence avec sa tête tournée vers la droite
    for(i=0;i<taille;i++){ // Initialisation des coordonnées du serpent
        lesY[i]=Y; // Implémentation de la position du serpent sur les lignes
        lesX[i]=X-i; // Implémentation de la position du serpent sur les colonnes
    }
    for(i=0;i<NB_GHOST;i++){
        deplacementghost[i]=0;
    }

    // AFFICHAGE
    dessinerPlateau(plateau);
    ajouterPomme(pomme,pommeMangee);
    dessinerSerpent(lesX,lesY,taille);
    clock_t begin = clock(); // Le CPU commence
    do{
        progresser(lesX,lesY,direction, &collision, taille, pomme, &mangee);
        deplacement++;
        if(mangee==true){
            mangee=false; // Le serpent n'a plus mangé de pomme
            pommeMangee++;
            ajouterPomme(pomme,pommeMangee); // Ajouter une nouvelle pomme sur le plateau
            // GHOST PAS DE PORTAILS
            for(i=0;i<taille;i++){
                lesXghost[i]=lesX[i];
                lesYghost[i]=lesY[i];
            }
            while((lesXghost[0]!=pomme[X_TAB])&&(lesYghost[0]!=pomme[Y_TAB])){
                if((direction==HAUT)||(direction==BAS)){ // Si le serpent avance vers la droite ou vers la gauche, il ne peut changer de direction que vers en bas ou vers en haut
                    if(pomme[X_TAB]<lesXghost[0]){ // Si la pomme est à gauche de la tête du serpent, le serpent va à gauche
                        direction=GAUCHE;
                    }else if(pomme[X_TAB]>lesXghost[0]){ // Si la pomme est à droite de la tête du serpent, le serpent va à droite
                        direction=DROITE;
                    }
                }else if((direction==GAUCHE)||(direction==DROITE)){ // Si le serpent avance vers le bas ou vers le haut, il ne peut changer de direction que vers la droite ou vers la gauche
                    if(pomme[Y_TAB]<lesYghost[0]){ // Quand la pomme et le tête du serpent sont sur la même colonne, si la pomme est au dessus de la tête du serpent, le serpent va en haut
                        direction=HAUT;
                    }else if(pomme[Y_TAB]>lesYghost[0]){ // Quand la pomme et le tête du serpent sont sur la même colonne, si la pomme est en dessous de la tête du serpent, le serpent va en bas
                        direction=BAS;
                    }
                }
                progresserghost(lesXghost,lesYghost,direction, taille, pomme);
                deplacementghost[0]++;
            }
            // GHOST PORTAIL DROITE
            for(i=0;i<taille;i++){
                lesXghost[i]=lesX[i];
                lesYghost[i]=lesY[i];
            }
            portail[X_TAB]=MAX_LONG;
            portail[Y_TAB]=(MAX_HAUT/2)-TAILLE_TROU;
            while((lesXghost[0]!=portail[X_TAB])&&(lesYghost[0]!=portail[Y_TAB])){
                if((direction==HAUT)||(direction==BAS)){ // Si le serpent avance vers la droite ou vers la gauche, il ne peut changer de direction que vers en bas ou vers en haut
                    if(portail[X_TAB]<lesXghost[0]){ // Si la pomme est à gauche de la tête du serpent, le serpent va à gauche
                        direction=GAUCHE;
                    }else if(portail[X_TAB]>lesXghost[0]){ // Si la pomme est à droite de la tête du serpent, le serpent va à droite
                        direction=DROITE;
                    }
                }else if((direction==GAUCHE)||(direction==DROITE)){ // Si le serpent avance vers le bas ou vers le haut, il ne peut changer de direction que vers la droite ou vers la gauche
                    if(portail[Y_TAB]<lesYghost[0]){ // Quand la pomme et le tête du serpent sont sur la même colonne, si la pomme est au dessus de la tête du serpent, le serpent va en haut
                        direction=HAUT;
                    }else if(portail[Y_TAB]>lesYghost[0]){ // Quand la pomme et le tête du serpent sont sur la même colonne, si la pomme est en dessous de la tête du serpent, le serpent va en bas
                        direction=BAS;
                    }
                }
                progresserghost(lesXghost,lesYghost,direction, taille, portail);
                deplacementghost[1]++;
            }
            lesXghost[0]+=2;
            deplacementghost[1]+=2;
            while((lesXghost[0]!=pomme[X_TAB])&&(lesYghost[0]!=pomme[Y_TAB])){
                if((direction==HAUT)||(direction==BAS)){ // Si le serpent avance vers la droite ou vers la gauche, il ne peut changer de direction que vers en bas ou vers en haut
                    if(pomme[X_TAB]<lesXghost[0]){ // Si la pomme est à gauche de la tête du serpent, le serpent va à gauche
                        direction=GAUCHE;
                    }else if(pomme[X_TAB]>lesXghost[0]){ // Si la pomme est à droite de la tête du serpent, le serpent va à droite
                        direction=DROITE;
                    }
                }else if((direction==GAUCHE)||(direction==DROITE)){ // Si le serpent avance vers le bas ou vers le haut, il ne peut changer de direction que vers la droite ou vers la gauche
                    if(pomme[Y_TAB]<lesYghost[0]){ // Quand la pomme et le tête du serpent sont sur la même colonne, si la pomme est au dessus de la tête du serpent, le serpent va en haut
                        direction=HAUT;
                    }else if(pomme[Y_TAB]>lesYghost[0]){ // Quand la pomme et le tête du serpent sont sur la même colonne, si la pomme est en dessous de la tête du serpent, le serpent va en bas
                        direction=BAS;
                    }
                }
                progresserghost(lesXghost,lesYghost,direction, taille, pomme);
                deplacementghost[1]++;
            }
            // GHOST PORTAIL GAUCHE
            for(i=0;i<taille;i++){
                lesXghost[i]=lesX[i];
                lesYghost[i]=lesY[i];
            }
            portail[X_TAB]=MIN;
            portail[Y_TAB]=(MAX_HAUT/2)-TAILLE_TROU;
            while((lesXghost[0]!=portail[X_TAB])&&(lesYghost[0]!=portail[Y_TAB])){
                if((direction==HAUT)||(direction==BAS)){ // Si le serpent avance vers la droite ou vers la gauche, il ne peut changer de direction que vers en bas ou vers en haut
                    if(portail[X_TAB]<lesXghost[0]){ // Si la pomme est à gauche de la tête du serpent, le serpent va à gauche
                        direction=GAUCHE;
                    }else if(portail[X_TAB]>lesXghost[0]){ // Si la pomme est à droite de la tête du serpent, le serpent va à droite
                        direction=DROITE;
                    }
                }else if((direction==GAUCHE)||(direction==DROITE)){ // Si le serpent avance vers le bas ou vers le haut, il ne peut changer de direction que vers la droite ou vers la gauche
                    if(portail[Y_TAB]<lesYghost[0]){ // Quand la pomme et le tête du serpent sont sur la même colonne, si la pomme est au dessus de la tête du serpent, le serpent va en haut
                        direction=HAUT;
                    }else if(portail[Y_TAB]>lesYghost[0]){ // Quand la pomme et le tête du serpent sont sur la même colonne, si la pomme est en dessous de la tête du serpent, le serpent va en bas
                        direction=BAS;
                    }
                }
                progresserghost(lesXghost,lesYghost,direction, taille, portail);
                deplacementghost[2]++;
            }
            lesXghost[0]-=2;
            deplacementghost[2]+=2;
            while((lesXghost[0]!=pomme[X_TAB])&&(lesYghost[0]!=pomme[Y_TAB])){
                if((direction==HAUT)||(direction==BAS)){ // Si le serpent avance vers la droite ou vers la gauche, il ne peut changer de direction que vers en bas ou vers en haut
                    if(pomme[X_TAB]<lesXghost[0]){ // Si la pomme est à gauche de la tête du serpent, le serpent va à gauche
                        direction=GAUCHE;
                    }else if(pomme[X_TAB]>lesXghost[0]){ // Si la pomme est à droite de la tête du serpent, le serpent va à droite
                        direction=DROITE;
                    }
                }else if((direction==GAUCHE)||(direction==DROITE)){ // Si le serpent avance vers le bas ou vers le haut, il ne peut changer de direction que vers la droite ou vers la gauche
                    if(pomme[Y_TAB]<lesYghost[0]){ // Quand la pomme et le tête du serpent sont sur la même colonne, si la pomme est au dessus de la tête du serpent, le serpent va en haut
                        direction=HAUT;
                    }else if(pomme[Y_TAB]>lesYghost[0]){ // Quand la pomme et la tête du serpent sont sur la même colonne, si la pomme est en dessous de la tête du serpent, le serpent va en bas
                        direction=BAS;
                    }
                }
                progresserghost(lesXghost,lesYghost,direction, taille, pomme);
                deplacementghost[2]++;
            }
            // GHOST PORTAIL HAUT
            for(i=0;i<taille;i++){
                lesXghost[i]=lesX[i];
                lesYghost[i]=lesY[i];
            }
            portail[X_TAB]=(MAX_LONG/2)-TAILLE_TROU;
            portail[Y_TAB]=MIN;
            while((lesXghost[0]!=portail[X_TAB])&&(lesYghost[0]!=portail[Y_TAB])){
                if((direction==HAUT)||(direction==BAS)){ // Si le serpent avance vers la droite ou vers la gauche, il ne peut changer de direction que vers en bas ou vers en haut
                    if(portail[X_TAB]<lesXghost[0]){ // Si la pomme est à gauche de la tête du serpent, le serpent va à gauche
                        direction=GAUCHE;
                    }else if(portail[X_TAB]>lesXghost[0]){ // Si la pomme est à droite de la tête du serpent, le serpent va à droite
                        direction=DROITE;
                    }
                }else if((direction==GAUCHE)||(direction==DROITE)){ // Si le serpent avance vers le bas ou vers le haut, il ne peut changer de direction que vers la droite ou vers la gauche
                    if(portail[Y_TAB]<lesYghost[0]){ // Quand la pomme et le tête du serpent sont sur la même colonne, si la pomme est au dessus de la tête du serpent, le serpent va en haut
                        direction=HAUT;
                    }else if(portail[Y_TAB]>lesYghost[0]){ // Quand la pomme et la tête du serpent sont sur la même colonne, si la pomme est en dessous de la tête du serpent, le serpent va en bas
                        direction=BAS;
                    }
                }
                progresserghost(lesXghost,lesYghost,direction, taille, portail);
                deplacementghost[3]++;
            }
            lesYghost[0]-=2;
            deplacementghost[3]+=2;
            while((lesXghost[0]!=pomme[X_TAB])&&(lesYghost[0]!=pomme[Y_TAB])){
                if((direction==HAUT)||(direction==BAS)){ // Si le serpent avance vers la droite ou vers la gauche, il ne peut changer de direction que vers en bas ou vers en haut
                    if(pomme[X_TAB]<lesXghost[0]){ // Si la pomme est à gauche de la tête du serpent, le serpent va à gauche
                        direction=GAUCHE;
                    }else if(pomme[X_TAB]>lesXghost[0]){ // Si la pomme est à droite de la tête du serpent, le serpent va à droite
                        direction=DROITE;
                    }
                }else if((direction==GAUCHE)||(direction==DROITE)){ // Si le serpent avance vers le bas ou vers le haut, il ne peut changer de direction que vers la droite ou vers la gauche
                    if(pomme[Y_TAB]<lesYghost[0]){ // Quand la pomme et le tête du serpent sont sur la même colonne, si la pomme est au dessus de la tête du serpent, le serpent va en haut
                        direction=HAUT;
                    }else if(pomme[Y_TAB]>lesYghost[0]){ // Quand la pomme et la tête du serpent sont sur la même colonne, si la pomme est en dessous de la tête du serpent, le serpent va en bas
                        direction=BAS;
                    }
                }
                progresserghost(lesXghost,lesYghost,direction, taille, pomme);
                deplacementghost[3]++;
            }
            // GHOST PORTAIL BAS
            for(i=0;i<taille;i++){
                lesXghost[i]=lesX[i];
                lesYghost[i]=lesY[i];
            }
            portail[X_TAB]=(MAX_LONG/2)-TAILLE_TROU;
            portail[Y_TAB]=MAX_HAUT;
            while((lesXghost[0]!=portail[X_TAB])&&(lesYghost[0]!=portail[Y_TAB])){
                if((direction==HAUT)||(direction==BAS)){ // Si le serpent avance vers la droite ou vers la gauche, il ne peut changer de direction que vers en bas ou vers en haut
                    if(portail[X_TAB]<lesXghost[0]){ // Si la pomme est à gauche de la tête du serpent, le serpent va à gauche
                        direction=GAUCHE;
                    }else if(portail[X_TAB]>lesXghost[0]){ // Si la pomme est à droite de la tête du serpent, le serpent va à droite
                        direction=DROITE;
                    }
                }else if((direction==GAUCHE)||(direction==DROITE)){ // Si le serpent avance vers le bas ou vers le haut, il ne peut changer de direction que vers la droite ou vers la gauche
                    if(portail[Y_TAB]<lesYghost[0]){ // Quand la pomme et le tête du serpent sont sur la même colonne, si la pomme est au dessus de la tête du serpent, le serpent va en haut
                        direction=HAUT;
                    }else if(portail[Y_TAB]>lesYghost[0]){ // Quand la pomme et la tête du serpent sont sur la même colonne, si la pomme est en dessous de la tête du serpent, le serpent va en bas
                        direction=BAS;
                    }
                }
                progresserghost(lesXghost,lesYghost,direction, taille, portail);
                deplacementghost[4]++;
            }
            lesYghost[0]+=2;
            deplacementghost[4]+=2;
            while((lesXghost[0]!=pomme[X_TAB])&&(lesYghost[0]!=pomme[Y_TAB])){
                if((direction==HAUT)||(direction==BAS)){ // Si le serpent avance vers la droite ou vers la gauche, il ne peut changer de direction que vers en bas ou vers en haut
                    if(pomme[X_TAB]<lesXghost[0]){ // Si la pomme est à gauche de la tête du serpent, le serpent va à gauche
                        direction=GAUCHE;
                    }else if(pomme[X_TAB]>lesXghost[0]){ // Si la pomme est à droite de la tête du serpent, le serpent va à droite
                        direction=DROITE;
                    }
                }else if((direction==GAUCHE)||(direction==DROITE)){ // Si le serpent avance vers le bas ou vers le haut, il ne peut changer de direction que vers la droite ou vers la gauche
                    if(pomme[Y_TAB]<lesYghost[0]){ // Quand la pomme et le tête du serpent sont sur la même colonne, si la pomme est au dessus de la tête du serpent, le serpent va en haut
                        direction=HAUT;
                    }else if(pomme[Y_TAB]>lesYghost[0]){ // Quand la pomme et la tête du serpent sont sur la même colonne, si la pomme est en dessous de la tête du serpent, le serpent va en bas
                        direction=BAS;
                    }
                }
                progresserghost(lesXghost,lesYghost,direction, taille, pomme);
                deplacementghost[4]++;
            }
        }
        if((direction==HAUT)||(direction==BAS)){ // Si le serpent avance vers la droite ou vers la gauche, il ne peut changer de direction que vers en bas ou vers en haut
            if(pomme[X_TAB]<lesX[0]){ // Si la pomme est à gauche de la tête du serpent, le serpent va à gauche
                direction=GAUCHE;
            }else if(pomme[X_TAB]>lesX[0]){ // Si la pomme est à droite de la tête du serpent, le serpent va à droite
                direction=DROITE;
            }
        }else if((direction==GAUCHE)||(direction==DROITE)){ // Si le serpent avance vers le bas ou vers le haut, il ne peut changer de direction que vers la droite ou vers la gauche
            if(pomme[Y_TAB]<lesY[0]){ // Quand la pomme et le tête du serpent sont sur la même colonne, si la pomme est au dessus de la tête du serpent, le serpent va en haut
                direction=HAUT;
            }else if(pomme[Y_TAB]>lesY[0]){ // Quand la pomme et le tête du serpent sont sur la même colonne, si la pomme est en dessous de la tête du serpent, le serpent va en bas
                direction=BAS;
            }
        }
        usleep(temp); // Attendre
        if(kbhit()==1){
            toucheAppuyer=getchar();
        }
    }while((toucheAppuyer!=ARRET)&&(collision==false)&&(pommeMangee<NB_POMMES)); // Si le caractère appuyé est un 'a' ou que le serpent a touché la bordure ou son corps ou que le serpent a atteint sa taille maximale, le programme s'arrête
    clock_t end = clock(); // Le CPU s'arrête
    enableEcho();
    system("clear"); // Efface tout ce qu'il y a à l'écran
    double tmpsCPU = ((end - begin)*1.0) / CLOCKS_PER_SEC;
    printf("Temps CPU = %.3f secondes\n",tmpsCPU);
    printf("Déplacements unitaires = %d\n",deplacement);
    return EXIT_SUCCESS;
}

/*********************************
*  PROCÉDURES & FONCTIONS CRÉES  *
*********************************/
/**
* @fn dessinerSerpent
* @brief Affiche un à un les éléments du serpent sur l'écran
* @param lesX : Tableau des positions en X du serpent
* @param lesY : Tableau des positions en Y du serpent
* @param taille : Taille du serpent
*/
void dessinerSerpent(int lesX[TAILLE_MAX], int lesY[TAILLE_MAX], int taille){
    
    // DÉCLARATION DES VARIABLES

    int i; // Compteurs

    // AFFICHAGE

    for(i=0;i<taille;i++){
        if((lesX[i]>=MIN)&&(lesY[i]>=MIN)){ // Affiche le serpent uniquement s'il est visible
            if(i==0){
                afficher(lesX[i],lesY[i],TETE); // Affichage de la tête
            }else if(lesX[i]>0){
                afficher(lesX[i],lesY[i],ANNEAUX); // Affichage du corps
            }
        }
    }
}

/**
* @fn progresser
* @brief Met à jour les positions du serpent et vérifie les collisions
* @param lesX : Tableau des positions en X du serpent
* @param lesY : Tableau des positions en Y du serpent
* @param direction : Direction dans laquelle se déplace le serpent
* @param collision : Pointeur sur un booléen indiquant si le serpent a touché un obstacle
* @param posPave : Tableau des positions des pavés
* @param taille : Taille du serpent
* @param pomme : Tableau des positions de la pomme
* @param mangee : Pointeur sur un booléen indiquant si le serpent a mangé la pomme
*/
void progresser(int lesX[TAILLE_MAX], int lesY[TAILLE_MAX], char direction, bool *collision, int taille, int pomme[2], bool *mangee){
    
    // DÉCLARATION DES VARIABLES

    int i; // Compteur

    // AFFICHAGE

    if((lesX[taille-1]>=MIN)&&(lesY[taille-1]>=MIN)){ // Supprimer le caractère derrière le serpent
        effacer(lesX[taille-1],lesY[taille-1]);
    }
    for(i=taille-1;i>0;i--){ // La partie du corps reprend les coordonnées de celle devant lui
        lesX[i]=lesX[i-1];
        lesY[i]=lesY[i-1];
    }
    if(direction==DROITE){ // Dans quelle direction va le serpent
        lesX[0]=lesX[0]+1;
    }else if(direction==GAUCHE){
        lesX[0]=lesX[0]-1;
    }else if(direction==HAUT){
        lesY[0]=lesY[0]-1;
    }else if(direction==BAS){
        lesY[0]=lesY[0]+1;
    }
    if(  ((lesX[0]<=MIN) && ((lesY[0]<((MAX_HAUT/2)-(TAILLE_TROU/2))) || (lesY[0]>=((MAX_HAUT/2)+(TAILLE_TROU/2)))))  ||  ((lesY[0]<=MIN) && ((lesX[0]<((MAX_LONG/2)-(TAILLE_TROU/2))) || (lesX[0]>=((MAX_LONG/2)+(TAILLE_TROU/2)))))  ||  ((lesX[0]>=MAX_LONG) && ((lesY[0]<((MAX_HAUT/2)-(TAILLE_TROU/2))) || (lesY[0]>=((MAX_HAUT/2)+(TAILLE_TROU/2)))))  ||  ((lesY[0]>=MAX_HAUT) && ((lesX[0]<((MAX_LONG/2)-(TAILLE_TROU/2))) || (lesX[0]>=((MAX_LONG/2)+(TAILLE_TROU/2)))))  ){ // Vérifier si le serpent a touché la bordure
        *collision=true;
    }
    for(i=1;i<taille;i++){ // Vérifier si le serpent a touché son corps
        if((lesX[i]==lesX[0])&&(lesY[i]==lesY[0])){
            *collision=true;
        }
    }
    if((lesX[0]<MIN) && ((lesY[0]>=((MAX_HAUT/2)-TAILLE_TROU)) && (lesY[0]<=((MAX_HAUT/2))))){ // Vérifier si le serpent a traversé le portail de gauche
        lesX[0]=MAX_LONG;
    }else if((lesY[0]<MIN) && ((lesX[0]>=((MAX_LONG/2)-TAILLE_TROU)) && (lesX[0]<=((MAX_LONG/2))))){ // Vérifier si le serpent a traversé le portail en haut
        lesY[0]=MAX_HAUT;
    }else if((lesX[0]>MAX_LONG) && ((lesY[0]>=((MAX_HAUT/2)-TAILLE_TROU)) && (lesY[0]<=((MAX_HAUT/2))))){ // Vérifier si le serpent a traversé le portail de droite
        lesX[0]=MIN;
    }else if((lesY[0]>MAX_HAUT) && ((lesX[0]>=((MAX_LONG/2)-TAILLE_TROU)) && (lesX[0]<=((MAX_LONG/2))))){ // Vérifier si le serpent a traversé le portail en bas
        lesY[0]=MIN;
    }
    if((lesX[0]==pomme[X_TAB])&&(lesY[0]==pomme[Y_TAB])){ // Vérifier si le serpent a mangé la pomme
        *mangee=true;
    }
    dessinerSerpent(lesX,lesY,taille);
}

void progresserghost(int lesX[TAILLE_MAX], int lesY[TAILLE_MAX], char direction, int taille, int destination[2]){
    
    // DÉCLARATION DES VARIABLES

    int i; // Compteur

    // CALCUL

    for(i=taille-1;i>0;i--){ // La partie du corps reprend les coordonnées de celle devant lui
        lesX[i]=lesX[i-1];
        lesY[i]=lesY[i-1];
    }

    if(direction==DROITE){ // Dans quelle direction va le serpent
        lesX[0]=lesX[0]+1;
    }else if(direction==GAUCHE){
        lesX[0]=lesX[0]-1;
    }else if(direction==HAUT){
        lesY[0]=lesY[0]-1;
    }else if(direction==BAS){
        lesY[0]=lesY[0]+1;
    }

    if((lesX[0]<MIN) && ((lesY[0]>=((MAX_HAUT/2)-TAILLE_TROU)) && (lesY[0]<=((MAX_HAUT/2))))){ // Vérifier si le serpent a traversé le portail de gauche
        lesX[0]=MAX_LONG;
    }else if((lesY[0]<MIN) && ((lesX[0]>=((MAX_LONG/2)-TAILLE_TROU)) && (lesX[0]<=((MAX_LONG/2))))){ // Vérifier si le serpent a traversé le portail en haut
        lesY[0]=MAX_HAUT;
    }else if((lesX[0]>MAX_LONG) && ((lesY[0]>=((MAX_HAUT/2)-TAILLE_TROU)) && (lesY[0]<=((MAX_HAUT/2))))){ // Vérifier si le serpent a traversé le portail de droite
        lesX[0]=MIN;
    }else if((lesY[0]>MAX_HAUT) && ((lesX[0]>=((MAX_LONG/2)-TAILLE_TROU)) && (lesX[0]<=((MAX_LONG/2))))){ // Vérifier si le serpent a traversé le portail en bas
        lesY[0]=MIN;
    }
}

/**
* @fn afficher
* @brief Affiche un caractère à une position donnée sur le terminal
* @param x : Position en X sur le terminal
* @param y : Position en Y sur le terminal
* @param c : Caractère à afficher
*/
void afficher(int x, int y, char c){
    
    // AFFICHAGE

    gotoXY(x,y);
    printf("%c", c);
}

/**
* @fn effacer
* @brief Remplace le caractère à la position x y par un espace
* @param x : Position en X sur le terminal
* @param y : Position en Y sur le terminal
*/
void effacer(int x, int y){
    
    // AFFICHAGE

    gotoXY(x,y);
    printf("%c", VIDE);
}

/**
* @fn initPlateau
* @brief Initialise le plateau de jeu avec des bordures et des espaces
* @param plateau : Plateau de jeu à initialiser
*/
void initPlateau(char plateau[MAX_LONG+1][MAX_HAUT+1]){
    
    int i,j; // Compteurs

    // INITIALISATION DES DONNÉES

    // Bordure du haut
    for(i=MIN;i<(MAX_LONG/2)-TAILLE_TROU;i++){
        plateau[i][MIN]=BORDURE;
    }
    for(i=(MAX_LONG/2)-TAILLE_TROU;i<(MAX_LONG/2);i++){
        plateau[i][MIN]=VIDE;
    }
    for(i=(MAX_LONG/2);i<=MAX_LONG;i++){
        plateau[i][MIN]=BORDURE;
    }
    // Bordure & espace de jeu
    for(i=MIN+1;i<(MAX_HAUT/2)-TAILLE_TROU;i++){
        plateau[MIN][i]=BORDURE;
        for(j=MIN+1;j<MAX_LONG;j++){
            plateau[j][i]=VIDE;
        }
        plateau[MAX_LONG][i]=BORDURE;
    }
    for(i=(MAX_HAUT/2)-TAILLE_TROU;i<(MAX_HAUT/2);i++){
        for(j=MIN;j<=MAX_LONG;j++){
            plateau[j][i]=VIDE;
        }
    }
    for(i=(MAX_HAUT/2);i<MAX_HAUT;i++){
        plateau[MIN][i]=BORDURE;
        for(j=MIN+1;j<MAX_LONG;j++){
            plateau[j][i]=VIDE;
        }
        plateau[MAX_LONG][i]=BORDURE;
    }
    // Bordure du bas
    for(i=MIN;i<(MAX_LONG/2)-TAILLE_TROU;i++){
        plateau[i][MAX_HAUT]=BORDURE;
    }
    for(i=(MAX_LONG/2)-TAILLE_TROU;i<(MAX_LONG/2);i++){
        plateau[i][MAX_HAUT]=VIDE;
    }
    for(i=(MAX_LONG/2);i<=MAX_LONG;i++){
        plateau[i][MAX_HAUT]=BORDURE;
    }
}

/**
* @fn dessinerPlateau
* @brief Affiche le plateau de jeu sur le terminal
* @param plateau : Plateau de jeu à afficher
*/
void dessinerPlateau(char plateau[MAX_LONG+1][MAX_HAUT+1]){
    
    int i,j; // Compteurs

    // AFFICHAGE

    for(i=MIN;i<=MAX_LONG;i++){
        for(j=MIN;j<=MAX_HAUT;j++){
            afficher(i,j,plateau[i][j]);
        }
    }
}

/**
* @fn ajouterPomme
* @brief Ajoute une pomme aléatoirement sur le plateau, en évitant de se trouver dans un pavé
* @param pomme : Tableau pour stocker les coordonnées de la pomme
* @param posPave : Tableau pour stocker les coordonnées des pavés
*/
void ajouterPomme(int pomme[2],int pommeMangee){
    
    // DÉCLARATIONS DES VARIABLES

    int lesPommesX[NB_POMMES] = {75, 75, 78, 2, 8, 78, 74, 2, 72, 5}; // Coordonnées de toutes les pommes sur les colonnes
    int lesPommesY[NB_POMMES] = { 8, 39, 2, 2, 5, 39, 33, 38, 35, 2}; // Coordonnées de toutes les pommes sur les lignes

    // INITIALISATION DES DONNÉES

    pomme[X_TAB]=lesPommesX[pommeMangee];
    pomme[Y_TAB]=lesPommesY[pommeMangee];

    // AFFICHAGE
    
    afficher(pomme[X_TAB],pomme[Y_TAB],POMME);
}

/**********************************************
*  PROCÉDURES & FONCTIONS DONNÉES SUR MOODLE  *
**********************************************/

void gotoXY(int x, int y) { 
    printf("\033[%d;%df", y, x);
}

int kbhit(){
	// la fonction retourne :
	// 1 si un caractere est present
	// 0 si pas de caractere present
	
	int unCaractere=0;
	struct termios oldt, newt;
	int ch;
	int oldf;

	// mettre le terminal en mode non bloquant
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
	ch = getchar();

	// restaurer le mode du terminal
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
 
	if(ch != EOF){
		ungetc(ch, stdin);
		unCaractere=1;
	} 
	return unCaractere;
}

void disableEcho() {
    struct termios tty;

    // Obtenir les attributs du terminal
    if (tcgetattr(STDIN_FILENO, &tty) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Desactiver le flag ECHO
    tty.c_lflag &= ~ECHO;

    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}

void enableEcho() {
    struct termios tty;

    // Obtenir les attributs du terminal
    if (tcgetattr(STDIN_FILENO, &tty) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Reactiver le flag ECHO
    tty.c_lflag |= ECHO;

    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}
