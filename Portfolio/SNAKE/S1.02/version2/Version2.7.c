/**
* @brief deuxième version de l'automatisation du snake
* @brief Faire comprendre au serpent l'existence des portails & interdiction de se retourner sur soi-même
* @author Armel KERMANAC'H & Timothé PELLEN 1C1
* @version 2.5
* @date 10/12/2024
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
* @brief Nombre de serpents fantômes utilisés pour calculer chaque chemins possibles, un qui n'utilise pas de portail, les quatre autres utilisant respectivement un des portails
*/

/*******************************************
*  DÉCLARATION DES FONCTIONS & PROCÉDURES  *
*******************************************/

void dessinerSerpent(int lesX[TAILLE_MAX], int lesY[TAILLE_MAX], int taille); // Affiche un à un les éléments du serpent sur l'écran
void progresser(int lesX[TAILLE_MAX], int lesY[TAILLE_MAX], char direction, bool *collision, int taille, int pomme[2], bool *mangee, bool *portailPasser); // Met à jour les positions du serpent et vérifie les collisions
void progresserPortail(int lesX[TAILLE_MAX], int lesY[TAILLE_MAX], char direction, bool *collision, int taille, int pomme[2], bool *mangee);

void afficher(int x, int y, char c); // Affiche un caractère à une position donnée sur le terminal
void effacer(int x, int y); // Remplace le car à la position x y par un espace

void initPlateau(char plateau[MAX_LONG+1][MAX_HAUT+1]); // Initialise le plateau de jeu avec des bordures et des espaces
void dessinerPlateau(char plateau[MAX_LONG+1][MAX_HAUT+1]); // Affiche le plateau de jeu sur le terminal

void ajouterPomme(int pomme[2],int pommeMangee); // Ajoute une pomme aléatoirement sur le plateau

void gotoXY(int x, int y); // Aller aux coordonnées données

int kbhit(); // Vérifie si une touche à été pressée

void disableEcho(); // Désactive l'affichage d'un caractère à l'écran
void enableEcho(); // Réactive l'affichage des caractères à l'écran

