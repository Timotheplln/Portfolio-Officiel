/**
* @brief deuxième version du jeu de snake
* @brief déplacer le serpent dans quatre directions différentes
* @author Armel KERMANAC'H 1C1
* @version 2.4
* @date 08/11/2024
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

/*******************************
*  DÉCLARATION DES CONSTANTES  *
*******************************/

const int TAILLE=10; // Taille du serpent
const int TEMPORISATION=50000; // Nombre de µs pour la pause entre chaque affichage du serpent
const int MIN=1; // Coordonnée minimale pour la position du serpent
const int MAX=40; // Coordonnée maximale pour la position du serpent (pas utilisée)
const int X=20; // Position initiale de la tête sur les colonnes
const int Y=20; // Position initiale de la tête sur les lignes
const char TETE='O'; // Forme de la tête du serpent
const char ANNEAUX='X'; // Forme du reste du corps du serpent
const char HAUT='z'; // Touche à appuyer pour faire avancer le serpent vers le haut
const char GAUCHE='q'; // Touche à appuyer pour faire avancer le serpent vers la gauche
const char BAS='s'; // Touche à appuyer pour faire avancer le serpent vers le bas
const char DROITE='d'; // Touche à appuyer pour faire avancer le serpent vers la droite
const char ARRET='a'; // Touche à appuyer pour arrêter le programme
const char SUPPRIME=' '; // Le caractère par lequel les caractères "effacés" sont remplacés

/*******************************************
*  DÉCLARATION DES FONCTIONS & PROCÉDURES  *
*******************************************/

void gotoXY(int x, int y); // Aller aux coordonnées données
int kbhit(); // Vérifie si une touche à été pressée
void afficher(int x, int y, char c); // Affiche le car c à la position x y
void effacer(int x, int y); // Remplace le car à la position x y par un espace
void dessinerSerpent(int lesX[TAILLE], int lesY[TAILLE]); // Affiche un à un les éléments du serpent sur l'écran
void progresser(int lesX[TAILLE], int lesY[TAILLE], char direction); // Calcule et affiche la prochaine position du serpent
void disableEcho(); // Désactive l'affichage d'un caractère à l'écran
void enableEcho(); // Réactive l'affichage des caractères à l'écran

/************************
*  PROGRAMME PRINCIPAL  *
************************/

int main(){
    disableEcho();
    // DÉCLARATION DES VARIABLES
    int i;
    char toucheAppuyer; // Vérifie quelle touche à été appuyée
    char direction; // Indique dans quel direction le serpent doit se diriger
    int lesY[TAILLE],lesX[TAILLE]; // Position de chaques parties du serpent
    system("clear"); // Passer à l'affichage du serpent
    // INITIALISATION DES DONNÉES
    direction=DROITE; // Le serpent commence avec sa tête tournée vers la droite
    // TRAITEMENT DES DONNÉES
    for(i=0;i<TAILLE;i++){
        lesY[i]=Y; // Implémentation de la position du serpent sur les lignes
        lesX[i]=X-i; // Implémentation de la position du serpent sur les colonnes
    }
    // AFFICHAGE
    dessinerSerpent(lesX,lesY);
    do{
        progresser(lesX,lesY,direction);
        usleep(TEMPORISATION); // Attendre 50000µs (0.05s)
        if(kbhit()==1){
            toucheAppuyer=getchar();
            if(((toucheAppuyer==GAUCHE)&&(direction!=DROITE))||((toucheAppuyer==BAS)&&(direction!=HAUT))||((toucheAppuyer==DROITE)&&(direction!=GAUCHE))||((toucheAppuyer==HAUT)&&(direction!=BAS))){
                direction=toucheAppuyer; // Si la touche appuyée est une direction sans être l'opposé de la direction actuelle, la direction est modifiée
            }
        }
    }while(toucheAppuyer!=ARRET); // Si le caractère appuyé est un 'a', le programme s'arrête
    enableEcho();
    system("clear"); // Efface tout ce qu'il y a à l'écran
    return EXIT_SUCCESS;
}

/******************************************************
*  PROCÉDURES & FONCTIONS CRÉES PAR ARMEL KERMANAC'H  *
******************************************************/

void dessinerSerpent(int lesX[TAILLE], int lesY[TAILLE]){
    // DÉCLARATION DES VARIABLES
    int i;
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

void progresser(int lesX[TAILLE], int lesY[TAILLE], char direction){
    // DÉCLARATION DES VARIABLES
    int i;
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
    printf("%c", SUPPRIME);
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
