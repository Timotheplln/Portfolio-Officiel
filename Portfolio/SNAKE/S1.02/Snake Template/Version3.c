/**
* @brief troisième version du jeu de snake
* @brief intégrer les collisions au snake
* @author Armel KERMANAC'H 1C1
* @version 3.3
* @date 15/11/2024
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

const int TAILLE=10; // Taille du serpent
const int TEMPORISATION=50000; // Nombre de µs pour la pause entre chaque affichage du serpent
const int TAILLE_PAVE=5; // Taille de la hauteur et de la longueur d'un pavé
const int NB_PAVES=4; // Nombre de pavés à générer
const int X_PAVE=0; // Colonne dans le tableau posPave répertoriant les colonnes des pavés
const int Y_PAVE=1; // Colonne dans le tableau posPave répertoriant les lignes des pavés
const int MIN=1; // Coordonnée minimale pour la position du serpent
const int MAX_LONG=80; // Coordonnée maximale sur la longueur pour la position du serpent
const int MAX_HAUT=40; // Coordonnée maximale sur la hauteur pour la position du serpent
const int X=40; // Position initiale de la tête sur les colonnes
const int Y=20; // Position initiale de la tête sur les lignes
const char TETE='O'; // Forme de la tête du serpent
const char ANNEAUX='X'; // Forme du reste du corps du serpent
const char BORDURE='#'; // Caractère délimitant la bordure du terrain de jeu
const char HAUT='z'; // Touche à appuyer pour faire avancer le serpent vers le haut
const char GAUCHE='q'; // Touche à appuyer pour faire avancer le serpent vers la gauche
const char BAS='s'; // Touche à appuyer pour faire avancer le serpent vers le bas
const char DROITE='d'; // Touche à appuyer pour faire avancer le serpent vers la droite
const char ARRET='a'; // Touche à appuyer pour arrêter le programme
const char VIDE=' '; // Le caractère par lequel les caractères "effacés" sont remplacés

/*******************************************
*  DÉCLARATION DES FONCTIONS & PROCÉDURES  *
*******************************************/

void dessinerSerpent(int lesX[TAILLE], int lesY[TAILLE]); // Affiche un à un les éléments du serpent sur l'écran
void progresser(int lesX[TAILLE], int lesY[TAILLE], char direction, bool *collision, int posPave[NB_PAVES][2]); // Calcule et affiche la prochaine position du serpent
void afficher(int x, int y, char c); // Affiche le car c à la position x y
void effacer(int x, int y); // Remplace le car à la position x y par un espace
void initPlateau(char plateau[MAX_LONG+1][MAX_HAUT+1]); // Initialisation du plateau
void dessinerPlateau(char plateau[MAX_LONG+1][MAX_HAUT+1]); // Affiche le plateau de jeu sur l'écran
int aleatPlateau(int max); // Génère un nombre aléatoire entre MIN et MAX
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
    int i,j,k; // Compteurs
    int posPave[NB_PAVES][2]; // Position des pavés sur le plateau
    char toucheAppuyer; // Vérifie quelle touche à été appuyée
    char direction; // Indique dans quel direction le serpent doit se diriger
    int lesY[TAILLE],lesX[TAILLE]; // Position de chaques parties du serpent
    char plateau[MAX_LONG+1][MAX_HAUT+1]; // Plateau de jeu
    bool collision; // Indique si le serpent a touché la bordure ou son corps
    // INITIALISATION DES DONNÉES
    system("clear"); // Passer à l'affichage du serpent
    initPlateau(plateau);
    collision=false;
    direction=DROITE; // Le serpent commence avec sa tête tournée vers la droite
    for(i=0;i<TAILLE;i++){ // Initialisation des coordonnées du serpent
        lesY[i]=Y; // Implémentation de la position du serpent sur les lignes
        lesX[i]=X-i; // Implémentation de la position du serpent sur les colonnes
    }
    // AFFICHAGE
    dessinerPlateau(plateau);
    for(i=0;i<NB_PAVES;i++){ // Génération des pavés sur le plateau
        posPave[i][X_PAVE]=aleatPlateau(MAX_LONG-(TAILLE_PAVE+1)); // Génération du de la position du pavé sur les colonnes
        do{
            posPave[i][Y_PAVE]=aleatPlateau(MAX_HAUT-(TAILLE_PAVE+1)); // Génération du de la position du pavé sur les lignes
        }while((posPave[i][Y_PAVE]<=Y)&&(posPave[i][Y_PAVE]+TAILLE_PAVE>Y)); // Pour éviter qu'un pavé se génère sur la même ligne que le serpent
        for(j=0;j<TAILLE_PAVE;j++){
            for(k=0;k<TAILLE_PAVE;k++){
                afficher(posPave[i][X_PAVE]+j,posPave[i][Y_PAVE]+k,BORDURE); // Afficher le pavé
            }
        }
    }
    dessinerSerpent(lesX,lesY);
    do{
        progresser(lesX,lesY,direction, &collision, posPave);
        usleep(TEMPORISATION); // Attendre 50000µs (0.05s)
        if(kbhit()==1){
            toucheAppuyer=getchar();
            if(((toucheAppuyer==GAUCHE)&&(direction!=DROITE))||((toucheAppuyer==BAS)&&(direction!=HAUT))||((toucheAppuyer==DROITE)&&(direction!=GAUCHE))||((toucheAppuyer==HAUT)&&(direction!=BAS))){
                direction=toucheAppuyer; // Si la touche appuyée est une direction sans être l'opposé de la direction actuelle, la direction est modifiée
            }
        }
    }while((toucheAppuyer!=ARRET)&&(collision==false)); // Si le caractère appuyé est un 'a' ou que le serpent a touché la bordure ou son corps, le programme s'arrête
    enableEcho();
    system("clear"); // Efface tout ce qu'il y a à l'écran
    return EXIT_SUCCESS;
}