void progresserG(int lesX[TAILLE_MAX], int lesY[TAILLE_MAX], char *direction, int taille, int destination[2], int pomme[2], bool *mangee, int *deplacement); // Met à jour les positions du serpent fantôme
void progresserPortailG(int pomme[2], int *deplacement, int lesX[TAILLE_MAX], int lesY[TAILLE_MAX], char *direction, bool *mangee, int taille); // Deplacement du serpent en commençant par la sortie d'un portail

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
    int taille; // Taille du serpent
    int pommeMangee; // Combien de pomme ont été mangée
    int deplacement; // Combien de déplacement unitaire le serpent a fait
    int pomme[2]; // indique l'emplacement de la pomme sur le plateau
    char plateau[MAX_LONG+1][MAX_HAUT+1]; // Plateau de jeu
    bool collision; // Indique si le serpent a touché la bordure ou son corps
    bool mangee; // Indique si le serpent a mangé une pomme

    // Variables pour les serpents fantômes qui feront tous les chemins possibles avant le serpent et calculeront le chemin le plus court

    int lesYG[TAILLE_MAX], lesXG[TAILLE_MAX]; // Position de chaques parties du serpent fantôme
    int deplacementG[NB_GHOST]; // Combien de déplacement unitaire les serpents fantômes ont fait
    int bestDeplacement; // Déplacement gardé à la fin du calcul
    int portailG[2]={MIN-1,(MAX_HAUT/2)-TAILLE_TROU}; // indique l'emplacement du portail de droite
    int portailD[2]={MAX_LONG+1,(MAX_HAUT/2)-TAILLE_TROU}; // indique l'emplacement du portail de gauche
    int portailH[2]={(MAX_LONG/2)-TAILLE_TROU,MIN-1}; // indique l'emplacement du portail du haut
    int portailB[2]={(MAX_LONG/2)-TAILLE_TROU,MAX_HAUT+1}; // indique l'emplacement du portail du bas
    char directionG; // Indique dans quel direction le serpent doit se diriger
    bool mangeeG; // Indique si le serpent fantôme a théoriquement mangé une pomme
    bool portailPasser; // Indique si un portail à été traversé

    // INITIALISATION DES DONNÉES

    system("clear"); // Passer à l'affichage du serpent
    initPlateau(plateau);
    pommeMangee=0;
    deplacement=0;
    mangee=false;
    portailPasser=false;
    collision=false;
    temp=TEMPORISATION; // La pause commence à 200000µs (0.2s)
    taille=TAILLE_MIN; // Le serpent commence avec une taille de 10 cases
    direction=DROITE; // Le serpent commence avec sa tête tournée vers la droite
    for(i=0;i<taille;i++){ // Initialisation des coordonnées du serpent
        lesY[i]=Y; // Implémentation de la position du serpent sur les lignes
        lesX[i]=X-i; // Implémentation de la position du serpent sur les colonnes
    }

    // AFFICHAGE
    dessinerPlateau(plateau);
    ajouterPomme(pomme,pommeMangee);
    dessinerSerpent(lesX,lesY,taille);
    clock_t begin = clock(); // Le CPU commence
    do{
        if(!portailPasser){
            progresser(lesX,lesY,direction, &collision, taille, pomme, &mangee, &portailPasser);
        }else{
            progresserPortail(lesX,lesY,direction, &collision, taille, pomme, &mangee);
        }
        deplacement++;
        if(mangee){
            mangee=false; // Le serpent n'a plus mangé de pomme
            portailPasser=false;
            mangeeG=false;
            pommeMangee++;
            ajouterPomme(pomme,pommeMangee); // Ajouter une nouvelle pomme sur le plateau
            for(i=0;i<NB_GHOST;i++){
                deplacementG[i]=0;
            }
            for(i=0;i<taille;i++){
                lesXG[i]=lesX[i];
                lesYG[i]=lesY[i];
            }
            directionG=direction;
            while(!mangeeG){
                progresserG(lesXG, lesYG, &directionG, taille, pomme, pomme, &mangeeG, &deplacementG[0]);
            }
            mangeeG=false;
            for(i=0;i<taille;i++){
                lesXG[i]=lesX[i];
                lesYG[i]=lesY[i];
            }
            directionG=direction;
            while(!mangeeG){
                progresserG(lesXG, lesYG, &directionG, taille, portailD, pomme, &mangeeG, &deplacementG[1]);
            }
            mangeeG=false;
            for(i=0;i<taille;i++){
                lesXG[i]=lesX[i];
                lesYG[i]=lesY[i];
            }
            directionG=direction;
            while(!mangeeG){
                progresserG(lesXG, lesYG, &directionG, taille, portailG, pomme, &mangeeG, &deplacementG[2]);
            }
            mangeeG=false;
            for(i=0;i<taille;i++){
                lesXG[i]=lesX[i];
                lesYG[i]=lesY[i];
            }
            directionG=direction;
            while(!mangeeG){
                progresserG(lesXG, lesYG, &directionG, taille, portailH, pomme, &mangeeG, &deplacementG[3]);
            }
            mangeeG=false;
            for(i=0;i<taille;i++){
                lesXG[i]=lesX[i];
                lesYG[i]=lesY[i];
            }
            directionG=direction;
            while(!mangeeG){
                progresserG(lesXG, lesYG, &directionG, taille, portailB, pomme, &mangeeG, &deplacementG[4]);
            }
            bestDeplacement=deplacementG[0];
            for(i=1;i<NB_GHOST;i++){
                if(bestDeplacement>deplacementG[i]){
                    bestDeplacement=deplacementG[i];
                }
            }
        }
        for(i=1;i<taille;i++){
            if((direction==HAUT)||(direction==BAS)){ // Si le serpent avance vers la droite ou vers la gauche, il ne peut changer de direction que vers en bas ou vers en haut
                if(pomme[X_TAB]<lesX[0]){ // Si la pomme est à gauche de la tête du serpent, le serpent va à gauche
                    if(lesX[0]-1!=lesX[i]){
                        direction=GAUCHE;
                    }
                }else if(pomme[X_TAB]>lesX[0]){ // Si la pomme est à droite de la tête du serpent, le serpent va à droite
                    if(lesX[0]+1!=lesX[i]){
                        direction=DROITE;
                    }
                }
            }else if((direction==GAUCHE)||(direction==DROITE)){ // Si le serpent avance vers le bas ou vers le haut, il ne peut changer de direction que vers la droite ou vers la gauche
                if(pomme[Y_TAB]<lesY[0]){ // Quand la pomme et le tête du serpent sont sur la même colonne, si la pomme est au dessus de la tête du serpent, le serpent va en haut
                    if(lesY[0]-1!=lesY[i]){
                        direction=HAUT;
                    }
                }else if(pomme[Y_TAB]>lesY[0]){ // Quand la pomme et le tête du serpent sont sur la même colonne, si la pomme est en dessous de la tête du serpent, le serpent va en bas
                    if(lesY[0]+1!=lesY[i]){
                        direction=BAS;
                    }
                }
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
* @param taille : Taille du serpent
* @param pomme : Tableau des positions de la pomme
* @param mangee : Pointeur sur un booléen indiquant si le serpent a mangé la pomme
*/
void progresser(int lesX[TAILLE_MAX], int lesY[TAILLE_MAX], char direction, bool *collision, int taille, int pomme[2], bool *mangee, bool *portailPasser){
    // DÉCLARATION DES VARIABLES

    int i; // Compteur

    // AFFICHAGE

    if((*direction==HAUT)||(*direction==BAS)){ // Si le serpent avance vers la droite ou vers la gauche, il ne peut changer de direction que vers en bas ou vers en haut
        if(destination[X_TAB]<lesX[0]){ // Si la destination est à gauche de la tête du serpent, le serpent va à gauche
            *direction=GAUCHE;
        }else if(destination[X_TAB]>lesX[0]){ // Si la destination est à droite de la tête du serpent, le serpent va à droite
            *direction=DROITE;
        }
    }else if((*direction==GAUCHE)||(*direction==DROITE)){ // Si le serpent avance vers le bas ou vers le haut, il ne peut changer de direction que vers la droite ou vers la gauche
        if(destination[Y_TAB]<lesY[0]){ // Quand la destination et le tête du serpent sont sur la même colonne, si la destination est au dessus de la tête du serpent, le serpent va en haut
            *direction=HAUT;
        }else if(destination[Y_TAB]>lesY[0]){ // Quand la destination et le tête du serpent sont sur la même colonne, si la destination est en dessous de la tête du serpent, le serpent va en bas
            *direction=BAS;
        }
    }
    for (int g = 2; g < taille; g++)
	{
        if (*direction==DROITE)
        {
            if (lesX[0]+1==lesX[g] && lesY[0]==lesY[g])
            {
                if(destination[Y_TAB]<lesY[0]){ // Quand la destination et le tête du serpent sont sur la même colonne, si la destination est au dessus de la tête du serpent, le serpent va en haut
                    *direction=HAUT;
                }else if(destination[Y_TAB]>lesY[0]){ // Quand la destination et le tête du serpent sont sur la même colonne, si la destination est en dessous de la tête du serpent, le serpent va en bas
                    *direction=BAS;
                }
            }
        } else if (*direction==HAUT)
        {
            if (lesX[0]==lesX[g] && lesY[0]-1==lesY[g])
            {
                if(destination[X_TAB]<lesX[0]){ // Si la destination est à gauche de la tête du serpent, le serpent va à gauche
                    *direction=GAUCHE;
                }else if(destination[X_TAB]>lesX[0]){ // Si la destination est à droite de la tête du serpent, le serpent va à droite
                    *direction=DROITE;
                }
            }
        } else if (*direction==GAUCHE)
        {
            if (lesX[0]-1==lesX[g] && lesY[0]==lesY[g])
            {
                if(destination[Y_TAB]<lesY[0]){ // Quand la destination et le tête du serpent sont sur la même colonne, si la destination est au dessus de la tête du serpent, le serpent va en haut
                    *direction=HAUT;
                }else if(destination[Y_TAB]>lesY[0]){ // Quand la destination et le tête du serpent sont sur la même colonne, si la destination est en dessous de la tête du serpent, le serpent va en bas
                    *direction=BAS;
                }
            }
        } else if (*direction==BAS)
        {
            if (lesX[0]==lesX[g] && lesY[0]+1==lesY[g])
            {
                if(destination[X_TAB]<lesX[0]){ // Si la destination est à gauche de la tête du serpent, le serpent va à gauche
                    *direction=GAUCHE;
                }else if(destination[X_TAB]>lesX[0]){ // Si la destination est à droite de la tête du serpent, le serpent va à droite
                    *direction=DROITE;
                }
            }
        }
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
        *portailPasser=true;
    }else if((lesY[0]<MIN) && ((lesX[0]>=((MAX_LONG/2)-TAILLE_TROU)) && (lesX[0]<=((MAX_LONG/2))))){ // Vérifier si le serpent a traversé le portail en haut
        lesY[0]=MAX_HAUT;
        *portailPasser=true;
    }else if((lesX[0]>MAX_LONG) && ((lesY[0]>=((MAX_HAUT/2)-TAILLE_TROU)) && (lesY[0]<=((MAX_HAUT/2))))){ // Vérifier si le serpent a traversé le portail de droite
        lesX[0]=MIN;
        *portailPasser=true;
    }else if((lesY[0]>MAX_HAUT) && ((lesX[0]>=((MAX_LONG/2)-TAILLE_TROU)) && (lesX[0]<=((MAX_LONG/2))))){ // Vérifier si le serpent a traversé le portail en bas
        lesY[0]=MIN;
        *portailPasser=true;
    }
    if((lesX[0]==pomme[X_TAB])&&(lesY[0]==pomme[Y_TAB])){ // Vérifier si le serpent a mangé la pomme
        *mangee=true;
    }
    dessinerSerpent(lesX,lesY,taille);
}

/**
* @fn progresserG
* @brief Met à jour les positions du serpent fantôme
* @param lesXG : Tableau des positions en X du serpent fantôme
* @param lesYG : Tableau des positions en Y du serpent fantôme
* @param direction : Direction dans laquelle se déplace le serpent fantôme
* @param collision : Pointeur sur un booléen indiquant si le serpent a touché un obstacle
* @param taille : Taille du serpent
* @param pomme : Tableau des positions de la pomme
* @param mangee : Pointeur sur un booléen indiquant si le serpent a mangé la pomme
*/
void progresserG(int lesX[TAILLE_MAX], int lesY[TAILLE_MAX], char *direction, int taille, int destination[2], int pomme[2], bool *mangee, int *deplacement){
    
    // DÉCLARATION DES VARIABLES

    int i; // Compteur

    // AFFICHAGE

    if((*direction==HAUT)||(*direction==BAS)){ // Si le serpent avance vers la droite ou vers la gauche, il ne peut changer de direction que vers en bas ou vers en haut
        if(destination[X_TAB]<lesX[0]){ // Si la destination est à gauche de la tête du serpent, le serpent va à gauche
            *direction=GAUCHE;
        }else if(destination[X_TAB]>lesX[0]){ // Si la destination est à droite de la tête du serpent, le serpent va à droite
            *direction=DROITE;
        }
    }else if((*direction==GAUCHE)||(*direction==DROITE)){ // Si le serpent avance vers le bas ou vers le haut, il ne peut changer de direction que vers la droite ou vers la gauche
        if(destination[Y_TAB]<lesY[0]){ // Quand la destination et le tête du serpent sont sur la même colonne, si la destination est au dessus de la tête du serpent, le serpent va en haut
            *direction=HAUT;
        }else if(destination[Y_TAB]>lesY[0]){ // Quand la destination et le tête du serpent sont sur la même colonne, si la destination est en dessous de la tête du serpent, le serpent va en bas
            *direction=BAS;
        }
    }
    for (int g = 2; g < taille; g++)
	{
        if (*direction==DROITE)
        {
            if (lesX[0]+1==lesX[g] && lesY[0]==lesY[g])
            {
                if(destination[Y_TAB]<lesY[0]){ // Quand la destination et le tête du serpent sont sur la même colonne, si la destination est au dessus de la tête du serpent, le serpent va en haut
                    *direction=HAUT;
                }else if(destination[Y_TAB]>lesY[0]){ // Quand la destination et le tête du serpent sont sur la même colonne, si la destination est en dessous de la tête du serpent, le serpent va en bas
                    *direction=BAS;
                }
            }
        } else if (*direction==HAUT)
        {
            if (lesX[0]==lesX[g] && lesY[0]-1==lesY[g])
            {
                if(destination[X_TAB]<lesX[0]){ // Si la destination est à gauche de la tête du serpent, le serpent va à gauche
                    *direction=GAUCHE;
                }else if(destination[X_TAB]>lesX[0]){ // Si la destination est à droite de la tête du serpent, le serpent va à droite
                    *direction=DROITE;
                }
            }
        } else if (*direction==GAUCHE)
        {
            if (lesX[0]-1==lesX[g] && lesY[0]==lesY[g])
            {
                if(destination[Y_TAB]<lesY[0]){ // Quand la destination et le tête du serpent sont sur la même colonne, si la destination est au dessus de la tête du serpent, le serpent va en haut
                    *direction=HAUT;
                }else if(destination[Y_TAB]>lesY[0]){ // Quand la destination et le tête du serpent sont sur la même colonne, si la destination est en dessous de la tête du serpent, le serpent va en bas
                    *direction=BAS;
                }
            }
        } else if (*direction==BAS)
        {
            if (lesX[0]==lesX[g] && lesY[0]+1==lesY[g])
            {
                if(destination[X_TAB]<lesX[0]){ // Si la destination est à gauche de la tête du serpent, le serpent va à gauche
                    *direction=GAUCHE;
                }else if(destination[X_TAB]>lesX[0]){ // Si la destination est à droite de la tête du serpent, le serpent va à droite
                    *direction=DROITE;
                }
            }
        }
	}

    for(i=taille-1;i>0;i--){ // La partie du corps reprend les coordonnées de celle devant lui
        lesX[i]=lesX[i-1];
        lesY[i]=lesY[i-1];
    }
    if(*direction==DROITE){ // Dans quelle direction va le serpent fantôme
        lesX[0]=lesX[0]+1;
    }else if(*direction==GAUCHE){
        lesX[0]=lesX[0]-1;
    }else if(*direction==HAUT){
        lesY[0]=lesY[0]-1;
    }else if(*direction==BAS){
        lesY[0]=lesY[0]+1;
    }
    if((lesX[0]<MIN) && ((lesY[0]>=((MAX_HAUT/2)-TAILLE_TROU)) && (lesY[0]<=((MAX_HAUT/2))))){ // Vérifier si le serpent fantôme a traversé le portail de gauche
        lesX[0]=MAX_LONG;
        while(!*mangee){
            progresserPortailG(pomme, deplacement, lesX, lesY, direction, mangee, taille);
        }
    }else if((lesY[0]<MIN) && ((lesX[0]>=((MAX_LONG/2)-TAILLE_TROU)) && (lesX[0]<=((MAX_LONG/2))))){ // Vérifier si le serpent fantôme a traversé le portail en haut
        lesY[0]=MAX_HAUT;
        while(!*mangee){
            progresserPortailG(pomme, deplacement, lesX, lesY, direction, mangee, taille);
        }
    }else if((lesX[0]>MAX_LONG) && ((lesY[0]>=((MAX_HAUT/2)-TAILLE_TROU)) && (lesY[0]<=((MAX_HAUT/2))))){ // Vérifier si le serpent fantôme a traversé le portail de droite
        lesX[0]=MIN;
        while(!*mangee){
            progresserPortailG(pomme, deplacement, lesX, lesY, direction, mangee, taille);
        }
    }else if((lesY[0]>MAX_HAUT) && ((lesX[0]>=((MAX_LONG/2)-TAILLE_TROU)) && (lesX[0]<=((MAX_LONG/2))))){ // Vérifier si le serpent fantôme a traversé le portail en bas
        lesY[0]=MIN;
        while(!*mangee){
            progresserPortailG(pomme, deplacement, lesX, lesY, direction, mangee, taille);
        }
    }
    if((lesX[0]==pomme[X_TAB])&&(lesY[0]==pomme[Y_TAB])){ // Vérifier si le serpent fantôme a théoriquement mangé la pomme
        *mangee=true;
    }
}

/**
* @fn progresserPortail
* @brief
* @param portail : De quel portail on commence
* @param pomme : emplacement de la pomme et destination finale
* @param deplacement : compte le nombre de déplacements du serpent
*/
void progresserPortail(int lesX[TAILLE_MAX], int lesY[TAILLE_MAX], char direction, bool *collision, int taille, int pomme[2], bool *mangee){
    
    // DÉCLARATION DES VARIABLES

    int i,g; // Compteur

    // AFFICHAGE

    if((*direction==HAUT)||(*direction==BAS)){ // Si le serpent avance vers la droite ou vers la gauche, il ne peut changer de direction que vers en bas ou vers en haut
        if(pomme[X_TAB]<lesX[0]){ // Si la pomme est à gauche de la tête du serpent, le serpent va à gauche
            *direction=GAUCHE;
        }else if(pomme[X_TAB]>lesX[0]){ // Si la pomme est à droite de la tête du serpent, le serpent va à droite
            *direction=DROITE;
        }
    }else if((*direction==GAUCHE)||(*direction==DROITE)){ // Si le serpent avance vers le bas ou vers le haut, il ne peut changer de direction que vers la droite ou vers la gauche
        if(pomme[Y_TAB]<lesY[0]){ // Quand la pomme et le tête du serpent sont sur la même colonne, si la pomme est au dessus de la tête du serpent, le serpent va en haut
            *direction=HAUT;
        }else if(pomme[Y_TAB]>lesY[0]){ // Quand la pomme et le tête du serpent sont sur la même colonne, si la pomme est en dessous de la tête du serpent, le serpent va en bas
            *direction=BAS;
        }
    }
    for(g = 2; g < taille; g++){
        if(*direction==DROITE){
            if(lesX[0]+1==lesX[g] && lesY[0]==lesY[g]){
                if(pomme[Y_TAB]<lesY[0]){ // Quand la pomme et le tête du serpent sont sur la même colonne, si la pomme est au dessus de la tête du serpent, le serpent va en haut
                    *direction=HAUT;
                }else if(pomme[Y_TAB]>lesY[0]){ // Quand la pomme et le tête du serpent sont sur la même colonne, si la pomme est en dessous de la tête du serpent, le serpent va en bas
                    *direction=BAS;
                }
            }
        }else if(*direction==HAUT){
            if(lesX[0]==lesX[g] && lesY[0]-1==lesY[g]){
                if(pomme[X_TAB]<lesX[0]){ // Si la pomme est à gauche de la tête du serpent, le serpent va à gauche
                    *direction=GAUCHE;
                }else if(pomme[X_TAB]>lesX[0]){ // Si la pomme est à droite de la tête du serpent, le serpent va à droite
                    *direction=DROITE;
                }
            }
        }else if(*direction==GAUCHE){
            if(lesX[0]-1==lesX[g] && lesY[0]==lesY[g]){
                if(pomme[Y_TAB]<lesY[0]){ // Quand la pomme et le tête du serpent sont sur la même colonne, si la pomme est au dessus de la tête du serpent, le serpent va en haut
                    *direction=HAUT;
                }else if(pomme[Y_TAB]>lesY[0]){ // Quand la pomme et le tête du serpent sont sur la même colonne, si la pomme est en dessous de la tête du serpent, le serpent va en bas
                    *direction=BAS;
                }
            }
        }else if(*direction==BAS){
            if(lesX[0]==lesX[g] && lesY[0]+1==lesY[g]){
                if(pomme[X_TAB]<lesX[0]){ // Si la pomme est à gauche de la tête du serpent, le serpent va à gauche
                    *direction=GAUCHE;
                }else if(pomme[X_TAB]>lesX[0]){ // Si la pomme est à droite de la tête du serpent, le serpent va à droite
                    *direction=DROITE;
                }
            }
        }
	}
    for(i=taille-1;i>0;i--){ // La partie du corps reprend les coordonnées de celle devant lui
        lesX[i]=lesX[i-1];
        lesY[i]=lesY[i-1];
    }
    if(*direction==DROITE){ // Dans quelle direction va le serpent fantôme
        lesX[0]=lesX[0]+1;
    }else if(*direction==GAUCHE){
        lesX[0]=lesX[0]-1;
    }else if(*direction==HAUT){
        lesY[0]=lesY[0]-1;
    }else if(*direction==BAS){
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
    if((lesX[0]==pomme[X_TAB])&&(lesY[0]==pomme[Y_TAB])){ // Vérifier si le serpent fantôme a théoriquement mangé la pomme
        *mangee=true;
    }
    dessinerSerpent(lesX,lesY,taille);
}

/**
* @fn progresserPortailG
* @brief
* @param portail : De quel portail on commence
* @param pomme : emplacement de la pomme et destination finale
* @param deplacement : compte le nombre de déplacements du serpent
*/
void progresserPortailG(int pomme[2], int *deplacement, int lesX[TAILLE_MAX], int lesY[TAILLE_MAX], char *direction, bool *mangee, int taille){
    
    // DÉCLARATION DES VARIABLES

    int i,g; // Compteur

    // AFFICHAGE

    if((*direction==HAUT)||(*direction==BAS)){ // Si le serpent avance vers la droite ou vers la gauche, il ne peut changer de direction que vers en bas ou vers en haut
        if(pomme[X_TAB]<lesX[0]){ // Si la pomme est à gauche de la tête du serpent, le serpent va à gauche
            *direction=GAUCHE;
        }else if(pomme[X_TAB]>lesX[0]){ // Si la pomme est à droite de la tête du serpent, le serpent va à droite
            *direction=DROITE;
        }
    }else if((*direction==GAUCHE)||(*direction==DROITE)){ // Si le serpent avance vers le bas ou vers le haut, il ne peut changer de direction que vers la droite ou vers la gauche
        if(pomme[Y_TAB]<lesY[0]){ // Quand la pomme et le tête du serpent sont sur la même colonne, si la pomme est au dessus de la tête du serpent, le serpent va en haut
            *direction=HAUT;
        }else if(pomme[Y_TAB]>lesY[0]){ // Quand la pomme et le tête du serpent sont sur la même colonne, si la pomme est en dessous de la tête du serpent, le serpent va en bas
            *direction=BAS;
        }
    }
    for(g = 2; g < taille; g++){
        if(*direction==DROITE){
            if(lesX[0]+1==lesX[g] && lesY[0]==lesY[g]){
                if(pomme[Y_TAB]<lesY[0]){ // Quand la pomme et le tête du serpent sont sur la même colonne, si la pomme est au dessus de la tête du serpent, le serpent va en haut
                    *direction=HAUT;
                }else if(pomme[Y_TAB]>lesY[0]){ // Quand la pomme et le tête du serpent sont sur la même colonne, si la pomme est en dessous de la tête du serpent, le serpent va en bas
                    *direction=BAS;
                }
            }
        }else if(*direction==HAUT){
            if(lesX[0]==lesX[g] && lesY[0]-1==lesY[g]){
                if(pomme[X_TAB]<lesX[0]){ // Si la pomme est à gauche de la tête du serpent, le serpent va à gauche
                    *direction=GAUCHE;
                }else if(pomme[X_TAB]>lesX[0]){ // Si la pomme est à droite de la tête du serpent, le serpent va à droite
                    *direction=DROITE;
                }
            }
        }else if(*direction==GAUCHE){
            if(lesX[0]-1==lesX[g] && lesY[0]==lesY[g]){
                if(pomme[Y_TAB]<lesY[0]){ // Quand la pomme et le tête du serpent sont sur la même colonne, si la pomme est au dessus de la tête du serpent, le serpent va en haut
                    *direction=HAUT;
                }else if(pomme[Y_TAB]>lesY[0]){ // Quand la pomme et le tête du serpent sont sur la même colonne, si la pomme est en dessous de la tête du serpent, le serpent va en bas
                    *direction=BAS;
                }
            }
        }else if(*direction==BAS){
            if(lesX[0]==lesX[g] && lesY[0]+1==lesY[g]){
                if(pomme[X_TAB]<lesX[0]){ // Si la pomme est à gauche de la tête du serpent, le serpent va à gauche
                    *direction=GAUCHE;
                }else if(pomme[X_TAB]>lesX[0]){ // Si la pomme est à droite de la tête du serpent, le serpent va à droite
                    *direction=DROITE;
                }
            }
        }
	}
    for(i=taille-1;i>0;i--){ // La partie du corps reprend les coordonnées de celle devant lui
            lesX[i]=lesX[i-1];
            lesY[i]=lesY[i-1];
        }
        if(*direction==DROITE){ // Dans quelle direction va le serpent fantôme
            lesX[0]=lesX[0]+1;
        }else if(*direction==GAUCHE){
            lesX[0]=lesX[0]-1;
        }else if(*direction==HAUT){
            lesY[0]=lesY[0]-1;
        }else if(*direction==BAS){
            lesY[0]=lesY[0]+1;
        }
        if((lesX[0]==pomme[X_TAB])&&(lesY[0]==pomme[Y_TAB])){ // Vérifier si le serpent fantôme a théoriquement mangé la pomme
            *mangee=true;
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