/******************************************************
*  PROCÉDURES & FONCTIONS CRÉES PAR ARMEL KERMANAC'H  *
******************************************************/

void dessinerSerpent(int lesX[TAILLE], int lesY[TAILLE]){
    // DÉCLARATION DES VARIABLES
    int i; // Compteurs
    // AFFICHAGE
    for(i=0;i<TAILLE;i++){
        if((lesX[i]>=MIN)&&(lesY[i]>=MIN)){ // Affiche le serpent uniquement s'il est visible
            if(i==0){
                afficher(lesX[i],lesY[i],TETE); // Affichage de la tête
            }else if(lesX[i]>0){
                afficher(lesX[i],lesY[i],ANNEAUX); // Affichage du corps
            }
        }
    }
}

void progresser(int lesX[TAILLE], int lesY[TAILLE], char direction, bool *collision, int posPave[NB_PAVES][2]){
    // DÉCLARATION DES VARIABLES
    int i,j,k; // Compteurs
    // AFFICHAGE
    if((lesX[TAILLE-1]>=MIN)&&(lesY[TAILLE-1]>=MIN)){ // Supprimer le caractère derrière le serpent
        effacer(lesX[TAILLE-1],lesY[TAILLE-1]);
    }
    for(i=TAILLE-1;i>0;i--){ // La partie du corps reprend les coordonnées de celle devant lui
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
    for(i=1;i<TAILLE;i++){ // Vérifier si le serpent a touché son corps
        if((lesX[i]==lesX[0])&&(lesY[i]==lesY[0])){
            *collision=true;
        }
    }
    if((lesX[0]<=MIN)||(lesY[0]<=MIN)||(lesX[0]>=MAX_LONG)||(lesY[0]>=MAX_HAUT)){ // Vérifier si le serpent a touché la bordure
        *collision=true;
    }
    for(i=0;i<NB_PAVES;i++){ // Vérifier si le serpent a touché un pavé
        for(j=0;j<TAILLE_PAVE;j++){
            for(k=0;k<TAILLE_PAVE;k++){
                if((lesX[0]==posPave[i][X_PAVE]+j)&&(lesY[0]==posPave[i][Y_PAVE]+k)){
                    *collision=true;
                }
            }
        }
    }
    dessinerSerpent(lesX,lesY);
}

void afficher(int x, int y, char c){
    // AFFICHAGE
    gotoXY(x,y);
    printf("%c", c);
}

void effacer(int x, int y){
    // AFFICHAGE
    gotoXY(x,y);
    printf("%c", VIDE);
}

void initPlateau(char plateau[MAX_LONG+1][MAX_HAUT+1]){
    int i,j; // Compteurs
    // INITIALISATION DES DONNÉES
    for(i=MIN;i<=MAX_LONG;i++){
        plateau[i][MIN]=BORDURE;
    }
    for(i=MIN+1;i<MAX_HAUT;i++){
        plateau[MIN][i]=BORDURE;
        for(j=MIN+1;j<MAX_LONG;j++){
            plateau[j][i]=VIDE;
        }
        plateau[MAX_LONG][i]=BORDURE;
    }
    for(i=MIN;i<=MAX_LONG;i++){
        plateau[i][MAX_HAUT]=BORDURE;
    }
}

void dessinerPlateau(char plateau[MAX_LONG+1][MAX_HAUT+1]){
    int i,j; // Compteurs
    // AFFICHAGE
    for(i=MIN;i<=MAX_LONG;i++){
        for(j=MIN;j<=MAX_HAUT;j++){
            afficher(i,j,plateau[i][j]);
        }
    }
}

int aleatPlateau(int max){
    int random;
    // TRAITEMENT DES DONNÉES
    random=rand();
    random=(random%max)+2;
    return random;
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
